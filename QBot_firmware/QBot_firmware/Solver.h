#pragma once
#include <Arduino.h>
#include "Slider.h"
#include "Gripper.h"
#include "enum.h"
#include "hardware_parameters.h"

class Solver
{
public:
	Solver();
	void init(long baud);	//starts the serial communication, has to be called within the setup() function, starts up motors
	bool read_command();
	bool execute_comand();
	int char_to_int(char x);

//private:
	bool turn_side(Gripper &gripper, dir dir, int turns);
	bool slide(Slider &slider, dir dir);		//mode being either open or 
	bool turn_top_bot(dir dir, int turns, cube_sides side);
	bool flip(Gripper &gripper1, Gripper &gripper2);	//flipping the cube
		
	Slider slider_Y_;
	Slider slider_X_;
	Gripper gripper_L_;
	Gripper gripper_R_;
	Gripper gripper_F_;
	Gripper gripper_B_;

	state moving_state_; 
	int turns_;		//turning commands are in the fromat |side| + |#quarter turns|, the #of quarter turns is stored in this int
	char command_;	//the side to be turned is stored in this char, the direction is given by the command being capitalised or not
					//  R --> R
					//  r --> R'
	
	
};