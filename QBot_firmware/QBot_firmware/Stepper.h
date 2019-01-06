#pragma once

class Stepper
{
public:
	Stepper(int step_pin, int dir_pin, int en_pin, int steps_per_rotation, int speed); 
	bool update();
	void enable();

protected:
	void set_angle(int angle, bool dir);
	void set_speed(int speed);
	
private:
	int step_pin_;
	int dir_pin_;
	int en_pin_;
	int speed_;					//steps/s
	int steps_per_rotation_;		
	int steps_to_go_;			//remaining # of steps
	unsigned long next_call_;	//storing the system time when the stepper should move again

	int angle_to_steps(int angle);

};