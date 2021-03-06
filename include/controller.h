#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QDebug>
#include <QThread>
#include <QMainWindow>
#include "timer.h"

#include "pid.h"
#include "Packetizer.hpp"

class Controller : public QThread
{
    Q_OBJECT
private:
    Timer timer;

    // translation
    float trans;

    // speed
    float trans_speed;
    float launch_speed;


	Packetizer *packetizer;
	double rpm_factor;

signals:
    void ControllerFeedback(double trans, double speed);

protected:
    void run();

public:
    Controller(QObject *parent = 0);
    PID pid;
    ~Controller();
};

#endif // CONTROLLER_H
