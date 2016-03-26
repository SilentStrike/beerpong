#include "player.h"

Player::Player(QObject *parent) : QThread (parent)
{
    framerate = 30;
    dist_frame = Mat(Size(640,480),CV_16UC1);
    depth_frame = Mat(Size(640,480),CV_8UC1);
    video_frame = Mat(Size(640,480),CV_8UC3);

    // setup kinect
    freenect = new Freenect::Freenect();
    Kinect &k = freenect->createDevice<Kinect>(0);
    kinect = &k;
    kinect->startVideo();
    kinect->startDepth();
}

Player::~Player()
{
    kinect->stopVideo();
    kinect->stopDepth();
}

void Player::run()
{
    timer.Reset();

    // grab image
    kinect->GetVideo(video_frame);
    kinect->GetDepth(dist_frame);

    // process code here

    // emit image
    emit ProcessedVideo(MattoBGRQImage(video_frame));
    emit ProcessedDepth(MattoQImage(depth_frame));

    // update time
    msleep(((1000 / framerate) - timer.GetElapsedTime()) > 0 ? (1000 / framerate) - timer.GetElapsedTime() : 0);
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
