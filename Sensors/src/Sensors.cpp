/**
* Library to interpret input data to follow the line
* Author: Rosalind Ellis (github: rosalind210)
*/

#include "Sensors.h"

 /**
 * Constructor initializes serial for testing and sets value for pins
 */
Sensors::Sensors() 
{
 Serial.begin(9600);
}

void Sensors::getSensors()
{
 Sensors::getBin(Sensors::getValue());
 Serial.println(pinString);
}

 /**
  * Provided by Jonathan Maeda, gets the decimal value from the sensors
  */
int Sensors::getValue()
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
void Sensors::getBin(int dec) {
   String binary = String(dec, BIN); // string conversion provided by arduino
   int bin = 5; // length of pin bits
   int binaryLength = binary.length();
   // add zeros to front if necessary
   if (binaryLength < 6) {
    int numOfZeros = 6 - binaryLength;
    for (int i = 0; i <= numOfZeros; i++) {
      binary = "0" + binary;
    }
  }
  pinString = binary;
   // put into array
  while (bin > -1) {
   pins[bin] = binary.charAt(binaryLength - 1) - 48;
   binaryLength--;
   bin--;
 }
}
