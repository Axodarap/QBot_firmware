#pragma once

#include "Arduino.h"
#include "Stepper.h"

class Gripper: public Stepper
{
public:
	Gripper(int step_pin, int dir_pin, int num_steps, int speed);
	void set_turns(int quarter_turns, bool dir);
};