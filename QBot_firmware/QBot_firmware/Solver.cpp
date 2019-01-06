#include "Solver.h"


Solver::Solver():
slider_Y_(sliderY_step, sliderY_dir, sliderY_en, steps_per_rot, slider_speed, max_angle),
slider_X_(sliderX_step, sliderX_dir, sliderX_en, steps_per_rot, slider_speed, max_angle),
gripper_L_(gripperL_step, gripperL_dir, gripperL_en, steps_per_rot, gripper_speed),
gripper_R_(gripperR_step, gripperR_dir, gripperR_en, steps_per_rot, gripper_speed),
gripper_F_(gripperF_step, gripperF_dir, gripperF_en, steps_per_rot, gripper_speed),
gripper_B_(gripperB_step, gripperB_dir, gripperB_en, steps_per_rot, gripper_speed),
command_{'F'}, //TODO reset to 0
turns_{4}	//TODO reset to 0, was just set to 4 for testing
{
	Serial.begin(baudrate);
}

bool Solver::read_command()		//empties the serial buffer and stores the command in its approproiate variables, returns true once finished
{
	bool command_received = false;
	
	while(Serial.available())
	{
		char incoming_byte = Serial.read();

		if((48 <= static_cast<int>(incoming_byte)) && (static_cast<int>(incoming_byte) <= 57))  //if incoming byte is a digit
		{
			turns_ = char_to_int(incoming_byte);
		}
		else
			command_ = incoming_byte;

		command_received = true;
	}
	return command_received;
}

bool Solver::execute_comand()
{
	switch(command_)
	{
	case 'R':
		if(turn_side(gripper_R_,dir::cw,turns_))
			return(true);
		else
			return(false);

	case 'r':
		if(turn_side(gripper_R_,dir::ccw,turns_))
				return(true);
		else
			return(false);

	case 'L':
		if(turn_side(gripper_L_,dir::cw,turns_))
			return(true);
		else
			return(false);
	
	case 'l':
		if(turn_side(gripper_L_,dir::ccw,turns_))
			return(true);
		else
			return(false);

	case 'F':
		if(turn_side(gripper_F_,dir::cw,turns_))
			return(true);
		else
			return(false);

	case 'f':
		if(turn_side(gripper_F_,dir::ccw,turns_))
			return(true);
		else
			return(false);

	case 'B':
		if(turn_side(gripper_B_,dir::cw,turns_))
			return(true);
		else
			return(false);
	
	case 'b':
		if(turn_side(gripper_B_,dir::ccw,turns_))
			return(true);
		else
			return(false);
		
	case 'U':
		if(1)	//TODO implement
			return(true);
		else
			return(false);
		
	case 'u':	//TODO implement
		if(1)
			return(true);
		else
			return(false);
		
	case 'D':	//TODO implement
		if(1)
			return(true);
		else
			return(false);
		
	case 'd':	//TODO implement
		if(1)
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

//--------------------------------------- actual moving functions -----------------------------------------------------------------------

bool Solver::turn_side(Gripper & gripper, dir dir, int turns)
{
	static bool busy = false;

	if(!busy)	//if this is the first call of the function
	{
		gripper.set_turns(turns, dir);
		busy = true;
		return false;
	}
	else
	{
		if(gripper.update())
		{
			busy = false;
			return true;	//done moving
		}
		else
		{
			return false;	//still moving
		}
			
	}
}

bool Solver::slide(Slider & slider, dir dir)
{
	static bool busy = false;

	if(!busy)	//if this is the first call of the function
	{
		slider.set_direction(dir);
		busy = true;
		return false;
	}
	else
	{
		if(slider.update())
		{
			busy = false;
			return true;	//done moving
		}
		else
		{
			return false;	//still moving
		}
	}
}


