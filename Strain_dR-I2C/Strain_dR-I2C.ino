///////////////////////////////////////////////////////////////
// Script:   Strain_dR-I2C.ino                                /
// Description: This arduino file takes the inputs from a	    /
//				load cell and transmits the data an I2C             /
//				connection to another arduino acting as a           /
//				slave reciever.                                     /
//                                                            /
// Authors:  Kevin Rivera                                     /
// Creation Date:  12.04.18 v1.0                              /
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
//	Loadcell = follow the pins below.

//////////////////////////////////////Imports////////////////////////////////////////////

#include <Wire.h>
#include "HX711.h"


//////////////////////////////////////Variables//////////////////////////////////////////

char lbs[20];                              // Weight data will be stored here.
char tot[20] = "";                         // Message buffer that will contain all of the weight data.

const int CF = -7050.0;                    // Calibration value obtained for this load cell
const int DO = 3;                          // Pin connected to "data out" for the HX711 amplifier.
const int CLK = 2;                         // Pin connected to the "clock" for the HX711 amplifier.
HX711 loadcell(DO, CLK);                   // Object created that will have the functions available 
                                           // from the HX711 library. 


//////////////////////////////////////Functions//////////////////////////////////////////

void setup() {
  
  Wire.begin();                            // Begin I2C connection
  loadcell.set_scale(CF);                  // Setting the calibration factor of the load cell.
  loadcell.tare();                         // Resetting the load cell to a weight of 0.

}

void loop() {

  dtostrf(loadcell.get_units(), 6, 2, lbs);// Conversion of the load cell float data to a char string.

  strcat(tot, "b");
  strcat(tot,lbs);
  
  Wire.beginTransmission(8);               // Begin I2C transmission to slave #8.
  Wire.write(tot, strlen(tot));            // I2C transmission of the total data array.
  Wire.endTransmission();                  // stop transmitting.
  
  memset(lbs, 0, sizeof(lbs));             // Clearing the contents of the weight data array.
  memset(tot, 0, sizeof(tot));             // Clearing the contents of the total data array.
  delay(1000);                             // A second is waited before the loop is executed again.

}
