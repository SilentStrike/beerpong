#ifndef PLAYER_H
#define PLAYER_H

#include <QThread>
#include <QTimer>
#include <QImage>
#include <opencv2/core/core.hpp>

using namespace cv;

class Player : public QThread
{
    Q_OBJECT
private:
    Mat depth_frame;
    Mat color_frame;
    QTimer t;

signals:
    void ProcessedDepth(const QImage image);
    void ProcessedColor(const QImage image);

protected:
    void run();

public:
    Player(QObject *parent = 0);
    ~Player();

    QImage MattoQImage(cv::Mat frame);
    QImage MattoBGRQImage(cv::Mat frame);
    static void MatDeleter(void* mat){ delete static_cast<cv::Mat*> (mat); }
};

#endif // PLAYER_H
