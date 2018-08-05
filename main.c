#pragma config(Sensor, in1,    arm_sensor,     sensorPotentiometer)
#pragma config(Sensor, dgtl2,  right_drive_encoder, sensorQuadEncoder)
#pragma config(Sensor, dgtl6,  left_drive_encoder, sensorQuadEncoder)
#pragma config(Sensor, dgtl8,  ultrasonic,     sensorSONAR_mm)
#pragma config(Sensor, dgtl11, flywheel_encoder, sensorQuadEncoder)
#pragma config(Motor,  port1,           ball_intake,   tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           left_arm,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           left_back_drive, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           left_mid_drive, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           flywheel_1,    tmotorVex393_MC29, openLoop, reversed)
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

void
pre_auton() {
	SensorValue[flywheel_encoder]    =
	SensorValue[left_drive_encoder]  =
	SensorValue[right_drive_encoder] = 0;
	clearDebugStream();
	datalogClear();

	pid_init(&arm_pid, 0.3, 0, 0);
}

task
autonomous() {
}

task
usercontrol() {
	startTask(wrist);
	startTask(lift);
	startTask(flywheel);
	startTask(flywheel_rpm_task);
	while (true) {
		set_tank(vexRT[Ch3], vexRT[Ch2]);

		set_ball_intake((vexRT[Btn5U]-vexRT[Btn5D])*127);

		delay(20);
	}
}
