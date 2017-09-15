/**
* Header to interpret input data to follow the line
* Author: Rosalind Ellis (github: rosalind210)
*/

#ifndef Sensors_h
#define Sensors_h

#include "MeOrion.h"

class Sensors {

  public:
    uint8_t pins[6];
    String pinString; 

  private:
    const int DataPin = 12;
    uint8_t Sensor_Data[3];
    //Sensor Information
    getValue();
    getBin();
  
}
#endif


