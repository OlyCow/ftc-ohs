#pragma config(Hubs,  S1, HTServo,  HTMotor,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     infrared,       sensorI2CCustom)
#pragma config(Sensor, S3,     color,          sensorCOLORFULL)
#pragma config(Sensor, S4,     ultrasonic,     sensorSONAR)
#pragma config(Motor,  motorA,          motor_ramp,    tmotorNXT, openLoop)
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

	// The IR sensor needs this command (it seems like RobotC doesn't
	// initialize third-party sensors automatically.
	HTIRS2setDSPMode(infrared, g_IRsensorMode);

	Servo_SetSpeed(servo_IR, 30);		// maximum speed!
	Servo_SetSpeed(servo_claw, 20);		// maximum speed!
	Servo_SetSpeed(servo_ramp, 100);	// slowly update so ramp doesn't release.

	Servo_Rotate(servo_IR, g_IRServoExtended);		// will fold back up in tele-op
	Servo_Rotate(servo_claw, g_clawServoExtended);	// will be folded in tele-op
	Servo_Rotate(servo_ramp, g_rampServoDefault);	// stops ramp from deploying

	Motor_SetMaxSpeed(g_FullRegulatedPower);

	Motor_ResetEncoder(motor_L);
	Motor_ResetEncoder(motor_R);
	Motor_ResetEncoder(motor_lift);

	//// Wait this long so the claw & IR servos get to update.
	//// The ramp-release servo shouldn't move; the long update time
	//// is to prevent sudden jerks that might release the ramp.
	//// We don't need to wait for the IR sensor to stabalize since
	//// the robot doesn't read from it until it's at the first column,
	//// which should be ample time for RobotC to setup the sensor.
	//Time_Wait(10);

	return;
}



task main()
{
	// The IR signal strengh in all 5 directions.

	int IRdirA = 0;
	int IRdirB = 0;
	int IRdirC = 0;
	int IRdirD = 0;
	int IRdirE = 0;

	typedef enum PegIR
	{
		PEG_I = 0,
		PEG_II = 1,
		PEG_III = 2,
	};
	PegIR isPeg = PEG_II;

	waitForStart();
	initializeRobot();



	// For a better description of the below numbers,                   ________________________
	// see the page in Engineering Notebook describing                  |+----------------------                                _____   _____   _____   _____    _____		|
	// this program (it's labeled clearly).							    ||      Robot										    \ \  \  \ \     \ \      \ \      \ \		|
	const int forwardTimeA		= 140; //								||	      |												 \ \__\	 \_\__   \ \      \ \      \ \		|
	const int turnTimeA			= 40;  //                               ||        | Fwd A                            			  \ \  \    \ \   \ \      \ \      \ \		|
	const int forwardTimeB		= 140; //							    ||        | 											   \_\  \  __\_\   \_\___  _\_\__   _\_\ __	|
	const int backTimeA			= 80;  //                               ||   Turn A\																						|
                                       //								||	   	     \ Fwd B                                    								   								||      Back A \																			   //							    ||
									   //								||
	//Peg 1 Stuff
	const int turnTimeIA		= 90;
	const int forwardTimeIA		= 50;
	const int turnTimeIB		= 90;
	const int liftTimeIA		= 30;
	const int forwardTimeIB		= 50;
	const int liftTimeIB		= 30;
	//Peg 2 Stuff
	const int turnTimeIIA		= 90;
	const int forwardTimeIIA	= 50;
	const int turnTimeIIB		= 50;
	const int liftTimeIIA		= 60;
	const int forwardTimeIIB	= 50;
	const int liftTimeIIB		= 60;
	//Peg 3 Stuff
	const int turnTimeIIIA		= 90;
	const int forwardTimeIIIA	= 50;
	const int turnTimeIIIB		= 90;
	const int liftTimeIIIA		= 30;
	const int forwardTimeIIIB	= 50;
	const int liftTimeIIIB		= 30;



	Time_Wait(100);
	Servo_Rotate(servo_ramp, g_rampServoHold);
	Servo_Rotate(servo_claw, g_clawServoFolded);
	//Time_Wait(1000);

	Move_Forward	(forwardTimeA, g_AccurateMotorPower);
	Turn_Left 		(turnTimeA, g_AccurateMotorPower, g_AccurateMotorPower);
	Move_Forward	(forwardTimeB, g_AccurateMotorPower);

	Time_Wait(100);
	HTIRS2readAllACStrength(infrared, IRdirA, IRdirB, IRdirC, IRdirD, IRdirE);

	if ( (IRdirA+IRdirB) > g_IRthreshold )
		isPeg = PEG_III;
	if ( (IRdirE+IRdirD) > g_IRthreshold )
		isPeg = PEG_I;
	if ( IRdirC > (IRdirA+IRdirB+IRdirD+IRdirE) )
		isPeg = PEG_II;

	switch (isPeg)
	{
		case PEG_I:
			Move_Backward	(backTimeA, g_AccurateMotorPower);

			Turn_Right		(turnTimeIA, g_AccurateMotorPower, g_AccurateMotorPower);
			Move_Forward	(forwardTimeIA, g_AccurateMotorPower);
			Turn_Left		(turnTimeIB, g_AccurateMotorPower, g_AccurateMotorPower);

			Lift_Up			(liftTimeIA, g_AccurateMotorPower);
			Move_Forward	(forwardTimeIB, g_AccurateMotorPower);
			Lift_Down		(liftTimeIB, g_AccurateMotorPower);
			break;
		case PEG_II:
			Move_Backward	(backTimeA, g_AccurateMotorPower);

			Turn_Left		(turnTimeIIA, g_AccurateMotorPower, g_AccurateMotorPower);
			Move_Forward	(forwardTimeIIA, g_AccurateMotorPower);
			Turn_Right		(turnTimeIIB, g_AccurateMotorPower, g_AccurateMotorPower);

			Lift_Up			(liftTimeIIA, g_AccurateMotorPower);
			Move_Forward	(forwardTimeIIB, g_AccurateMotorPower);
			Lift_Down		(liftTimeIIB, g_AccurateMotorPower);
			break;
		case PEG_III:
			Move_Backward	(backTimeA, g_AccurateMotorPower);

			Turn_Left		(turnTimeIIIA, g_AccurateMotorPower, g_AccurateMotorPower);
			Move_Forward	(forwardTimeIIIA, g_AccurateMotorPower);
			Turn_Right		(turnTimeIIIB, g_AccurateMotorPower, g_AccurateMotorPower);

			Lift_Up			(liftTimeIIIA, g_AccurateMotorPower);
			Move_Forward	(forwardTimeIIIB, g_AccurateMotorPower);
			Lift_Down		(liftTimeIIIB, g_AccurateMotorPower);
			break;
	}

	while (true)
	{
		PlaySoundFile("moo.rso");
		while(bSoundActive == true)
		{
			Time_Wait(1);
		}
	}

}

























