///////////////////////////////////////////////////////////////
// Script:   Temp_dR-I2C.ino                                  /
// Description: This arduino file takes the inputs from a	    /
//				thermocouple and thermistor and transmits           /
//				the data using an I2C connection to another         /     
//				arduino acting as a slave reciever.                 /
//                                                            /
// Authors:  Kevin Rivera                                     /
// Creation Date:  12.04.17 v1.0                              /
///////////////////////////////////////////////////////////////

//Circuit diagrams:
//	Thermistor = 
//
//          A0             A2
//      5V-----thermistor-----resistor-----Gnd
//    
//      notes: makes sure resistor has a low temperature
//             coefficient to ensure that resistance measurements
//             are accurate for the thermistor.
//
//
//	I2C connection = 
//
//
//		Arduino 1 pin A4----------------Arduino 2 pin A4
//		Arduino 1 pin A5----------------Arduino 2 pin A5
//		Arduino 1 Gnd-------------------Arduino 2 Gnd
//
//
//	Thermocouple = follow the pins below.


//////////////////////////////////////Imports////////////////////////////////////////////

#include <Math.h>
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_MAX31855.h"


//////////////////////////////////////Variables//////////////////////////////////////////


const int aPin0 = 0;                       // Analog Pin 0 will read voltage data.
float thermRes;                            // Thermistor resistance.
int data1;                                 // Data relayed from ADC pin A0 will be mapped here.
const float R2 = 92000;                    // R2 resistance.
float T;                                   // Temperature reported by the thermistor.
char TC[8];                                // Thermocouple data will be stored here.
char TM[10];                               // Thermistor data will be stored here.
char tot[20] = "";                         // Message buffer that will contains the temperature and thermistor data.

const int DO = 4;                          // Pin connected to "data out" for thermocouple amplifier.
const int CS = 5;                          // Pin connected to "chip select" for the thermocouple amplifier.
const int CLK = 6;                         // Pin connected to "clock" for the thermocouple amplifier.
Adafruit_MAX31855 typeK(CLK, CS, DO);      // Object created that will have the functions available 
                                           // from the Max31855 library.


//////////////////////////////////////Functions//////////////////////////////////////////

void setup(){
  
  Wire.begin();                            // Begin I2C connection
} 

void loop(){

  data1 = analogRead(aPin0);               // Analog data from A0 is read.
  thermRes=R2*((1023.0 / data1) - 1.0);    // Thermistor resistance measured.
  dtostrf(thermRes, 6, 2, TM);             // Conversion of the thermistor float data to a char string.
  dtostrf(typeK.readCelsius(), 6, 2, TC);  // Conversion of the thermocouple float data to a char string.

  strcat(tot, "a");                        // Concatenation of "a" into the total data array.
  strcat(tot,TM);                          // Concatenation of thermistor data into the total data array.
  strcat(tot, ",");                        // Concatenation of "," into the total data array.
  strcat(tot,TC);                          // Concatenation of thermocouple data into the total data array.

  Wire.beginTransmission(8);               // Begin I2C transmission to slave #8.
  Wire.write(tot, strlen(tot));            // I2C transmission of the total data array.
  Wire.endTransmission();                  // stop transmitting.

  memset(TM, 0, sizeof(TM));               // Clearing the contents of the thermistor data array.
  memset(TC, 0, sizeof(TC));               // Clearing the contents of the thermocouple data array.
  memset(tot, 0, sizeof(tot));             // Clearing the contents of the total data array.
  delay(1000);                             // A second is waited before the loop is executed again.

}

// Method that can convert the thermistor resistance to temperature directly.
// Use it with the thermRes data if needed.
float thermT(float a){
  T = 1.0 / (0.001129148 + (0.000234125 * log(a)) + (0.0000000876741 * log(a) * log(a) * log(a)));
  T = T - 273.15;                          // Conversion from Kelvin to Celsius
  return T;
}
