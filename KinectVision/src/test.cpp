#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/highgui/highgui_c.h"

#include "libfreenect/libfreenect.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h>
//#include <cv.h>
#include <cxcore.h>

using namespace cv;
using namespace std;


// .7 to 6 m
class myMutex {
public:
    myMutex() {
        pthread_mutex_init( &m_mutex, NULL );
    }
    void lock() {
        pthread_mutex_lock( &m_mutex );
    }
    void unlock() {
        pthread_mutex_unlock( &m_mutex );
    }
private:
    pthread_mutex_t m_mutex;
};


class MyFreenectDevice : public Freenect::FreenectDevice {
public:
    MyFreenectDevice(freenect_context *_ctx, int _index)
    : Freenect::FreenectDevice(_ctx, _index),
    m_buffer_depth(freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_REGISTERED).bytes / 2),
    m_buffer_rgb(freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB).bytes),
    m_gamma(2048), m_new_rgb_frame(false),
    m_new_depth_frame(false), depthMat(Size(640,480),CV_16UC1),
    rgbMat(Size(640,480), CV_8UC3, Scalar(0)),
    ownMat(Size(640,480),CV_8UC3,Scalar(0)) {

        setDepthFormat(FREENECT_DEPTH_REGISTERED);
        // for( unsigned int i = 0 ; i < 2048 ; i++) {
        //     float v = i/2048.0;
        //     v = std::pow(v, 3)* 6;
        //     m_gamma[i] = v*6*256;
        // }
    }

    // Do not call directly even in child
    void VideoCallback(void* _rgb, uint32_t timestamp) {
        std::cout << "RGB callback" << std::endl;
        m_rgb_mutex.lock();
        uint8_t* rgb = static_cast<uint8_t*>(_rgb);
        rgbMat.data = rgb;
        m_new_rgb_frame = true;
        m_rgb_mutex.unlock();
    };

    // Do not call directly even in child
    void DepthCallback(void* _depth, uint32_t timestamp) {
        std::cout << "Depth callback" << std::endl;
        m_depth_mutex.lock();
        uint16_t* depth = static_cast<uint16_t*>(_depth);
        depthMat.data = (uchar*) depth;
        m_new_depth_frame = true;
        m_depth_mutex.unlock();
    }

    bool getVideo(Mat& output) {
        m_rgb_mutex.lock();
        if(m_new_rgb_frame) {
            cv::cvtColor(rgbMat, output, CV_RGB2BGR);
            m_new_rgb_frame = false;
            m_rgb_mutex.unlock();
            return true;
        } else {
            m_rgb_mutex.unlock();
            return false;
        }
    }

    bool getDepth(Mat& output) {
        m_depth_mutex.lock();
        if(m_new_depth_frame) {
            depthMat.copyTo(output);
            m_new_depth_frame = false;
            m_depth_mutex.unlock();
            return true;
        } else {
            m_depth_mutex.unlock();
            return false;
        }
    }
private:
    // std::vector<uint8_t> m_buffer_depth;
    std::vector<uint16_t> m_buffer_depth;
    std::vector<uint8_t> m_buffer_rgb;
    std::vector<uint16_t> m_gamma;
    Mat depthMat;
    Mat rgbMat;
    Mat ownMat;
    myMutex m_rgb_mutex;
    myMutex m_depth_mutex;
    bool m_new_rgb_frame;
    bool m_new_depth_frame;
};

float depthToMeters(Mat& depthMat, int y, int x) {
    int depthValue = depthMat.at<uint16_t>(y, x);
    // cout << "depth value: " << depthValue << endl;
    float depthM = 0.0; // FIXME: Not sure default value of depthM
    if (depthValue <= 2047) {
        depthM = 0.1236 * tan(depthValue / 2842.5 + 1.1863);
    }
    // cout << "depth :" << depthM << " meters" << endl;
    return depthM;
}

