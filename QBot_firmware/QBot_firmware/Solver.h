#pragma once
#include <Arduino.h>
#include "Slider.h"
#include "Gripper.h"
#include "enum.h"

class Solver
{

public:
	Solver();

private:
	bool read_command();
	bool execute_comand();
	int char_to_int(char x);
	bool turn_R(dir dir, int turns);
	bool turn_L(dir dir, int turns);
	bool turn_F(dir dir, int turns);
	bool turn_B(dir dir, int turns);
	bool turn_U(dir dir, int turns);
	bool turn_D(dir dir, int turns);

	Slider slider_Y_;
	Slider slider_X_;
	Gripper gripper_L_;
	Gripper gripper_R_;
	Gripper gripper_F_;
	Gripper gripper_B_;

	int turns_;		//turning commands are in the fromat |side| + |#quarter turns|, the #of quarter turns is stored in this int
	char command_;	//the side to be turned is stored in this char, the direction is given by the command being capitalised or not
					//  R --> R
					//  r --> R'
	
	
};