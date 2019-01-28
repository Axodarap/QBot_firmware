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

int count = 0;	//testing

void setup() 
{
	qbot.init(baudrate);
}

//TODO maybe create a communication class
//Problems: 
//		* need o read 2 a maximum of 2 bytes and guarantee that theses get read, I also have 1 byte commands tho
//		* when calling two gripper simotaniously they sometimes move more often then they should --> implement a stop in the turn side function?

//TODO Zeit von solver an Gripper und Slider übergeben und die übergeben dann an stepper
//aktuelle zeit beim aufruf einfach immer mitgeben!! #papa_rules

void loop() 
{
	count++;	

	Serial.println("\n");
	Serial.print("------ iteration nr: ");
	Serial.print(count);
	Serial.println(" ------");
	
	while(!qbot.read_command());	//waiting for command to be received
	while(!qbot.execute_comand());	//executing command 


}
