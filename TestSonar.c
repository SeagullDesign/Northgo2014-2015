#pragma config(Hubs,  S1, MatrxRbtcs, none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CCustom)
#pragma config(Sensor, S3,     ,               sensorI2CCustom)
#pragma config(Sensor, S4,     ,               sensorI2CCustom)
#pragma config(Motor,  mtr_Matrix_S1_1, motor1,        tmotorMatrix, openLoop, encoder)
#pragma config(Motor,  mtr_Matrix_S1_2, motor2,        tmotorMatrix, openLoop, encoder)
#pragma config(Motor,  mtr_Matrix_S1_3, motor3,        tmotorMatrix, openLoop, encoder)
#pragma config(Motor,  mtr_Matrix_S1_4, motor4,        tmotorMatrix, openLoop, encoder)
#pragma config(Servo,  srvo_Matrix_S1_1, servo1,               tServoNone)
#pragma config(Servo,  srvo_Matrix_S1_2, servo2,               tServoNone)
#pragma config(Servo,  srvo_Matrix_S1_3, servo3,               tServoNone)
#pragma config(Servo,  srvo_Matrix_S1_4, servo4,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

///////////////////////////////////////////////////////////////////////////
// Teleop.c                                                              //
// Teleop program for Robot C using the OmniWheelDriver library          //
// Copyright (C) 2014  Arthur Admiraal                                   //
//                                                                       //
// This program is free software: you can redistribute it and/or modify  //
// it under the terms of the GNU General Public License as published by  //
// the Free Software Foundation, either version 3 of the License, or     //
// (at your option) any later version.                                   //
//                                                                       //
// This program is distributed in the hope that it will be useful,       //
// but WITHOUT ANY WARRANTY; without even the implied warranty of        //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         //
// GNU General Public License for more details.                          //
//                                                                       //
// You should have received a copy of the GNU General Public License     //
// along with this program.  If not, see <http://www.gnu.org/licenses/>. //
///////////////////////////////////////////////////////////////////////////

// include drivers
#include "JoystickDriver.c"        // Include file to handle the Bluetooth messages
#include "OmniWheelDriver.h"       // Include driver for the sensor multiplexer
#include "hitechnic-compass.h"     // Include compass sensor file
#include "hitechnic-irseeker-v2.h" // Include driver for the IR Seeker V2
#include "lego-light.h"            // Include driver for the light sensors
#include "lego-touch.h"            // Include driver for the touch sensors
#include "lego-ultrasound.h"       // Include driver for the ultrasound sensors

// define some constants
#define LIMIT 10

// define some variables
int minDist = 255;
int head    = 0;
int state   = 0;

void initializeRobot() {
	// put all the initialization code here
}

task main()
{
	initializeRobot();
	OWinitialize(S2, 45, 135, 225, 315);	// initialize the omniwheel driver with the compass sensor on sensor port 2
	eraseDisplay();

	ClearTimer(T1);

	while(true) {
		switch(state) {
			case 0: {
				OWsetRotationSpeed(16);
				OWupdate();

				int dist = USreadDist(S3);
				if(dist<minDist) {
					minDist = dist;
					head    = heading;
				}

				if(time1[T1]>4000) {
					OWsetRotationSpeed(0);
					OWupdate();
					state = 1;
				}
			} break;
			case 1: {
				OWsetTurnTarget(head+180);
				OWupdate();
			} break;
		}

		nxtSetPixel(50 + cosDegrees(360-heading)*32*USreadDist(S3)/50, 32 + sinDegrees(360-heading)*32*USreadDist(S3)/50);
	}
}
