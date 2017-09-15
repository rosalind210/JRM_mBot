/**
* Methods controlling movement of motors with maze solvers
* Author(s):  Rosalind Ellis (github: rosalind210)
*             Jonas Tjahjadi (github: Jonarjr)
*/

#include "FollowMotors.h"

class FollowMotors {

  /**
  * Constructor starts motors
  */
  FollowMotors::FollowMotors() {
    MeDCMotor motor1(M1);
    MeDCMotor motor2(M2);
    motorSpeed = 60;
    t = 5;
    done = false;
  }

  void FollowMotors::moveMotor(double x, double y) {
    motor1.run(x * motorSpeed);
    motor2.run(y * motorSpeed);
    delay(t);
  }

  void FollowMotors::checkMovement() 
  {
    //check for T, intersect, or dead end
    if(pins[0], pins[1], pins[2], pins[3], pins[4], pins[5] == 0) {
      FollowMotors::checkComplex();
    //check lost
    } else if (pins[0], pins[1], pins[2], pins[3], pins[4], pins[5] == 1) {
      FollowMotors::heckLost();
    }
    
    if(pins[0] == 0 && pins[1] == 0) {
      FollowMotors::checkLeft();
    } else if (pins[4] == 0 && pins[5] == 0) {
      FollowMotors::checkRight();
    } else {
      FollowMotors::lineCorrector();
    }
  }

  /**
  * Check if at T/Intersection/Done
  */
  void FollowMotors::checkComplex()
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

  void FollowMotors::checkLost()
  {

  }

  void FollowMotors::checkLeft()
  {

  }

  void FollowMotors::checkRight()
  {

  }

  void FollowMotors::lineCorrector()
  {
    
  }

}