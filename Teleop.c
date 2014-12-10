#pragma config(Hubs,  S1, MatrxRbtcs, none,     none,     none)
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
#include "FTCtools.h"              // Include some usefull scripts
#include "hitechnic-compass.h"     // Include compass sensor file
#include "hitechnic-irseeker-v2.h" // Include driver for the IR Seeker V2
#include "lego-light.h"            // Include driver for the light sensors
#include "lego-touch.h"            // Include driver for the touch sensors

// define some constants
#define LIMIT 5

// define some variables
int x1 = 0;
int y1 = 0;

void initializeRobot() {
	// put all the initialization code here
}

task main()
{
	initializeRobot();
	OWinitialize(S2, 45, 135, 225, 315);	// initialize the omniwheel driver with the compass sensor on sensor port 2
	waitForStart();     // wait for start of tele-op phase

	while(true) {
		getJoystickSettings(joystick);

		// apply a dead zone to the joystick data and scale the data
		x1 = -deadZonei(joystick.joy1_x1, LIMIT)*99/127;
		y1 = deadZonei(joystick.joy1_y1, LIMIT)*99/127;
		
		// apply logarithmic controls
		x1 = x1*x1*sgn(x1)/100;
		y1 = y1*y1*sgn(y1)/100;

		// set the drive direction, magnitude and turntarget
		OWsetDriveVec(x1, y1);
		OWsetRotationSpeed(pow(abs(deadZonei(joystick.joy1_x2, LIMIT)*100/127), 2)*sgn(joystick.joy1_x2)*ROTATION*(1+joy1Btn(8)*3)/40000);

		// update the omniwheels with the lates compass sensor readings
		OWupdate();
	}
}
