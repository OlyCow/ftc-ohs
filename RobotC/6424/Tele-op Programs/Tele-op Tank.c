#pragma config(Hubs,  S1, HTServo,  HTMotor,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     infrared,       sensorI2CCustom)
#pragma config(Sensor, S3,     color,          sensorCOLORFULL)
#pragma config(Sensor, S4,     ultrasonic,     sensorSONAR)
#pragma config(Motor,  motorA,          motor_ramp,    tmotorNXT, PIDControl)
#pragma config(Motor,  motorB,          motor_B,       tmotorNXT, openLoop)
#pragma config(Motor,  motorC,          motor_C,       tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     motor_L,       tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     motor_R,       tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     motor_lift,    tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     motor_G,       tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C1_1,    servo_IR,             tServoStandard)
#pragma config(Servo,  srvo_S1_C1_2,    servo_claw,           tServoStandard)
#pragma config(Servo,  srvo_S1_C1_3,    servo_ramp,           tServoStandard)
#pragma config(Servo,  srvo_S1_C1_4,    servo_D,              tServoStandard)
#pragma config(Servo,  srvo_S1_C1_5,    servo_E,              tServoStandard)
#pragma config(Servo,  srvo_S1_C1_6,    servo_F,              tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// Code generated by the 'ROBOTC' configuration wizard.
#include "JoystickDriver.c"
#include "IR-driver.h"
#include "typedefs.h"
#include "enums.h"
#include "structs.h"
#include "global vars.h"
#include "low-level functions.h"
#include "high-level functions.h"
#include "subroutines.h"



void initializeRobot()
{
	// Sensors are config'ed and setup by RobotC (need to stabalize).

	Servo_SetSpeed(servo_IR, 20);		// relatively high speed.
	Servo_SetSpeed(servo_claw, 20);		// relatively high speed.
	Servo_SetSpeed(servo_ramp, 0);	// ??? Might not work anymore.

	Servo_Rotate(servo_IR, g_IRServoLowered);		// it gets in the way
	Servo_Rotate(servo_claw, g_clawServoFolded);	// servo bracket gets bent
	Servo_Rotate(servo_ramp, g_rampServoDefault);	// stops ramp from deploying

	Motor_SetMaxSpeed(g_FullPower);

	Motor_ResetEncoder(motor_L);
	Motor_ResetEncoder(motor_R);
	Motor_ResetEncoder(motor_lift);

	bFloatDuringInactiveMotorPWM = false;

	return;
}



