#ifndef PLAYER_H
#define PLAYER_H

#include <sys/time.h>
#include <iostream>
#include <QDebug>
#include <QThread>
#include <QImage>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "timer.h"
#include "kinect.h"

using namespace cv;
using namespace std;

class Player : public QThread
{
    Q_OBJECT
private:
    // depth map
    float min_depth;
    float max_depth;

    // color
    int h_lo;
    int h_hi;
    int s_lo;
    int s_hi;
    int v_lo;
    int v_hi;

    // video
    float framerate;
    Timer timer;

    // kinect
    Freenect::Freenect *freenect;
    Kinect *kinect;

    // frames
    Mat depthMat;
    Mat depthF;
    Mat depthD;
    Mat rgbMat;
    Mat ownMat;

signals:
    void ProcessedVideo(const QImage image);
    void ProcessedDepth(const QImage image);

protected:
    void run();

public:
    Player(QObject *parent = 0);
    ~Player();

    void SetMinDepth(int value){min_depth = value;}
    void SetMaxDepth(int value){max_depth = value;}
    void SetHLo(int value){h_lo = value;}
    void SetHHi(int value){h_hi = value;}
    void SetSLo(int value){s_lo = value;}
    void SetSHi(int value){s_hi = value;}
    void SetVLo(int value){v_lo = value;}
    void SetVHi(int value){v_hi = value;}

    QImage MattoQImage(cv::Mat frame);
    QImage MattoBGRQImage(cv::Mat frame);
    static void MatDeleter(void* mat){ delete static_cast<cv::Mat*> (mat); }
};

#endif // PLAYER_H
