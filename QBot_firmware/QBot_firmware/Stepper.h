#pragma once

class Stepper
{
public:
	Stepper(int step_pin, int dir_pin, int num_steps, int speed);

protected:
	void rotate(int angle, bool dir);
	void set_speed(int speed);

private:
	int step_pin_;
	int dir_pin_;
	int speed_;					//steps/s
	int num_steps_;				//steps/rotation
	int steps_to_go_;			//remaining # of steps
	unsigned long last_call_;	//storing the system time of the last function call

	int angle_2_steps(int angle);

};