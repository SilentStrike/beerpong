#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QThread>
#include <QTimer>

#include "pid.h"

class Controller : public QThread
{
    Q_OBJECT
private:
    QTimer t;
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
