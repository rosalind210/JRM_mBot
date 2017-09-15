/**
* Header to interpret input data to follow the line
* Author: Rosalind Ellis (github: rosalind210)
*/

#ifndef Sensors_h
#define Sensors_h

#include "MeOrion.h"

class Sensors {

  public:
    int pins[6];
    String pinString;
    void getSensors();
    Sensors();

  private:
    const int DataPin = 12;
    int Sensor_Data[3];
    //Sensor Information
    int getValue();
    void getBin(int dec);
  
};
#endif


