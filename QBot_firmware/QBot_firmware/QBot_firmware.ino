/*
 Name:		QBot_firmware.ino
 Created:	12/31/2018 6:10:59 PM
 Author:	Jakob Soukup
*/
#include <Arduino.h>
#include "Slider.h"
#include "Gripper.h"
#include "Stepper.h"
#include "Solver.h"
#include "hardware_parameters.h"



Solver qbot;


void setup() 
{
	qbot.init(baudrate);
	//qbot.gripper_R_.set_turns(1,dir::ccw);
	//qbot.gripper_L_.set_turns(1,dir::ccw);
}


void loop() 
{
	//qbot.gripper_R_.update();
	//qbot.gripper_L_.update();

	qbot.turn_side(qbot.gripper_L_,dir::ccw,1);
	qbot.turn_side(qbot.gripper_R_,dir::ccw,1);
		
	
	
	/*
	if(qbot.turn_side(qbot.gripper_R_,dir::cw,1) && qbot.turn_side(qbot.gripper_L_,dir::cw,1))
		while(1)
		{
			Serial.println("out");
		}
	*/
	
	
	//while(!qbot.read_command());	//waiting for command to be received
	//while(!qbot.execute_comand());	//executing command 

//TODO  pull up to enbale pin ? disabling not happening immideatly

//TODO figure out the delay problem
//TODO when starting for the first time the motor draws 1 amp and the supply breaks down, after being turned once this goes down to normal 
	//kinda solved --> if arduino is plugged in after applying power everything is fine --> usb plug cut power rail KEEP GND
	
//TODO maybe implement a "disable_stepper" before read and an "enable steppers" afer the read

}