//task main()
//{
//	// The IR signal strengh in all 5 directions.
//	int IRdirA = 0;
//	int IRdirB = 0;
//	int IRdirC = 0;
//	int IRdirD = 0;
//	int IRdirE = 0;

//	waitForStart();
//	initializeRobot();

//	Time_Wait(100);
//	Servo_Rotate(servo_ramp, g_rampServoHold);
//	Servo_Rotate(servo_claw, g_clawServoFolded);
//	//Time_Wait(1000);



//	// The amount of time the robot...
//	const int forwardTimeAA	= 25;
//	const int turnTimeA 	= 45;

//	const int forwardTimeA 	= 155;
//	const int forwardTimeBA	= 21;
//	const int liftTimeB 	= 56;
//	const int turnTimeB 	= 94;
//	const int forwardTimeBB	= 90;

//	const int turnTimeCA	= 54;
//	const int forwardTimeCA	= 160;
//	const int turnTimeCB	= 48;
//	const int forwardTimeCB	= 30;
//	const int liftTimeD		= 143;
//	const int turnTimeD		= 105;
//	const int forwardTimeD	= 218;

//	const int forwardTimeEA	= 10;
//	const int turnTimeEA	= 40;
//	const int forwardTimeEB	= 100;
//	const int turnTimeEB	= 40;
//	const int liftTimeF		= 45;
//	const int turnTimeF		= 80;
//	const int forwardTimeF	= 100;

//	//^^^DONE----->
//	const int liftTimeG		= 30;	//TODO
//	const int backwardTimeG	= 100;	//TODO
//	const int turnTimeG		= 70;	//TODO
//	const int forwardTimeG	= 20;	//TODO

//	const int liftTimeH		= 50;	//TODO
//	const int backwardTimeH	= 90;	//TODO
//	const int turnTimeH		= 100;	//TODO
//	const int forwardTimeH	= 70;	//TODO

//	const int liftTimeI		= 30;	//TODO
//	const int backwardTimeI	= 130;	//TODO
//	const int turnTimeI		= 70;	//TODO
//	const int forwardTimeI	= 170;	//TODO

