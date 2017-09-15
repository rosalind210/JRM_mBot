/**
* Header for motor control with maze solver
* Author: Rosalind Ellis (github: rosalind210)
*/

#ifndef FollowMotors_h
#define FollowMotors_h

#include "MeOrion.h"
#include "Sensors.h"

class FollowMotors
{
	public:
		int motorSpeed;
		int t;
		void startMotors();
	private: 
		void moveMotor(double x, double, y);
		void checkMovement();
		void checkComplex();
		void lineCorrector();
		void checkLeft();
		void checkRight();
		void inch();
} 

#endif