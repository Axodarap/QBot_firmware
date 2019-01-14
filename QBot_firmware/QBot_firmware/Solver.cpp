#include "Solver.h"


Solver::Solver():
slider_Y_(sliderY_step, sliderY_dir, sliderY_en, steps_per_rot, slider_speed, max_angle),
slider_X_(sliderX_step, sliderX_dir, sliderX_en, steps_per_rot, slider_speed, max_angle),
gripper_L_(gripperL_step, gripperL_dir, gripperL_en, steps_per_rot, gripper_speed),
gripper_R_(gripperR_step, gripperR_dir, gripperR_en, steps_per_rot, gripper_speed),
gripper_F_(gripperF_step, gripperF_dir, gripperF_en, steps_per_rot, gripper_speed),
gripper_B_(gripperB_step, gripperB_dir, gripperB_en, steps_per_rot, gripper_speed),
command_{'0'}, 
turns_{0},
moving_state_{state::unlock1}
{

}

void Solver::init(long baud)
{
	Serial.begin(baud);

	//enabling steppers
	gripper_L_.enable(true);
	slider_Y_.enable(true);
	gripper_R_.enable(true);
	gripper_F_.enable(true);
	gripper_B_.enable(true);
	slider_X_.enable(true);
	 
	
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

		delay(2);	//TODO find out why this delay is neede, if its not there the buffer remains full
	}
	return command_received;
}

bool Solver::execute_comand()
{
	bool executed = false;
	
	switch(command_)
	{
	case 'R':
		executed = turn_side(gripper_R_,dir::cw,turns_);
		break;

	case 'r':
		executed = turn_side(gripper_R_,dir::ccw,turns_);
		break;

	case 'L':
		executed = turn_side(gripper_L_,dir::cw,turns_);
		break;
	
	case 'l':
		executed = turn_side(gripper_L_,dir::ccw,turns_);
		break;

	case 'F':
		executed = turn_side(gripper_F_,dir::cw,turns_);
		break;

	case 'f':
		executed = turn_side(gripper_F_,dir::ccw,turns_);
		break;

	case 'B':
		executed = turn_side(gripper_B_,dir::cw,turns_);
		break;
	
	case 'b':
		executed = turn_side(gripper_B_,dir::ccw,turns_);
		break;
		
	case 'U':
		executed = turn_top_bot(dir::cw, turns_, cube_sides::U);
		break;
		
	case 'u':
		executed = turn_top_bot(dir::ccw, turns_, cube_sides::U);
		break;
		
	case 'D':
		executed = turn_top_bot(dir::cw, turns_, cube_sides::D);
		break;
		
	case 'd':
		executed = turn_top_bot(dir::ccw, turns_, cube_sides::D);
		break;

	case 'X':
		executed = slide(slider_X_, dir::open);
		break;

	case 'x':
		executed = slide(slider_X_, dir::close);
		break;

	case 'Y':
		executed = slide(slider_Y_, dir::open);
		break;

	case 'y':
		executed = slide(slider_Y_, dir::close);
		break;
		
	default:
		executed = false;
	}

	if(executed)	
	{
		Serial.write('ACK');	//sending ACK 
	}

	return executed;
	
}

int Solver::char_to_int(char x)
{
	return static_cast<int>(x) - 48;
}

//--------------------------------------- actual moving functions -----------------------------------------------------------------------

bool Solver::turn_side(Gripper & gripper, dir dir, int turns)
{
	static bool busy = false;

	if(!gripper.moving)	//if this is the first call of the function
	{
		gripper.set_turns(turns, dir);
		gripper.moving = true;
		return false;
	}
	else
	{
		if(gripper.update())
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



bool Solver::turn_top_bot(dir direction, int turns, cube_sides side)
{
	
	switch(moving_state_)
	{
		case state::unlock1:
			if(slide(slider_X_, dir::open))	//moving steppers away
				moving_state_ = state::flip1;
			break;
			
		case state::flip1:
		Serial.println("seocnd state");
			if(turn_side(gripper_F_,dir::ccw,1) && turn_side(gripper_B_,dir::cw,1))
				moving_state_ = state::lock1;
			break;

		case state::lock1:
			if(slide(slider_X_, dir::close))
				moving_state_ = state::unlock2;
			break;

		case state::unlock2:
			if(slide(slider_Y_, dir::open))
				moving_state_ = state::flip2;
			break;

		case state::flip2:
			if(turn_side(gripper_F_,dir::ccw,1) && turn_side(gripper_B_,dir::cw,1))
				moving_state_ = state::lock2;
			break;

		case state::lock2:
			if(slide(slider_Y_, dir::close))
				moving_state_ = state::turn;
			break;

		case state::turn:
			if(side == cube_sides::U)
			{
				if(turn_side(gripper_L_,direction,turns))
					moving_state_ = state::unlock3;
			}
			if(side == cube_sides::D)
			{
				if(turn_side(gripper_R_,direction,turns))
					moving_state_ = state::unlock3;
			}
			break;

		case state::unlock3:
			if(slide(slider_Y_, dir::open))
				moving_state_ = state::flip3;
			break;

		case state::flip3:
			if(turn_side(gripper_F_,dir::cw,1) && turn_side(gripper_B_,dir::ccw,1))
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
			if(turn_side(gripper_L_,n_dir,turns))
					moving_state_ = state::lock3;
			}
			if(side == cube_sides::D)
			{
				if(turn_side(gripper_R_,n_dir,turns))
					moving_state_ = state::lock3;
			}
			break;

		case state::lock3:
			if(slide(slider_X_, dir::close))
				moving_state_ = state::unlock4;
			break;

		case state::unlock4:
			if(slide(slider_Y_, dir::open))
				moving_state_ = state::flip4;
			break;

		case state::flip4:
			if(turn_side(gripper_F_,dir::cw,1) && turn_side(gripper_B_,dir::ccw,1))
				moving_state_ = state::lock4;
			break;

		case state::lock4:
			if(slide(slider_Y_, dir::close))
				moving_state_ = state::unlock1;
			return true;	//done moving
	}
	return false;
}

bool Solver::flip(Gripper & gripper1, Gripper & gripper2)
{
	return false;
}


