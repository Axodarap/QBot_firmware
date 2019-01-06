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

//TODO establish communication -> read command, make sure you dont miss anything, send ACK
//TODO also make sure that the actual moving is done in a seperate loop, no time spent listening for commands while moving!

	/*
	 * if(qbot.slide(qbot.slider_X_,dir::close))
	 *	 while(1);
	 */


}
