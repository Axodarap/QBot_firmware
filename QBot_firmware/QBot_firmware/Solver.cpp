#include "Solver.h"


Solver::Solver():
slider_Y_(sliderY_step, sliderY_dir, sliderY_en, steps_per_rot, slider_speed, max_angle),
slider_X_(sliderX_step, sliderX_dir, sliderX_en, steps_per_rot, slider_speed, max_angle),
gripper_L_(gripperL_step, gripperL_dir, gripperL_en, steps_per_rot, gripper_speed),
gripper_R_(gripperR_step, gripperR_dir, gripperR_en, steps_per_rot, gripper_speed),
gripper_F_(gripperF_step, gripperF_dir, gripperF_en, steps_per_rot, gripper_speed),
gripper_B_(gripperB_step, gripperB_dir, gripperB_en, steps_per_rot, gripper_speed),
command_{'0'}, 
indicator_{0},
moving_state_{state::unlock1}
{

}

void Solver::init(long baud)
{
	Serial.begin(baud);
	enable_steppers();
}

bool Solver::read_command()		//empties the serial buffer and stores the command in its approproiate variables, returns true once finished
{
	bool command_received = false;

	//TODO eventuell nur auf Reihenfolge gehen

	if(Serial.available() >= 2)		//waiting until the buffer has reached a size of 2 bytes --> maybe implement a timeout to regularly empty the buffer
	{
		while(Serial.available())
		{
			char incoming_byte = Serial.read();

			if((48 <= static_cast<int>(incoming_byte)) && (static_cast<int>(incoming_byte) <= 57))  //if incoming byte is a digit
			{
				indicator_ = char_to_int(incoming_byte);
			}
			else
				command_ = incoming_byte;

			command_received = true;	
		}
	}
	
	return command_received;
}

bool Solver::execute_comand()
{
	bool executed = false;
	unsigned long current_time = millis(); //die hier an turn_side() übergeben 
	
	switch(command_)
	{
	case 'R':
		executed = turn_side(gripper_R_,dir::cw,indicator_, current_time);
		break;

	case 'r':
		executed = turn_side(gripper_R_,dir::ccw,indicator_, current_time);
		break;

	case 'L':
		executed = turn_side(gripper_L_,dir::cw,indicator_, current_time);
		break;
	
	case 'l':
		executed = turn_side(gripper_L_,dir::ccw,indicator_, current_time);
		break;

	case 'F':
		executed = turn_side(gripper_F_,dir::cw,indicator_, current_time);
		break;

	case 'f':
		executed = turn_side(gripper_F_,dir::ccw,indicator_, current_time);
		break;

	case 'B':
		executed = turn_side(gripper_B_,dir::cw,indicator_, current_time);
		break;
	
	case 'b':
		executed = turn_side(gripper_B_,dir::ccw,indicator_, current_time);
		break;
		
	case 'U':
		executed = turn_top_bot(dir::cw, indicator_, cube_sides::U, current_time);
		break;
		
	case 'u':
		executed = turn_top_bot(dir::ccw, indicator_, cube_sides::U, current_time);
		break;
		
	case 'D':
		executed = turn_top_bot(dir::cw, indicator_, cube_sides::D, current_time);
		break;
		
	case 'd':
		executed = turn_top_bot(dir::ccw, indicator_, cube_sides::D, current_time);
		break;

	case 'X':
		executed = slide(slider_X_, dir::open, current_time);
		break;

	case 'x':
		executed = slide(slider_X_, dir::close, current_time);
		break;

	case 'Y':
		executed = slide(slider_Y_, dir::open, current_time);
		break;

	case 'y':
		executed = slide(slider_Y_, dir::close, current_time);
		break;

	case '>':
		executed = enable_steppers();
		break;

	case '<':
		executed = disable_steppers();
		break;

	case 'A':
		if(!adjust_cmd(dir::cw))
		{
			Serial.write('NUL');
			return true;
		}
		executed = true;
		break;

	case 'a':
		if(!adjust_cmd(dir::ccw))
		{
			Serial.write('NUL');
			return true;	//in order to avoid setting executed to true, but still signaling the end of the reading process
		}
		executed = true;	//correct command read
		break;

	//only testing
	case 'v':
		executed = turn_side(gripper_F_,dir::ccw,1, current_time) & turn_side(gripper_B_,dir::cw,1, current_time);
		break;
	//end of testing area

	default:
		Serial.write('NUL');
		return true; //finished executing command, even ignoring a wrong command is considered executing it
	}

	if(executed)	
	{
		Serial.write('ACK');	//sending ACK 
	}

	return executed;	//returns true if done moving
	
}



int Solver::char_to_int(char x)
{
	return static_cast<int>(x) - 48;
}

