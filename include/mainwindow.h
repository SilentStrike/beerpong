#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "controller.h"
#include "player.h"

namespace Ui {
class MainWindow;
}

// robot state info
enum STATE
{
    MANUAL,
    AUTO
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // threads
    Controller *controller;
    Player *player;

    // depth map
    float min_dist;
    float max_dist;

    // color
    int h_lo;
    int h_hi;
    int s_lo;
    int s_hi;
    int v_lo;
    int v_hi;

    // translation
    float trans;

    // speed
    float trans_speed;
    float launch_speed;

private slots:
    void UpdateFeedback(float trans, float speed);
    void UpdateDepth(const QImage image);
    void UpdateColor(const QImage image);
};

#endif // MAINWINDOW_H
