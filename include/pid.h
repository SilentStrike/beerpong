#ifndef PID_H
#define PID_H

#include <QDebug>

class PID
{
	public:
		PID(void)
		{
			kP = 0;
			kI = 0;
			kD = 0;

            max_integral = 0;

			goal = 0;
			error_integral = 0;
			prev_error = 0;
		}

		~PID(void) {}

		void setP(double _kP)
		{
			kP = _kP;
		}

		void setI(double _kI)
		{
			kI = _kI;
		}

		void setD(double _kD)
		{
			kD = _kD;
		}

		void setmaxI(double _max_integral)
		{
			max_integral = _max_integral;
		}

		void reset(void)
		{
			goal = 0;
			error_integral = 0;
			prev_error = 0;
		}

		double update(double current_value)
		{
			double p, i, d;
			double error;
            double output;

            error =  goal - current_value;

			p = kP * error;

			error_integral += error;

            if(error_integral > max_integral)
                error_integral = max_integral;

            if(error_integral < 0)
                error_integral = 0;


			i = kI * error_integral;

			double derivative_error = error - prev_error;
			prev_error = error;

			d = kD * derivative_error;



            output = p + i + d;

            //qDebug() << "goal: " << goal << "error: " << error << "output: " << output;

            return output;
		}

		void set_goal(double new_goal)
		{
			goal = new_goal;
		}


		double get_goal(void)
		{
			return goal;
		}

	private:
		double kP;
		double kI;
		double kD;

		double max_integral;

		double goal;

		double error_integral;
		double prev_error;
};

#endif // PID_H
