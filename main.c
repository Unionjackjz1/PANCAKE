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

#pragma platform(VEX2)
#pragma competitionControl(Competition)
#include "Mod_Vex_Comp_Control.c"
#include "Lib.c"
#include "motor.h"
#include "cap.c"
#include "flywheel.c"
#include "DriveFunctions.c"
#include "Auton.c"

//Pre Auton and LCD Control
const int kMaxNumberOfPages = 5;
const int kMinNumberOfPages = 1;
int iAuton = 1;
string rgSAuton[kMaxNumberOfPages] = {"RED CAP", "RED FLAG", "BLUE CAP", "BLUE FLAG", "NONE"};
void
pre_auton() {
	flywheel_graph = false; //Make true if you want to graph target vs current velocity

	//Clearing some stuffz
	clear_encoder();
	clearDebugStream();
	datalogClear();

	//Drive PID Initialization
	pid_init(&l_drive, 0.1, 0.0001, 15);
	pid_threshold(&l_drive, 100);
	pid_i_threshold(&l_drive, 1000);

	pid_init(&r_drive, 0.1, 0.0001, 15);
	pid_threshold(&r_drive, 100);
	pid_i_threshold(&r_drive, 1000);

	//Arm PID Initialization
	pid_init(&arm_pid, 0.175, 0.0003, 16);
	pid_threshold(&arm_pid, 10);
	pid_i_threshold(&arm_pid, 200);

	//LCD Code, will only run when competition control is in DISABLED mode
	bLCDBacklight = true;
	while(!bIfiAutonomousMode && bIfiRobotDisabled) {
		if(nLCDButtons == 1) {
			iAuton = iAuton == kMinNumberOfPages ? kMaxNumberOfPages : iAuton - 1;
			wait_for_release();
		}
		if(nLCDButtons == 4) {
			iAuton = iAuton == kMaxNumberOfPages ? kMinNumberOfPages : iAuton + 1;
			wait_for_release();
		}
		if(nLCDButtons == 2) {
			wait_for_release();
			break;
		}

		displayLCDCenteredString(0, rgSAuton[iAuton - 1]);
		displayLCDCenteredString( 1, "<    Select    >");
		delay(50);
		clear_lcd_lines();
	}
	bLCDBacklight = false;
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
	while (true) {
		//Flywheel speeds
		if (vexRT[Btn7D]) {
			mid_flag();
		}
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

		//Run ball intake
		set_ball_intake((vexRT[Btn5U]-vexRT[Btn5D])*127);

		delay(20);
	}
}
