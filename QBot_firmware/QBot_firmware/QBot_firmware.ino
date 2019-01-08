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
	Serial.begin(115200);
	Serial.println(qbot.command_);
	Serial.println(qbot.turns_);

}


void loop() 
{

	while(!qbot.read_command());	//waiting for command to be received

	while(! qbot.execute_comand());	//executing command 



//TODO send ACK
//TODO figure out the delay problem
//TODO when starting for the first time the motor draws 1 amp and the supply breaks down, after being turned once this goes down to normal 
	//kinda solved --> if arduino is plugged in after applying power everything is fine --> usb plug cut power rail KEEP GND
	
//TODO maybe implement a "disable_stepper" before red and a "enable steppers" afer the read

}
