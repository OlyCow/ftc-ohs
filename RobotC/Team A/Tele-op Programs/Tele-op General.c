#pragma config(Hubs,  S1, HTMotor,  HTServo,  HTMotor,  none)
#pragma config(Sensor, S2,     touch,          sensorTouch)
#pragma config(Sensor, S3,     infrared_L,     sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S4,     infrared_R,     sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  motorA,          claw_L,        tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  motorB,          claw_R,        tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  motorC,          release_ramp,  tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     motor_L,       tmotorNormal, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     motor_R,       tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     motor_lift,    tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     motor_G,       tmotorNormal, openLoop)
#pragma config(Servo,  srvo_S1_C2_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// Code generated by 'ROBOTC' configuration wizard

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "typedefs.h"
#include "global vars.h"
#include "enums.h"
#include "structs.h"
#include "low-level functions.h"
#include "high-level functions.h"
#include "subroutines.h"


void initializeRobot()
{
	// Place code here to init servos to starting positions.
	// Sensors are auto-config'ed and setup by RobotC. They may need to stabilize.
	return;
}


task main()
{
	initializeRobot();
	waitForStart();   // Wait for start of tele-op phase.

	while (true)
	{
		Joystick_UpdateData();

		if ( Joystick_Button(BUTTON_LB)==1 || Joystick_Button(BUTTON_RB)==1 )
		{
			Motor_SetPower( motor_L, JoyStick_Joystick(JOYSTICK_L, AXIS_Y) / 4 );
			Motor_SetPower( motor_R, JoyStick_Joystick(JOYSTICK_R, AXIS_Y) / 4 );
		}
		else
		{
			Motor_SetPower( motor_L, JoyStick_Joystick(JOYSTICK_L, AXIS_Y) );
			Motor_SetPower( motor_R, JoyStick_Joystick(JOYSTICK_R, AXIS_Y) );
		}

		//if ( Joystick_Direction()==DIRECTION_F )
		//{
		//	Motor_SetPower( motor_L, 75 );
		//	Motor_SetPower( motor_R, 75 );
		//	Time_Wait( 100 );
		//	Motor_Stop( motor_L );
		//	Motor_Stop( motor_R );
		//}
	}
}
