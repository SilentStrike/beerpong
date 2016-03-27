#include "controller.h"

Controller::Controller(QObject *parent) : QThread (parent)
{
    packetizer = new Packetizer("/dev/rfcomm0", 0x0A);
	rpm_factor = 256.0/15000000.0/16.0;
    emit ControllerFeedback(0, 0);

    start();
}

Controller::~Controller()
{
	uint8_t buf[5];

        buf[0] = 0;
        buf[1] = 0;
        packetizer->send(buf, 2);
}

void Controller::run()
{
	uint8_t buf[256];
	int size;
    double current_rpm;
    double pwm_value;
    double pid_output;

    while(1)
    {
        //get rpm from microcontroller
        size = packetizer->get(buf);
        current_rpm = (buf[1] << 8 | buf[2]);

        if(current_rpm != 0)
        {
            current_rpm = 1.0/ (current_rpm * rpm_factor);
        }

        emit ControllerFeedback(0, current_rpm);

        //update parameters
        //pid.set_goal(current_goal);

        pid_output = pid.update(current_rpm);

        pwm_value = pid_output;

        if(pwm_value > 100)
            pwm_value = 100;
        else if(pwm_value < 20)
            pwm_value = 20;

        if(pid.get_goal() == 0)
            pwm_value = 0;
        //pwm_value = pid.get_goal();

        //qDebug() << "goal: " << pid.get_goal() << "pwm: " << pwm_value << "pid: " << pid_output;
        //pwm_value = pid.get_goal();

        //send pwm value to microcontroller
        buf[0] = 0;
        buf[1] = (int) pwm_value;
        packetizer->send(buf, 2);

        //sleep?
        //might not need to, packetizer.get() blocks for new data
    }

}
