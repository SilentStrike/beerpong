#include "player.h"

Player::Player(QObject *parent) : QThread (parent)
{
    framerate = 100;
    depthMat = Mat(Size(640,480),CV_16UC1);
    depthF = Mat(Size(640,480),CV_8UC1);
    depthD = Mat(Size(640,480),CV_32FC1,Scalar(0));
    rgbMat = Mat(Size(640,480),CV_8UC3,Scalar(0));
    ownMat = Mat(Size(640,480),CV_8UC3,Scalar(0));

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
        emit ProcessedVideo(MattoBGRQImage(rgbMatMinRange));

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
