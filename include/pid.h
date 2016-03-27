#ifndef PID_H
#define PID_H

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

		void set_parameters(double _kP, double _kI, double _kD, double _max_integral)
		{
			kP = _kP;
			kI = _kI;
			kD = _kD;

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

			error = current_value - goal;
			
			p = kP * error;

			error_integral += error;
			i = kI * error_integral;

			double derivative_error = error - prev_error;
			prev_error = error;

			d = kD * derivative_error;

			return p + i + d;
		}

		void set_goal(double new_goal)
		{
			goal = new_goal;
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
