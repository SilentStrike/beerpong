#ifndef KINECT_H
#define KINECT_H

#include <vector>
#include <cmath>
#include <pthread.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "libfreenect/libfreenect.hpp"

using namespace cv;

class Kinect : public Freenect::FreenectDevice
{
private:
    class Mutex
    {
    private:
        pthread_mutex_t mutex;

    public:
        Mutex() {pthread_mutex_init(&mutex, NULL);}
        void lock() {pthread_mutex_lock(&mutex);}
        void unlock() {pthread_mutex_unlock(&mutex);}
    };

    std::vector<uint8_t> m_buffer_depth;
    std::vector<uint8_t> m_buffer_rgb;
    std::vector<uint16_t> m_gamma;
    Mat depthMat;
    Mat rgbMat;
    Mat ownMat;
    Mutex m_rgb_mutex;
    Mutex m_depth_mutex;
    bool m_new_rgb_frame;
    bool m_new_depth_frame;

public:
    Kinect(freenect_context *_ctx, int _index)
        : Freenect::FreenectDevice(_ctx, _index),
        m_buffer_depth(freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_REGISTERED).bytes / 2),
        m_buffer_rgb(freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB).bytes),
        m_gamma(2048), m_new_rgb_frame(false),
        m_new_depth_frame(false), depthMat(Size(640,480),CV_16UC1),
        rgbMat(Size(640,480), CV_8UC3, Scalar(0)),
        ownMat(Size(640,480),CV_8UC3,Scalar(0))
    {
        setDepthFormat(FREENECT_DEPTH_REGISTERED);
    }

    void VideoCallback(void* _rgb, uint32_t timestamp);
    void DepthCallback(void* _depth, uint32_t timestamp);
    bool GetVideo(Mat& output);
    bool GetDepth(Mat& output);
};

#endif // KINECT_H
