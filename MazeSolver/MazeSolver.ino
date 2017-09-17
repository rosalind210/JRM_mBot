
#include "MeOrion.h"

// MOTOR STUFF
MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
int motorSpeed;
int t;
bool done;

// SENSOR STUFF
int pins[6];
uint8_t Sensor_Data[3];
const int DataPin = 12;

void setup() 
{
  motorSpeed = 60;
  t = 5;
  done = false;
}

void loop()
{
  getBin(getValue());
  if (!done) {
    checkMovement();
  }
  delay(20);
}

/**
 * SENSOR METHODS
 */
 
/**
 * Provided by Jonathan Maeda, gets the decimal value from the sensors
 */
int getValue()
{  
  long time_out_flag = 0;
  pinMode(DataPin, OUTPUT);
  digitalWrite(DataPin, LOW);
  delayMicroseconds(980);
  digitalWrite(DataPin, HIGH);
  delayMicroseconds(40);
  pinMode(DataPin, INPUT_PULLUP);
  delayMicroseconds(50); 
  time_out_flag = millis();
  while((digitalRead(DataPin) == 0)&&((millis() - time_out_flag) < 6)); 
  time_out_flag = millis();
  while((digitalRead(DataPin) == 1)&&((millis() - time_out_flag) < 6));
  for(uint8_t k=0; k<3; k++)
  {
   Sensor_Data[k] = 0x00;
   for(uint8_t i=0;i<8;i++)
   {
     time_out_flag = millis(); 
     while(digitalRead(DataPin) == 0&&((millis() - time_out_flag) < 6));
     uint32_t HIGH_level_read_time = micros();
     time_out_flag = millis(); 
     while(digitalRead(DataPin) == 1&&((millis() - time_out_flag) < 6));
     HIGH_level_read_time = micros() - HIGH_level_read_time;
     if(HIGH_level_read_time > 50 && HIGH_level_read_time < 100)  
     {
       Sensor_Data[k] |= (0x80 >> i);
     }
   }
  }
  if (Sensor_Data[1] == (uint8_t)(~(uint8_t)Sensor_Data[0]))
  {
  return Sensor_Data[0];
  }
}

/**
* Converts decimal from getValue to a binary array
*/
void getBin(int dec) {
  String binary = String(dec, BIN); // string conversion provided by arduino
  int bin = 5; // length of pin bits
  int binaryLength = binary.length();
  // add zeros to front if necessary
  if (binaryLength < 6) {
  int numOfZeros = 6 - binaryLength;
  for (int i = 0; i <= numOfZeros; i++) {
    binary = "0" + binary;
  }
  // put into array
  while (bin > -1) {
    pins[bin] = binary.charAt(binaryLength - 1) - 48;
    binaryLength--;
    bin--;
  }
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

void checkMovement() 
{
  //check for T, intersect, or dead end
  if(pins[0], pins[1], pins[2], pins[3], pins[4], pins[5] == 0) {
    checkComplex(pins);
  //check lost
  } else if (pins[0], pins[1], pins[2], pins[3], pins[4], pins[5] == 1) {
    checkLost();
  }

  if(pins[0] == 0 && pins[1] == 0) {
    checkLeft();
  } else if (pins[4] == 0 && pins[5] == 0) {
    checkRight();
  } else {
    lineCorrector();
  }
}

/**
* Check if at T/Intersection/Done
*/
void checkComplex(int pins[6])
{
  inch();
  // has reached end of maze
  if (pins[0], pins[1], pins[2], pins[3], pins[4], pins[5] == 0) {
    t = 10;
    moveMotor(1, 1);
    done = true;
   } else { // was at an intersection (want to turn left)
      checkLost();
   }
}

/**
* Moves forward a tiny bit
*/
void inch()
{
  t = 2;
  moveMotor(.75, -.75);
  t = 5;
}

/**
* Turns left until it finds line or it stops
*/
void checkLost()
{
  t = 10;
  moveMotor(1, 1);
  t = 5;
}

/**
* Handles turning left
*/
void checkLeft()
{
  t = 50;
  moveMotor(1, 1);
  t = 5;
  inch();
}

/**
* Handles turning right
*/
void checkRight()
{
  t = 50;
  moveMotor(-1, -1);
  t = 5;
  inch();
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
      moveMotor(-1.5, 1);
    } else if(pins[1] == 0){
      moveMotor(-1.1, 1);
    }
  //correct right
  } else if(pins[4] == 0 || pins[5] == 0) {
    //far right first
    if(pins[5] == 0) {
      moveMotor(-1, 1.5);
    } else if(pins[4] == 0) {
      moveMotor(-1, 1.1);
    }
  //correct! straight!
  } else {
    moveMotor(-1, 1);
  }
}

