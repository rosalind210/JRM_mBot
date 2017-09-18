
#include "MeOrion.h"
#include "LineArray.h"


// MOTOR STUFF
MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
int motorSpeed;
int t;
bool done;

// SENSOR STUFF
LineArray lineArray;
uint8_t pins[6];
const int DataPin = 12;

void setup() 
{
  Serial.begin(9600);
  lineArray.setPort(DataPin);
  motorSpeed = 60;
  t = 1;
  done = false;
  delay(5000);
  Serial.println("Starting...");
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
void moveMotor(double x, double y) 
{
  motor1.run(x * motorSpeed);
  motor2.run(y * motorSpeed);
  delay(t);
}

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
  inch(1000);
  getBinArray(lineArray.getBINValue());
  // has reached end of maze
  if (pins[0] == 0 && pins[1] == 0 && pins[2] == 0 && pins[3] == 0 && pins[4] == 0 && pins[5] == 0) {
    Serial.println("DOOOONE");
    done = true;
   } else { // was at an intersection (want to turn left)
      checkMovement();
   }
}

/**
* Turns left until it finds line or it stops
*/
void checkLost()
{
  t = 30;
  moveMotor(1, 1);
  t = 5;
}

/**
* Handles turning left
*/
void checkLeft()
{
  inch(500);
  t = 900;
  moveMotor(1.1, 1.1);
  t = 1;
}

/**
* Handles turning right
*/
void checkRight()
{
  Serial.println("IN RIGHT");
  inch(500);
  t = 1000;
  moveMotor(-1.1, -1.1);
  t = 1;
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
      t=5;
      moveMotor(-1.5, 1);
      t=1;
    } else if(pins[1] == 0){
      moveMotor(-1.1, 1);
    }
  //correct right
  } else if(pins[4] == 0 || pins[5] == 0) {
    //far right first
    if(pins[5] == 0) {
      t=5;
      moveMotor(-1, 1.5);
      t=1;
    } else if(pins[4] == 0) {
      moveMotor(-1, 1.1);
    }
  //correct! straight!
  } else {
    moveMotor(-1, 1);
  }
}


