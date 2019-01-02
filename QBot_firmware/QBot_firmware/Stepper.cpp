#include "Stepper.h"
#include <Arduino.h>

Stepper::Stepper(int step_pin, int dir_pin, int speed, int steps_per_rotation):
	step_pin_{step_pin},
	dir_pin_{dir_pin}, 
	speed_{speed}, 
	steps_per_rotation_{steps_per_rotation},
	steps_to_go_{0},
	next_call_{0}
{
	pinMode(step_pin_, OUTPUT);
	pinMode(dir_pin_, OUTPUT);
	digitalWrite(step_pin_, LOW);
	digitalWrite(dir_pin_, LOW);
}


bool Stepper::update()
{
	int delay = 1000/speed_/2; //delay in ms in order to achieve pwm with 50% duty cycle and the desired steps/s
		
	if(!steps_to_go_) 
	{
		return true; //done moving
	}
	else
	{
		if(millis() >= next_call_)
		{
			digitalWrite(step_pin_, !digitalRead(step_pin_));
			next_call_ = millis() + delay;

			if(digitalRead(!step_pin_))
				steps_to_go_--;	//to avoid reducing the counter twice each step
		}
		return false;
	}
}

void Stepper::set_angle(int angle, bool dir)
{
	digitalWrite(dir_pin_, dir);			//setting direction
	steps_to_go_ = angle_to_steps(angle);	//calculating requiered #of steps to reach desired angle
}

void Stepper::set_speed(int speed)
{
	speed_ = speed;
}

int Stepper::angle_to_steps(int angle)
{
	return angle * steps_per_rotation_/360;		//returns # of steps
}
