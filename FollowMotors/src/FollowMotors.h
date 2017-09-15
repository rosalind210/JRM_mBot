/**
* Header for motor control with maze solver
* Author: Rosalind Ellis (github: rosalind210)
*/

#ifndef FollowMotors_h
#define FollowMotors_h

#include "MeOrion.h"

class FollowMotors
{
	public:
		MeDCMotor motor1;
  		MeDCMotor motor2;
		int motorSpeed;
		int t;
		void startMotors();
		bool done;
		void checkMovement(int pins[6]);
		FollowMotors(MeDCMotor motorA, MeDCMotor motorB);
	private: 
		void moveMotor(double x, double y);
		void checkComplex(int pins[6]);
		void lineCorrector(int pins[6]);
		void checkLeft();
		void checkRight();
		void checkLost();
		void inch();
};

#endif