task main()
{
	// These will be used every loop, and are declared
	// here to save from declaring them every loop.
	int powerL = 0;
	int powerR = 0;
	int powerRamp = 0;
	//// Not implemented yet. Will implement when adding ring code.
	//MotorState isMotorStateL = MOTOR_JOYSTICK;
	//MotorState isMotorStateR = MOTOR_JOYSTICK;

	waitForStart();
	initializeRobot();



	// At "max capacity", each loop should do 8 checks and
	// 3 assignments: 2 x (2 joysticks + buttons + 1 D-pad).
	// The order is:
		// (D-pad)
		// Joysticks
		// Buttons

	// Default arguments are never passed if not needed (to optimize).

	// Using a `for` loop (instead of `while`) is more intuitive,
	// flexible, and makes code more readable (e.g. for indexing).

	for (int i=0; ; i++)	// `int i` is used in many included headers.
	{
		Joystick_UpdateData();

		// These should be zeroed after every loop, so if there
		// isn't any input, the motors will have no power.
		powerL = 0;
		powerR = 0;
		powerLift = 0;
		powerRamp = 0;



	// CONTROLLER 2 INPUT:==================================================||>

		// JOYSTICKS INPUT:--------------------------------------------------|>
		// Input from the two joysticks will control the lift at
		// different speeds (fast for R, slow for L). These will
		// not override the input from the primary driver.
			// This is because the input from the primary driver is
			// processed last, therefore getting "the last say".

		// The signals are only processed if it is above a threshold.
		if ( abs(Joystick_Joystick(JOYSTICK_L, AXIS_Y, CONTROLLER_2))
					>g_JoystickThreshold )
		{
			isLiftState = LIFT_JOYSTICK;
			powerLift =
				Math_ToLogarithmic
					(Joystick_Joystick(JOYSTICK_L, AXIS_Y, CONTROLLER_2));
		}
		if ( abs(Joystick_Joystick(JOYSTICK_R, AXIS_Y, CONTROLLER_2))
					>g_JoystickThreshold )
		{
			isLiftState = LIFT_JOYSTICK;
			powerLift =
				Math_ToLogarithmic
					(Joystick_Joystick(JOYSTICK_R, AXIS_Y, CONTROLLER_2)
						/g_SlowLiftFactor);
		}


		// BUTTONS INPUT:--------------------------------------------------|>
		// This includes servo controls and the ramp release confirm.

		// Uncomment the next line and comment the one after if masking.
			// For an explanation of masking the buttons' input,
			// see the comments accompanying CONTROLLER_1's buttons.
		//if ( (g_ControllerMaskB & joystick.joy2_Buttons) != false )
		if ( joystick.joy2_Buttons != false )
		{
			if ( Joystick_Button(BUTTON_LT, CONTROLLER_2)==true )
			{
				Servo_Rotate(servo_IR, g_IRServoLowered);
			}
			if ( Joystick_Button(BUTTON_LB, CONTROLLER_2)==true )
			{
				Servo_Rotate(servo_IR, g_IRServoExtended);
			}
			if ( Joystick_Button(BUTTON_RT, CONTROLLER_2)==true )
			{
				Servo_Rotate(servo_claw, g_clawServoFolded);
			}
			if ( Joystick_Button(BUTTON_RB, CONTROLLER_2)==true )
			{
				Servo_Rotate(servo_claw, g_clawServoExtended);
			}
			if ( Joystick_Button(BUTTON_START, CONTROLLER_2) && Joystick_Button(BUTTON_START, CONTROLLER_1) )
		    {
		    	powerRamp = 100;

		    }
			//// Both controllers need to press START to deploy ramp.
			//// The code is in CONTROLLER_1's buttons code block.
			//if ( Joystick_Button(BUTTON_BACK, CONTROLLER_2)==true )
			//{
			//	Servo_Rotate(servo_ramp, g_rampServoHold);
			//}
		}



	// CONTROLLER 1 INPUT:==================================================||>

		// D-PAD INPUT:--------------------------------------------------|>
		// Only if D-pad is pressed, test for direction.

		// Controls lift and has two unimplemented functions
		// for taking rings off/putting rings on.
		if ( Joystick_Direction() != DIRECTION_NONE )
		{
			switch ( Joystick_Direction() )
			{

				// Operate lift at full power if F/B.
				case DIRECTION_F:
					isLiftState = LIFT_JOYSTICK;
					powerLift = g_FullLiftPower/g_SlowLiftFactor;
					break;
				case DIRECTION_B:
					isLiftState = LIFT_JOYSTICK;
					powerLift = (-1)*g_FullLiftPower/g_SlowLiftFactor;
					break;

				case DIRECTION_L:

					break;
				case DIRECTION_R:

					break;
			}
		}


		// JOYSTICKS INPUT:--------------------------------------------------|>
		// Controls most of the driving. We are using two separate
		// checks, because combining them into one check will execute
		// both even if only one joystick is pressed. The linking of
		// both checks (disjunction) checks both inputs anyways.

		// The signals are only processed if it is above a threshold.
		if ( abs(Joystick_Joystick(JOYSTICK_L, AXIS_Y)) > g_JoystickThreshold )
		{
			powerL = Math_ToLogarithmic(Joystick_Joystick(JOYSTICK_L, AXIS_Y));
		}
		if ( abs(Joystick_Joystick(JOYSTICK_R, AXIS_Y)) > g_JoystickThreshold )
		{
			powerR = Math_ToLogarithmic(Joystick_Joystick(JOYSTICK_R, AXIS_Y));
		}


		// BUTTONS INPUT:--------------------------------------------------|>
		// Everything other than the buttons used might be masked to
		// (possibly) increase speed.
		// Reasoning: `&` compares every bit, so we might as well mask,
		// in case something irrelevant is pressed.

		// A `0` value means no buttons (that we test for) are pressed.
		// Directly using the struct since this is the only time we
		// use it, and the code is meant to be low-level anyways.

		// Uncomment the next line and comment the one after if masking.
			// For an explanation of masking the buttons' input,
			// see the comments accompanying CONTROLLER_1's buttons.
		//if ( (g_ControllerMaskA & joystick.joy1_Buttons) != false )
		if ( joystick.joy1_Buttons != false )
		{
			// Buttons Y/B/A/X will control lift height.
			if ( Joystick_Button(BUTTON_Y)==true )
			{
				isLiftState = LIFT_TOP;
			}
			if ( Joystick_Button(BUTTON_B)==true )
			{
				isLiftState = LIFT_MIDDLE;
			}
			if ( Joystick_Button(BUTTON_A)==true )
			{
				isLiftState = LIFT_BOTTOM;
			}
			if ( Joystick_Button(BUTTON_X)==true )
			{
				isLiftState = LIFT_FETCH;
			}

			// Buttons LT/RT fine-tune the lift.
			if ( Joystick_Button(BUTTON_RT)==true )
			{
				isLiftState = LIFT_JOYSTICK;
				powerLift = g_FullLiftPower;
			}
			if ( Joystick_Button(BUTTON_LT)==true )
			{
				isLiftState = LIFT_JOYSTICK;
				powerLift = (-1)*g_FullLiftPower;
			}

			// If LB/RB is pressed, fine-tune the motors.
			if ( (Joystick_Button(BUTTON_LB)||
					Joystick_Button(BUTTON_RB)) ==true )
			{
				powerL /= g_SlowDriveFactor;
				powerR /= g_SlowDriveFactor;
			}

			// Both controllers need to press START to deploy ramp.
			if ( (Joystick_Button(BUTTON_START)&&
					Joystick_Button(BUTTON_START, CONTROLLER_2))==true )
			{
				//Servo_Rotate(servo_ramp, g_rampServoDeployed);
				Motor_Target(motor_ramp, g_rampMotorRotation);
				Motor_SetPower(motor_ramp, 100);
				//// The above code should work. If the motor doesn't stop
				//// turning after the specified degrees/rotation (I don't
				//// know which), use the commented out code below. And
				//// don't forget to comment out the two lines above.
				//Motor_SetPower(motor_ramp, 100);
				//Time_Wait(g_rampMotorTime);
				//Motor_Stop(motor_ramp);
			}
			//if ( Joystick_Button(BUTTON_BACK)==true )
			//{
			//	Servo_Rotate(servo_ramp, g_rampServoHold);
			//}
		}



	// FINAL PROCESSING:==================================================||>

		// After preliminary processing of the controller data,
		// the actual motor/servo assignments happen here.

		switch (isLiftState)
		{
			case LIFT_BOTTOM:
				sub_LiftToHeight(g_BottomLiftAngle);
				break;
			case LIFT_MIDDLE:
				sub_LiftToHeight(g_MiddleLiftAngle);
				break;
			case LIFT_TOP:
				sub_LiftToHeight(g_TopLiftAngle);
				break;
			case LIFT_FETCH:
				sub_LiftToHeight(g_FetchLiftAngle);
		}

		Motor_SetPower(motor_L, powerL);
		Motor_SetPower(motor_R, powerR);
		Motor_SetPower(motor_lift, powerLift);
		Motor_SetPower(motor_ramp, powerRamp);
	}
}
