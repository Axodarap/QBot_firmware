#include "Solver.h"


Solver::Solver():
slider_Y_(sliderY_step, sliderY_dir, sliderY_en, steps_per_rot, slider_speed, max_angle),
slider_X_(sliderX_step, sliderX_dir, sliderX_en, steps_per_rot, slider_speed, max_angle),
gripper_L_(gripperL_step, gripperL_dir, gripperL_en, steps_per_rot, gripper_speed),
gripper_R_(gripperR_step, gripperR_dir, gripperR_en, steps_per_rot, gripper_speed),
gripper_F_(gripperF_step, gripperF_dir, gripperF_en, steps_per_rot, gripper_speed),
gripper_B_(gripperB_step, gripperB_dir, gripperB_en, steps_per_rot, gripper_speed),
command_{0},
turns_{0}
{
	Serial.begin(baudrate);
}

bool Solver::read_command()		//empties the serial buffer and stores the command in its approproiate variables, returns true once finished
{
	
	while(Serial.available())
	{
		char incoming_byte = Serial.read();

		if((48 <= static_cast<int>(incoming_byte)) && (static_cast<int>(incoming_byte) <= 57))  //if incoming byte is a digit
		{
			turns_ = char_to_int(incoming_byte);
		}
		else
			command_ = incoming_byte;

	}
	return true;
}

bool Solver::execute_comand()
{
	switch(command_)
	{
	case 'R':
		if(turn_R(dir::cw, turns_))
			return(true);
		else
			return(false);

	case 'r':
		if(turn_R(dir::ccw, turns_))
				return(true);
		else
			return(false);

	case 'L':
		if(turn_L(dir::cw, turns_))
			return(true);
		else
			return(false);
	
	case 'l':
		if(turn_L(dir::ccw, turns_))
			return(true);
		else
			return(false);

	case 'F':
		if(turn_F(dir::cw, turns_))
			return(true);
		else
			return(false);

	case 'f':
		if(turn_F(dir::ccw, turns_))
			return(true);
		else
			return(false);

	case 'B':
		if(turn_B(dir::cw, turns_))
			return(true);
		else
			return(false);
	
	case 'b':
		if(turn_B(dir::ccw, turns_))
			return(true);
		else
			return(false);
		
	case 'U':
		if(turn_U(dir::cw, turns_))
			return(true);
		else
			return(false);
		
	case 'u':
		if(turn_U(dir::ccw, turns_))
			return(true);
		else
			return(false);
		
	case 'D':
		if(turn_D(dir::cw, turns_))
			return(true);
		else
			return(false);
		
	case 'd':
		if(turn_D(dir::ccw, turns_))
			return(true);
		else
			return(false);
		
	default:
		return false;
	}
	
}

int Solver::char_to_int(char x)
{
	return static_cast<int>(x) - 48;
}

bool Solver::turn_R(dir dir, int turns)
{
	static bool busy = false;

	if(!busy)	//if this i the first call of the function
	{
		gripper_R_.set_turns(turns, dir);
		busy = true;
		return false;
	}
	else
	{
		if(gripper_R_.update())
		{
			busy = false;
			return true;	//done moving
		}
		else
			return false;	//still moving
	}
}