bool Solver::enable_steppers()
{
	gripper_L_.enable(true);
	slider_Y_.enable(true);
	gripper_R_.enable(true);
	gripper_F_.enable(true);
	gripper_B_.enable(true);
	slider_X_.enable(true);
	return true;
}

bool Solver::disable_steppers()
{
	gripper_L_.enable(false);
	slider_Y_.enable(false);
	gripper_R_.enable(false);
	gripper_F_.enable(false);
	gripper_B_.enable(false);
	slider_X_.enable(false);
	return true;
}

//--------------------------------------- actual moving functions -----------------------------------------------------------------------

bool Solver::turn_side(Gripper & gripper, dir dir, int turns, unsigned long time)
{

	if(!gripper.moving)	//if this is the first call of the function
	{
		gripper.set_turns(turns, dir);
		gripper.moving = true;
		return false;
	}
	else
	{
		if(gripper.update(time))
		{
			gripper.moving = false;
			return true;	//done moving
		}
		else
		{
			return false;	//still moving
		}
			
	}
}

bool Solver::slide(Slider & slider, dir dir, unsigned long time)
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
		if(slider.update(time))
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



bool Solver::turn_top_bot(dir direction, int turns, cube_sides side, unsigned long time)
{
	
	switch(moving_state_)
	{
		case state::unlock1:
			if(slide(slider_X_, dir::open, time))	//moving steppers away
				moving_state_ = state::flip1;
			break;
			
		case state::flip1:
			if(turn_side(gripper_F_,dir::ccw,1, time) & turn_side(gripper_B_,dir::cw,1, time))
				moving_state_ = state::lock1;
			break;

		case state::lock1:
			if(slide(slider_X_, dir::close, time))
				moving_state_ = state::unlock2;
			break;

		case state::unlock2:
			if(slide(slider_Y_, dir::open, time))
				moving_state_ = state::flip2;
			break;

		case state::flip2:
			if(turn_side(gripper_F_,dir::ccw,1, time) & turn_side(gripper_B_,dir::cw,1, time))
				moving_state_ = state::lock2;
			break;

		case state::lock2:
			if(slide(slider_Y_, dir::close, time))
				moving_state_ = state::turn;
			break;

		case state::turn:
			if(side == cube_sides::U)
			{
				if(turn_side(gripper_L_,direction,turns, time))
					moving_state_ = state::unlock3;
			}
			if(side == cube_sides::D)
			{
				if(turn_side(gripper_R_,direction,turns, time))
					moving_state_ = state::unlock3;
			}
			break;

		case state::unlock3:
			if(slide(slider_X_, dir::open, time))
					moving_state_ = state::flip3;
			break;

		case state::flip3:
			if(turn_side(gripper_F_,dir::cw,1, time) & turn_side(gripper_B_,dir::ccw,1, time))
				moving_state_ = state::turn_back;
			break;

		case state::turn_back:
			dir n_dir;					//reversed direction
			if(direction == dir::cw)	//switching direcition to turn gripper back into initial position
					n_dir = dir::ccw;
				else
					n_dir = dir::cw;

			if(side == cube_sides::U)
			{
				if(turn_side(gripper_L_,n_dir,turns, time))
					moving_state_ = state::lock3;
			}
			if(side == cube_sides::D)
			{
				if(turn_side(gripper_R_,n_dir,turns, time))
					moving_state_ = state::lock3;
			}
			break;

		case state::lock3:
			if(slide(slider_X_, dir::close, time))
				moving_state_ = state::unlock4;
			break;

		case state::unlock4:
			if(slide(slider_Y_, dir::open, time))
				moving_state_ = state::flip4;
			break;

		case state::flip4:
			if(turn_side(gripper_F_,dir::cw,1, time) & turn_side(gripper_B_,dir::ccw,1, time))
				moving_state_ = state::lock4;
			break;

		case state::lock4:
			if(slide(slider_Y_, dir::close, time))
			{
				moving_state_ = state::unlock1;
				return true;	//done moving
			}
			break;

	}
	return false;
}

bool Solver::adjust_cmd(dir dir)
{
	switch(indicator_)
	{
		case 1:
			slider_X_.adjust(dir);
			return true;

		case 2:
			slider_Y_.adjust(dir);
			return true;

		case 3:
			gripper_L_.adjust(dir);
			return true;

		case 4:
			gripper_R_.adjust(dir);
			return true;

		case 5:
			gripper_F_.adjust(dir);
			return true;

		case 6:
			gripper_B_.adjust(dir);
			return true;

		default:
		return false; //unknown command
	break;
	}
}

void Solver::clear_buffer()
{
	while(Serial.available() > 0) 
	{
		char t = Serial.read();
	}
}



