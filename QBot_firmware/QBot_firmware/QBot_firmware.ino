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

//TODO when turning a side for an odd number of quarter turns it has to be turned back, should happen on the highest level possible. L1 -> L1,X,l1,x
//TODO pass command and indicator onto execute_command()

void setup() 
{
	qbot.init(baudrate);

}

void loop() 
{
	while(!qbot.communication_handle.read_command());
	while(!qbot.execute_comand(qbot.communication_handle.get_cmd(), qbot.communication_handle.get_indicator()));
}
