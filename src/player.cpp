#include "player.h"

Player::Player(QObject *parent) : QThread (parent)
{
    framerate = 100;
    depthMat = Mat(Size(640,480),CV_16UC1);
    depthF = Mat(Size(640,480),CV_8UC1);
    depthD = Mat(Size(640,480),CV_32FC1,Scalar(0));
    rgbMat = Mat(Size(640,480),CV_8UC3,Scalar(0));
    ownMat = Mat(Size(640,480),CV_8UC3,Scalar(0));
    minarea = 100;

    // setup kinect
    freenect = new Freenect::Freenect();
    Kinect &k = freenect->createDevice<Kinect>(0);
    kinect = &k;
    kinect->startVideo();
    kinect->startDepth();
    start();
}

Player::~Player()
{
    kinect->stopVideo();
    kinect->stopDepth();
}

float Player::CalcSpeed(float x, float y, float theta)
{
    float top = pow(x,2) * GRAVITY;
    float bot = x * sin(2 * theta) - 2 * y * pow(cos(theta),2);
    return sqrt(top / bot);
}

void Player::run()
{
    while(1)
    {
        timer.Reset();

        // grab image
        kinect->GetVideo(rgbMat);
        kinect->GetDepth(depthMat);

        // threshold distance
        depthMat.convertTo(depthD, CV_32FC1);
        threshold(depthD, depthD, min_depth, 0, CV_THRESH_TOZERO);
        threshold(depthD, depthD, max_depth, 0, CV_THRESH_TOZERO_INV);
        depthD.convertTo(depthMat, CV_16UC1);
        depthMat.convertTo(depthF, CV_8UC1, 255.0/2048.0);
        Mat binImage(Size(640,480),CV_8UC1,Scalar(0));
        Mat binImage2(Size(640,480),CV_8UC1,Scalar(0));
        threshold(depthF, binImage, 10, 255, CV_THRESH_BINARY);
        Mat rgbMatMinDepth(Size(640,480),CV_8UC3,Scalar(0));
        rgbMat.copyTo(rgbMatMinDepth, binImage);
        emit ProcessedDepth(MattoQImage(depthF));

        // threshold color
        medianBlur(rgbMat,rgbMat,5);
        Mat rgbMatMinRange(Size(640,480),CV_8UC3,Scalar(0));
        Mat rgbMatMinDepthHSV(Size(640,480),CV_8UC3,Scalar(0));
        Mat rgbMatMinDepthHSV2(Size(640,480),CV_8UC3,Scalar(0));
        cv::cvtColor(rgbMatMinDepth, rgbMatMinDepthHSV, CV_BGR2HSV);
        rgbMatMinDepthHSV.copyTo(rgbMatMinDepthHSV2);
        inRange(rgbMatMinDepthHSV, Scalar(0, s_lo, v_lo), Scalar(h_lo, s_hi, v_hi), binImage);
        inRange(rgbMatMinDepthHSV, Scalar(h_hi, s_lo, v_lo), Scalar(179, s_hi, v_hi), binImage2);
        bitwise_or(binImage, binImage2, binImage);
        erode(binImage, binImage, getStructuringElement(MORPH_RECT, Size(3, 3)));
        dilate(binImage, binImage, getStructuringElement(MORPH_RECT, Size(3, 3)));
        rgbMatMinDepth.copyTo(rgbMatMinRange, binImage);

        // find centroid and overlay
        int largest_area = 0;
        int largest_contour_index = 0;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        // find largest contour and overlay
        double a = 0;
        findContours(binImage, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
        for (int i = 0; i < contours.size(); i++)
        {
            a = contourArea(contours[i]);
            if (a > largest_area)
            {
                largest_area = a;
                largest_contour_index = i;
            }
            drawContours(binImage, contours, i, Scalar(255,255,255,0), CV_FILLED, 8, hierarchy);
        }
        if (a > minarea)
        {
            Moments moment = moments(contours[largest_contour_index], true);
            centroid = Point(moment.m10 / moment.m00, moment.m01 / moment.m00);
            circle(rgbMatMinRange, centroid, 5, Scalar(0, 255, 0), -1, 8, 0);
        }

        emit ProcessedVideo(MattoBGRQImage(rgbMatMinRange));

        // calculate cup distance
        distance = depthMat.at<uint16_t>(centroid.y, centroid.x);

        // calc launch speed
        speed = CalcSpeed(x, y, theta);

        // update time
        msleep(((1000 / framerate) - timer.GetElapsedTime()) > 0 ? (1000 / framerate) - timer.GetElapsedTime() : 0);
    }
}

QImage Player::MattoBGRQImage(cv::Mat frame)
{
    cvtColor(frame, frame, CV_RGB2BGR);
    const QImage img(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888, &MatDeleter, new cv::Mat(frame));
    return img;
}

QImage Player::MattoQImage(cv::Mat frame)
{
    if (frame.channels() == 3)
    {
        const QImage img(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888, &MatDeleter, new cv::Mat(frame));
        return img;
    } else {
        const QImage img(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_Indexed8, &MatDeleter, new cv::Mat(frame));
        return img;
    }
}
