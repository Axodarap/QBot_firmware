#include "Gripper.h"

Gripper::Gripper(int step_pin, int dir_pin, int num_steps, int speed) : Stepper(step_pin, dir_pin, num_steps, speed)
{
}

void Gripper::set_turns(int quarter_turns, bool dir)
{
	set_angle(90*quarter_turns, dir);
}
