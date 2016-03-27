#ifndef CONTROLLER_H
#define CONTROLLER_H

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

	PID pid;
	Packetizer *packetizer;
	double rpm_factor;

signals:
    void ControllerFeedback(float trans, float speed);

protected:
    void run();

public:
    Controller(QObject *parent = 0);
    ~Controller();
};

#endif // CONTROLLER_H
