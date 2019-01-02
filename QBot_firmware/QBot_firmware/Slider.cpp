#include "Slider.h"

Slider::Slider(int step_pin, int dir_pin, int num_steps, int speed, int max_angle):
	Stepper(step_pin, dir_pin, num_steps, speed),
	max_angle_{max_angle}
{
}

void Slider::set_direction(bool dir)
{
	set_angle(max_angle_, dir);
}
