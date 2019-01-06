/*
 Name:		QBot_firmware.ino
 Created:	12/31/2018 6:10:59 PM
 Author:	Jakob Soukup
*/
#include <Arduino.h>
#include "Slider.h"
#include "Gripper.h"
#include "Stepper.h"
#include "hardware_parameters.h"



Gripper gripper1(gripperL_step, gripperL_dir, gripperL_en, steps_per_rot, gripper_speed);


void setup() 
{
	Serial.begin(115200);
	gripper1.enable();
	gripper1.set_turns(4, dir::cw);
}


void loop() 
{
	if(gripper1.update())
	{
		gripper1.set_turns(4,dir::cw);
		delay(1000);
	}	
}
