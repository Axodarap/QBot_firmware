#include "Stepper.h"
#include <Arduino.h>

Stepper::Stepper(int step_pin, int dir_pin, int speed, int num_steps):
	step_pin_{step_pin},
	dir_pin_{dir_pin}, 
	speed_{speed}, 
	num_steps_{num_steps},
	steps_to_go_{0},
	last_call_{0}
{
	pinMode(step_pin_, OUTPUT);
	pinMode(dir_pin_, OUTPUT);
}


void Stepper::rotate(int angle, bool dir)
{
	int delay = 1000/speed_/2; //delay in ms in order to achieve pwm with 50% duty cycle and the desired steps/s
	digitalWrite(dir_pin_, dir);

	if(!steps_to_go_) 
	{
		return; //done moving
	}
	else
	{
		if(millis() - last_call_ >= delay)
		{
			digitalWrite(step_pin_, !digitalRead(step_pin_));
			last_call_ = millis();

			if(digitalRead(step_pin_))
				steps_to_go_--;	//to avoid reducing the counter twice each step
		}
	
	}
	
}

void Stepper::set_speed(int speed)
{
	speed_ = speed;
}
