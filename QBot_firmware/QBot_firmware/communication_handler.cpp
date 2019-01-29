#include "communication_handler.h"

communication_handler::communication_handler():command_{'\n'}, indicator_{0}, buffer_{0}, starting_time_{millis()}
{
}

bool communication_handler::read_command()
{
	static constexpr int timeout = 1000;
	
	if(Serial.available() >= 2)		//if 2 bytes are stored in the rx buffer
	{
		for(int i=0; Serial.available(); i++)	//rx reading buffer into locla buffer array
		{
			buffer_[i] = Serial.read();
		}
		if(validate_cmd(buffer_[0], buffer_[1]))	//validating command received
		{
			command_ = buffer_[0];
			indicator_ = char_to_int(buffer_[1]);
			flush_buffer();
			return true;
		}
		else //invalid command
		{
			Serial.write('NUL');
			return false;
		}
			
	}
	else if(millis() - starting_time_ >= timeout)
	{
		flush_buffer();
		starting_time_ = millis();
		return false;	//no command received in time
	}

	return false;	//not enough bytes received yet
}

char communication_handler::get_cmd()
{
	return command_;
}

int communication_handler::get_indicator()
{
	return indicator_;
}

void communication_handler::init_communication(unsigned long baudrate)
{
	Serial.begin(baudrate);
}

void communication_handler::flush_buffer()
{
  while(Serial.available())
  {
  	char temp_buffer = Serial.read();
  }
}

bool communication_handler::validate_cmd(char command, int indicator)
{
	if(isAlpha(command) && isDigit(indicator))
		return true;
	else
		return false;
}

int communication_handler::char_to_int(char x)
{
	return static_cast<int>(x) - 48;
}