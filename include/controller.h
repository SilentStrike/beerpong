#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QThread>
#include <QTimer>

class Controller : public QThread
{
    Q_OBJECT
private:
    QTimer t;

signals:
    void ControllerFeedback(float trans, float speed);

protected:
    void run();

public:
    Controller(QObject *parent = 0);
    ~Controller();
};

#endif // CONTROLLER_H
