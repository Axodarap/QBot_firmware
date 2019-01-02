#pragma once

#include <Arduino.h>
#include "Stepper.h"

class Slider:public Stepper
{
public:
	Slider(int step_pin, int dir_pin, int num_steps, int speed, int max_angle);
	void set_direction(bool dir);

private:
	int max_angle_;	//defines the angle to move grippers away from cube

};