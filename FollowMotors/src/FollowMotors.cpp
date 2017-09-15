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
      FollowMotors::checkLost();
    }
    
    if(pins[0] == 0 && pins[1] == 0) {
      FollowMotors::checkLeft();
    } else if (pins[4] == 0 && pins[5] == 0) {
      FollowMotors::checkRight();
    } else {
      FollowMotors::lineCorrector();
    }
  }

    void FollowMotors::checkComplex()
  {

  }

  void FollowMotors::checkLost()
  {
    t = 10;
    FollowMotors::moveMotor(1, 1);
  }

  void FollowMotors::checkLeft()
  {

  }

  void FollowMotors::checkRight()
  {

  }

  void FollowMotors::lineCorrector()
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
