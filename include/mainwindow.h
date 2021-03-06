#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
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
    bool m_auto;
    void exit();

    Ui::MainWindow *ui;

    // threads
    Controller *controller;
    Player *player;

private slots:
    void UpdateFeedback(double trans, double speed);
    void UpdateVideo(const QImage image);
    void UpdateDepth(const QImage image);
    void UpdateDist(float dist, float speed);
    void on_minDepthSlider_valueChanged(int value);
    void on_maxDepthSlider_valueChanged(int value);
    void on_hueLoSlider_valueChanged(int value);
    void on_hueHiSlider_valueChanged(int value);
    void on_satLoSlider_valueChanged(int value);
    void on_satHiSlider_valueChanged(int value);
    void on_valLoSlider_valueChanged(int value);
    void on_valHiSlider_valueChanged(int value);
    void on_pBox_returnPressed();
    void on_iBox_returnPressed();
    void on_dBox_returnPressed();
    void on_pidReset_clicked();
    void on_desiredLaunchBox_returnPressed();
    void on_maxiBox_returnPressed();
    void on_autoButton_clicked();
    void on_manualButton_clicked();
};

#endif // MAINWINDOW_H
