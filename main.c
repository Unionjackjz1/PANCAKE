#pragma config(Sensor, in1,    arm_sensor,     sensorPotentiometer)
#pragma config(Sensor, dgtl6,  left_drive_encoder, sensorQuadEncoder)
#pragma config(Sensor, dgtl8,  right_drive_encoder, sensorQuadEncoder)
#pragma config(Sensor, dgtl11, flywheel_encoder, sensorQuadEncoder)
#pragma config(Motor,  port1,           ball_intake,   tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           left_arm,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           left_back_drive, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           left_mid_drive, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           flywheel_1,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           flywheel_2,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           right_mid_drive, tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           right_back_drive, tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           right_arm,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          cap_intake,    tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
CONTROLS:
5D- Intake Without Indexer
5U- Intake With Indexer
6D- Lift Down
6U- Lift Up
7R- Flywheel Toggle
7L- Nothing
7D- Intake Out
7U- Flywheel High Speed
8R- Rescore Posts
8L- Nothing
8D- Descore Position
8U- Wrist Flip Toggle
*/

#define REINTAKE_CAP 620
#define DESCORE 1290
#define INDEXER  1870
#define ARM_DOWN 2310

#pragma platform(VEX2)
#pragma competitionControl(Competition)
#include "Mod_Vex_Comp_Control.c"
#include "Lib.c"
#include "motor.h"
#include "cap.c"
#include "flywheel.c"
#include "DriveFunctions.c"
#include "Auton.c"
#include "LCD.c"

void
pre_auton() {
	flywheel_graph = false; //Make true if you want to graph target vs current velocity
	bStopTasksBetweenModes = true; //Make this true if you want tasks to stop between modes
	bDisplayCompetitionStatusOnLcd = false; //Make this true if you want the gross default LCD stuff

	//Clearing some stuffz
	clear_encoder();
	clearDebugStream();
	datalogClear();

	//Drive PID Initialization
	pid_init(&l_drive, 0.3, 0.0001, 25);
	pid_threshold(&l_drive, 30);
	pid_i_threshold(&l_drive, 360);

	pid_init(&r_drive, 0.3, 0.0001, 25);
	pid_threshold(&r_drive, 30);
	pid_i_threshold(&r_drive, 360);

	//Arm PID Initialization
	//pid_init(&arm_pid, 0.175, 0.0003, 16);
	//pid_init(&arm_pid, 0.23, 0.0002, 12);
	pid_init(&arm_pid, 0.17, 0.00001, 13.5);
	pid_threshold(&arm_pid, 20);
	pid_i_threshold(&arm_pid, 100);
}

//Auton
task
autonomous() {
	clear_encoder();
	startTask(drive_pid_task);
	startTask(arm_pid_task);
	startTask(flywheel);
	startTask(flywheel_rpm_task);
	arm_pid.des = get_arm_sensor();

	//Run auton based off of LCD selection
	switch (iAuton) {
		//RED FLAG
	case 1:
		bFlip = false;
		intake_ball_auton();
		break;
		//RED CAP
	case 2:
		bFlip = false;
		flag_auton();
		break;
		//BLUE FLAG
	case 3:
		bFlip = true;
		intake_ball_auton();
		break;
		//BLUE CAP
	case 4:
		bFlip = true;
		flag_auton();
		break;
		//NONE
	case 5:
		break;
		//DEFAULT
	default:
		break;
	}

	stopTask(drive_pid_task);
	stopTask(arm_pid_task);
	stopTask(flywheel);
	allMotorsOff();
}

//User Control
task
usercontrol() {
	clear_encoder();
	startTask(wrist);
	startTask(lift);
	startTask(flywheel);
	startTask(flywheel_rpm_task);
	startTask(drive_control);
	startTask(ball_intake_task);
	while (true) {
		if (vexRT[Btn7U]) {
			high_flag();
		}

		//Toggle flywheel on/off
		if (vexRT[Btn7L]) {
			flywheel_toggle = !flywheel_toggle;
			while (vexRT[Btn7L]) {
				delay(1);
			}
		}

		delay(20);
	}
}
