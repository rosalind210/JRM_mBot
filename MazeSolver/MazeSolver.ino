
#include "MeOrion.h"

// MOTOR STUFF
MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
int motorSpeed;
int t;
bool done;

// SENSOR STUFF
uint8_t pins[6];
uint8_t Sensor_Data[3];
const int DataPin = 12;

void setup() 
{
  Serial.begin(9600);
  motorSpeed = 60;
  t = 1;
  done = false;
  delay(5000);
  Serial.println("Starting...");
}

void loop()
{
  getBin(getValue());
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
 * SENSOR METHODS
 */
 
/**
 * Provided by Jonathan Maeda, gets the decimal value from the sensors
 */
uint8_t getValue()
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
void getBin(uint8_t dec) {
  String binary = String(dec, BIN);
  int bin = 0; // length of pin bits
  int binaryLength = binary.length();
  while (bin < 6) {
    if (binaryLength > 0) {
      pins[bin] = binary.charAt(binaryLength - 1) - 48;
      binaryLength--;
    } else {
      pins[bin] = 0;
    }
    bin++;
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
  if(pins[0] == 0 && pins[1] == 0 && pins[2] == 0 && pins[3] == 0 && pins[4] == 0 && pins[5] == 0) {
    stopMotor();
    checkComplex();
  //check lost
  } else if (pins[0] == 1 && pins[1] == 1 && pins[2] == 1 && pins[3] == 1 && pins[4] == 1 && pins[5] == 1) {
    checkLost();
  } else if(pins[0] == 0 && pins[1] == 0) {
    stopMotor();
    checkLeft();
  } else if (pins[4] == 0 && pins[5] == 0) {
    stopMotor();
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
  inch();
  // has reached end of maze
  if (pins[0] == 0 && pins[1] == 0 && pins[2] == 0 && pins[3] == 0 && pins[4] == 0 && pins[5] == 0) {
    done = true;
   } else { // was at an intersection (want to turn left)
      checkLost();
   }
}

void stopMotor() {
  motor1.stop();
  motor2.stop();
}


/**
* Moves forward a tiny bit
*/
void inch()
{
  moveMotor(-1, 1);
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
  t=1000;
  inch();
  stopMotor();
  t = 50;
  moveMotor(1.5, 1.5);
  t = 1;
  inch();
}

/**
* Handles turning right
*/
void checkRight()
{
  Serial.println("IN RIGHT");
  t=1000;
  inch();
  stopMotor();
  delay(1000);
  t = 50;
  moveMotor(-2, -2);
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
      stopMotor();
      t=5;
      moveMotor(-1, 1);
      t=1;
    } else if(pins[1] == 0){
      moveMotor(-1.1, 1);
    }
  //correct right
  } else if(pins[4] == 0 || pins[5] == 0) {
    //far right first
    if(pins[5] == 0) {
      stopMotor();
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


