/**
* Methods controlling movement of motors with maze solvers
* Author(s):  Rosalind Ellis (github: rosalind210)
*             Jonas Tjahjadi (github: Jonarjr)
*/

#include "FollowMotors.h"

  /**
  * Constructor starts motors
  */
FollowMotors::FollowMotors(MeDCMotor motorA, MeDCMotor motorB) 
{
  motor1 = motorA;
  motor2 = motorB;
  motorSpeed = 60;
  t = 5;
  done = false;
}

void FollowMotors::moveMotor(double x, double y) {
  motor1.run(x * motorSpeed);
  motor2.run(y * motorSpeed);
  delay(t);
}

void FollowMotors::checkMovement(int pins[6]) 
{
    //check for T, intersect, or dead end
  if(pins[0], pins[1], pins[2], pins[3], pins[4], pins[5] == 0) {
    FollowMotors::checkComplex(pins);
    //check lost
  } else if (pins[0], pins[1], pins[2], pins[3], pins[4], pins[5] == 1) {
    FollowMotors::checkLost();
  }

  if(pins[0] == 0 && pins[1] == 0) {
    FollowMotors::checkLeft();
  } else if (pins[4] == 0 && pins[5] == 0) {
    FollowMotors::checkRight();
  } else {
    FollowMotors::lineCorrector(pins);
  }
}

  /**
  * Check if at T/Intersection/Done
  */
void FollowMotors::checkComplex(int pins[6])
{
  FollowMotors::inch();
    // has reached end of maze
  if (pins[0], pins[1], pins[2], pins[3], pins[4], pins[5] == 0) {
    t = 10;
    FollowMotors::moveMotor(1, 1);
    done = true;
    } else { // was at an intersection (want to turn left)
      FollowMotors::checkLost();
    }
  }

  /**
  * Moves forward a tiny bit
  */
  void FollowMotors::inch()
  {
    t = 2;
    FollowMotors::moveMotor(.75, -.75);
    t = 5;
  }

  /**
  * Turns left until it finds line or it stops
  */
  void FollowMotors::checkLost()
  {
    t = 10;
    FollowMotors::moveMotor(1, 1);
    t = 5;
  }

  /**
  * Handles turning left
  */
  void FollowMotors::checkLeft()
  {
    t = 50;
    FollowMotors::moveMotor(1, 1);
    t = 5;
    FollowMotors::inch();
  }

  /**
  * Handles turning right
  */
  void FollowMotors::checkRight()
  {
    t = 50;
    FollowMotors::moveMotor(-1, -1);
    t = 5;
    FollowMotors::inch();
  }

  /**
  * Handles going straight or getting back on the line
  */
  void FollowMotors::lineCorrector(int pins[6])
  {
    //correct left
    if(pins[0] == 0 || pins[1] == 0) {
      //far left first
      if(pins[0] == 0) {
        FollowMotors::moveMotor(-1.5, 1);
      } else if(pins[1] == 0){
        FollowMotors::moveMotor(-1.1, 1);
      }
    //correct right
    } else if(pins[4] == 0 || pins[5] == 0) {
      //far right first
      if(pins[5] == 0) {
        FollowMotors::moveMotor(-1, 1.5);
      } else if(pins[4] == 0) {
        FollowMotors::moveMotor(-1, 1.1);
      }
    //correct! straight!
    } else {
      FollowMotors::moveMotor(-1, 1);
    }
  }