#include "MeOrion.h"
#include <FollowMotors.h>
#include <Sensors.h>

  MeDCMotor motorA(M1);
  MeDCMotor motorB(M2);
  
  Sensors sensors;
  FollowMotors motors(motorA, motorB);
  
  void setup() 
  {
      Serial.begin(9600);
      delay(100);
      Serial.println("Start.....");
  }

  void loop()
  {
    sensors.getSensors();
    if (!motors.done) {
      motors.checkMovement(sensors.pins);
    }
  }