//	const int forwardTimeJ	= 50;	//TODO
//	const int turnTimeK		= 90;	//TODO
//	const int liftTimeK		= 30;	//TODO
//	const int forwardTimeK	= 50;	//TODO



//	Move_Forward	(forwardTimeAA, g_AccurateMotorPower);
//	Turn_Left		(turnTimeA, g_AccurateMotorPower, g_AccurateMotorPower);
//	Move_Forward	(forwardTimeA, g_AccurateMotorPower);

//	Time_Wait(100);
//	HTIRS2readAllDCStrength(infrared, IRdirA, IRdirB, IRdirC, IRdirD, IRdirE);

//	if ( (IRdirA+IRdirB+IRdirC+IRdirD+IRdirE) > g_IRthreshold )
//	{
//		Move_Forward	(forwardTimeBA, g_AccurateMotorPower);
//		Turn_Right		(turnTimeB, g_AccurateMotorPower, g_AccurateMotorPower);
//		Lift_Up			(liftTimeB, g_AccurateMotorPower);
//		Move_Forward	(forwardTimeBB, g_AccurateMotorPower);

//		Lift_Down		(liftTimeG, g_AccurateMotorPower);
//		//Move_Backward	(backwardTimeG, g_AccurateMotorPower);
//		//Turn_Right		(turnTimeG, g_AccurateMotorPower, g_AccurateMotorPower);

//		//Move_Forward	(forwardTimeG, g_AccurateMotorPower);
//	}
//	else
//	{
//		Turn_Left		(turnTimeCA, g_AccurateMotorPower, g_AccurateMotorPower);
//		Move_Forward	(forwardTimeCA, g_AccurateMotorPower);
//		Turn_Right		(turnTimeCB, g_AccurateMotorPower, g_AccurateMotorPower);

//		Time_Wait(100);
//		HTIRS2readAllACStrength(infrared, IRdirA, IRdirB, IRdirC, IRdirD, IRdirE);
//		Move_Forward	(forwardTimeCB, g_AccurateMotorPower);

//		if ( (IRdirA+IRdirB+IRdirC+IRdirD+IRdirE) > g_IRthreshold )
//		{
//			Turn_Right		(turnTimeD, g_AccurateMotorPower, g_AccurateMotorPower);
//			Lift_Up			(liftTimeD, g_AccurateMotorPower);
//			Move_Forward	(forwardTimeD, g_AccurateMotorPower);

//			Lift_Down		(liftTimeH, g_AccurateMotorPower);
//			//Move_Backward	(backwardTimeH, g_AccurateMotorPower);
//			//Turn_Right		(turnTimeH, g_AccurateMotorPower, g_AccurateMotorPower);

//			//Move_Forward	(forwardTimeH, g_AccurateMotorPower);
//		}
//		//else
//		//{
//		//	Move_Forward	(forwardTimeEA, g_AccurateMotorPower);
//		//	Turn_Right		(turnTimeEA, g_AccurateMotorPower, g_AccurateMotorPower);
//		//	Move_Forward	(forwardTimeEB, g_AccurateMotorPower);
//		//	//Turn_Left		(turnTimeEB, g_AccurateMotorPower, g_AccurateMotorPower);

//		//	Turn_Right		(turnTimeF, g_AccurateMotorPower, g_AccurateMotorPower);
//		//	Lift_Up			(liftTimeF, g_AccurateMotorPower);
//		//	Move_Forward	(forwardTimeF, g_AccurateMotorPower);

//		//	Lift_Down		(liftTimeI, g_AccurateMotorPower);
//		//	//Move_Backward	(backwardTimeI, g_AccurateMotorPower);
//		//	//Turn_Right		(turnTimeI, g_AccurateMotorPower, g_AccurateMotorPower);

//		//	//Move_Forward	(forwardTimeI, g_AccurateMotorPower);
//		//}
//	}
//	//Move_Forward	(forwardTimeJ, g_AccurateMotorPower);
//	//Turn_Right		(turnTimeK, g_AccurateMotorPower, g_AccurateMotorPower);
//	//Lift_Up			(liftTimeK, g_AccurateMotorPower);
//	//Move_Forward	(forwardTimeK, g_AccurateMotorPower);



//	while (true)
//	{
//		PlaySoundFile("moo.rso");
//		while(bSoundActive == true)
//		{
//			Time_Wait(1);
//		}
//	}
//}