int main(int argc, char **argv) {
    bool die(false);
    string filename("snapshot");
    string suffix(".png");
    int i_snap(0),iter(0);

    Mat depthMat(Size(640,480),CV_16UC1);
    Mat depthf (Size(640,480),CV_8UC1);
    Mat depthD(Size(640,480),CV_32FC1,Scalar(0));
    Mat rgbMat(Size(640,480),CV_8UC3,Scalar(0));
    Mat ownMat(Size(640,480),CV_8UC3,Scalar(0));

    // The next two lines must be changed as Freenect::Freenect
    // isn't a template but the method createDevice:
    // Freenect::Freenect<MyFreenectDevice> freenect;
    // MyFreenectDevice& device = freenect.createDevice(0);
    // by these two lines:

    Freenect::Freenect freenect;
    MyFreenectDevice& device = freenect.createDevice<MyFreenectDevice>(0);

    namedWindow("rgb",CV_WINDOW_AUTOSIZE);
    namedWindow("depth",CV_WINDOW_AUTOSIZE);
    device.startVideo();
    device.startDepth();
    while (!die) {
        device.getVideo(rgbMat);
        device.getDepth(depthMat);


        //threshold(depthf, )
        depthMat.convertTo(depthD, CV_32FC1);
        threshold(depthD, depthD, 1000, 0, CV_THRESH_TOZERO);
        //depthMat_tmp.copyTo(depthMat);
        threshold(depthD, depthD, 2000, 0, CV_THRESH_TOZERO_INV);
        //depthMat_tmp.copyTo(depthMat);

        //cout << "distance: " << depthToMeters(depthMat, 480/2, 640/2) << endl;
        cout << "distance: " << depthMat.at<uint16_t>(480/2, 640/2) << endl;

        depthD.convertTo(depthMat, CV_16UC1);
        depthMat.convertTo(depthf, CV_8UC1, 255.0/2048.0);
        cv::imshow("depth",depthf);

        Mat binImage(Size(640,480),CV_8UC1,Scalar(0));
        Mat binImage2(Size(640,480),CV_8UC1,Scalar(0));
        threshold(depthf, binImage, 10, 255, CV_THRESH_BINARY);

        // bitwise_and(rgbMat, rgbMat, rgbMat, depthB);
        Mat rgbMatMinDepth(Size(640,480),CV_8UC3,Scalar(0));
        rgbMat.copyTo(rgbMatMinDepth, binImage);

        Mat rgbMatMinRange(Size(640,480),CV_8UC3,Scalar(0));
        Mat rgbMatMinDepthHSV(Size(640,480),CV_8UC3,Scalar(0));
        Mat rgbMatMinDepthHSV2(Size(640,480),CV_8UC3,Scalar(0));
        cv::cvtColor(rgbMatMinDepth, rgbMatMinDepthHSV, CV_BGR2HSV);
        rgbMatMinDepthHSV.copyTo(rgbMatMinDepthHSV2);
        inRange(rgbMatMinDepthHSV, Scalar(0, 80, 80), Scalar(20, 255, 255), binImage);
        inRange(rgbMatMinDepthHSV, Scalar(150, 80, 80), Scalar(179, 255, 255), binImage2);
        bitwise_or(binImage, binImage2, binImage);
        rgbMatMinDepth.copyTo(rgbMatMinRange, binImage);

        cv::imshow("rgb", rgbMatMinRange);
        char k = cvWaitKey(5);
        if( k == 27 ){
            cvDestroyWindow("rgb");
            cvDestroyWindow("depth");
            break;
        }
        if( k == 8 ) {
            std::ostringstream file;
            file << filename << i_snap << suffix;
            cv::imwrite(file.str(),rgbMat);
            i_snap++;
        }
        if(iter >= 10000) break;
        iter++;
    }

    device.stopVideo();
    device.stopDepth();
    return 0;
}

/* Source: https://openkinect.org/wiki/C%2B%2BOpenCvExample */
