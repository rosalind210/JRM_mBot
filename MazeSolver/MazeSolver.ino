
#include "MeOrion.h"
#include "LineArray.h"


// MOTOR STUFF
MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
int motorSpeed;
int t;
bool done;

// ROBOT STUFF - to deal with hardware differences between robots
int delaySpeedInchMme = 500; // currently both bender and Mme
int delaySpeedTurnMme = 500;
double lostSpeedMme = 1;
double lostSpeedBender = 1;
double turnSpeedMme = 1.4;
double turnSpeedBender = 1.1;
double turnSpeedSlowMme = 1.2;
double turnSpeedSlowBender = 1.1;
bool bender; // Bender is Jonas' robot

// SENSOR STUFF
LineArray lineArray;
uint8_t pins[6];
const int DataPin = 12;

void setup() 
{
  bender = true; // choose robot
  lineArray.setPort(DataPin);
  motorSpeed = 60;
  t = 1;
  done = false;
  delay(5000);
}

void loop()
{
  getBinArray(lineArray.getBINValue());
  printPins();
  if (!done) {
    checkMovement();
  } else {
    motor1.stop();
    motor2.stop();
  }
}

/**
* Print pins for debugging
*/
void printPins() {
  Serial.print(pins[0]);
  Serial.print(pins[1]);
  Serial.print(pins[2]);
  Serial.print(pins[3]);
  Serial.print(pins[4]);
  Serial.println(pins[5]);
}

/**
 * SENSOR METHOD
 */

/**
* Converts decimal from getValue to a binary array
*/
void getBinArray(String stringBin) {
  for (int i = 0; i < 6; i++) {
    pins[i] = stringBin.charAt(5 - i) - 48;
  }
}

/**
 * MOTOR METHODS
 */

/**
* Move motors for time t, affected by scalars
*/
void moveMotor(double x, double y) 
{
  motor1.run(x * motorSpeed);
  motor2.run(y * motorSpeed);
  delay(t);
}

/**
* Stop motors for time d
*/
void stopMotor(int d) {
  motor1.stop();
  motor2.stop();
  delay(d);
}

/**
* Moves forward a tiny bit
*/
void inch(int d)
{
  moveMotor(-1.5, 1.5);
  delay(d);
  stopMotor(1000);
}

/**
* Check line to choose direction
*/
void checkMovement() 
{
  //check for T, intersect, or dead end
  if(pins[0] == 0 && pins[1] == 0 && pins[2] == 0 && pins[3] == 0 && pins[4] == 0 && pins[5] == 0) {
    stopMotor(1000);
    checkComplex();
  //check lost
  } else if (pins[0] == 1 && pins[1] == 1 && pins[2] == 1 && pins[3] == 1 && pins[4] == 1 && pins[5] == 1) {
    checkLost();
  } else if(pins[0] == 0 && pins[1] == 0 && pins[2] == 0) {
    stopMotor(1000);
    checkLeft();
  } else if (pins[3] == 0 && pins[4] == 0 && pins[5] == 0) {
    stopMotor(1000);
    checkRight();
  } else {
    lineCorrector();
  }
}

/**
* Check if at T/Intersection/Done
*/
void checkComplex()
{
  if (bender) {
    inch(delaySpeedInchBender);
  } else {
    inch(delaySpeedInchMme);
  }
  
  getBinArray(lineArray.getBINValue());
  // has reached end of maze
  if (pins[0] == 0 && pins[1] == 0 && pins[2] == 0 && pins[3] == 0 && pins[4] == 0 && pins[5] == 0) {
    done = true;
   } else { // was at an intersection (want to turn left)
      checkLeft();
   }
}

/**
* Turns left until it finds line or it stops
*/
void checkLost()
{
  bool foundLine = false;
  t = 1;
  while (!foundLine) {
    getBinArray(lineArray.getBINValue());
    if (pins[0], pins[1], pins[2], pins[3], pins[4], pins[5] == 0) {
      stopMotor(1000);
      foundLine = true;
    }
    if (bender) { // bender variable check
      moveMotor(lostSpeedBender, lostSpeedBender);
    } else {
      moveMotor(lostSpeedMme, lostSpeedMme);
    }
  }
  t = 1;
  lineCorrector();
}

/**
* Handles turning left
*/
void checkLeft()
{
  if (bender) { // bender variable check
    inch(delaySpeedInchBender);
    t = delaySpeedTurnBender;
    moveMotor(turnSpeedBender, turnSpeedBender);
  } else {
    inch(delaySpeedInchMme);
    t = delaySpeedTurnMme;
    moveMotor(turnSpeedMme, turnSpeedMme);
  }
  t = 1;
}

/**
* Handles turning right
*/
void checkRight()
{
  if (bender) { // bender variable check
    inch(delaySpeedInchBender);
  } else {
    inch(delaySpeedInchMme);
  }
  stopMotor(50);
  getBinArray(lineArray.getBINValue());
  if (pins[0], pins[1], pins[2], pins[3], pins[4], pins[5] == 0) { // preference straight
    checkMovement();
  } else { // true right
    if (bender) { // bender variable check
      t = delaySpeedTurnBender;
      moveMotor(-turnSpeedBender, -turnSpeedBender);
    } else {
      t = delaySpeedTurnMme;
      moveMotor(-turnSpeedMme, -turnSpeedMme);
    }
    t = 1;
  } 
}

/**
* Handles going straight or getting back on the line
*/
void lineCorrector()
{
  //correct left
  if(pins[0] == 0 || pins[1] == 0) {
    //far left first
    if(pins[0] == 0) {
      t=3;
      moveMotor(-1, 1.5);
      t=1;
    } else if(pins[1] == 0){
      if (bender) { // bender variable check
        moveMotor(-1, turnSpeedSlowBender);
      } else {
        moveMotor(-1, turnSpeedSlowMme);
      }
    }
  //correct right
  } else if(pins[4] == 0 || pins[5] == 0) {
    //far right first
    if(pins[5] == 0) {
      t=3;
      moveMotor(-1.5, 1);
      t=1;
    } else if(pins[4] == 0) {
      if (bender) { // bender variable check
        moveMotor(-1, turnSpeedSlowBender);
      } else {
        moveMotor(-1, turnSpeedSlowMme);
      }
    }
  //correct! straight!
  } else {
    moveMotor(-1, 1);
  }
}


