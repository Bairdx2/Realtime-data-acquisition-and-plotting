///////////////////////////////////////////////////////////////
// Script:   I2C_connection.ino                               /
// Description: This file allows an arduino to receive 		    /
//				temperature, strain and resistance data from        /
//				sensors located on a separate arduino with          /
//				the sensors and ability to connect to the           /
//				channel the slave is recieving from.   	            / 		  
//                                                            /
// Authors:  Kevin Rivera                                     /
// Creation Date:  12.04.17 v1.0                              /
///////////////////////////////////////////////////////////////

//Circuit diagrams:
//  Thermistor = 
//
//          A0             A2
//      5V-----thermistor-----resistor-----Gnd
//    
//      notes: makes sure resistor has a low temperature
//             coefficient to ensure that resistance measurements
//             are accurate for the thermistor.
//
//
//  I2C connection = 
//
//
//    Arduino 1 pin A4----------------Arduino 2 pin A4
//    Arduino 1 pin A5----------------Arduino 2 pin A5
//    Arduino 1 Gnd-------------------Arduino 2 Gnd

//////////////////////////////////////Imports////////////////////////////////////////////

#include <Wire.h>


//////////////////////////////////////Functions//////////////////////////////////////////

void setup(){
  
  Wire.begin(8);                           // join I2C bus as slave #8
  Serial.begin(9600);                      // Serial connection with baud rate of 9600
}

void loop(){
  Wire.onReceive(request);                 // Method called in order to extract info sent on I2C
}

// Method that will be called by the slave in order to collect data from the master.
void request(int howMany) {
  while (1 < Wire.available()) {           // Checks to see if data is being sent.
    char c = Wire.read();                  // Extract on character at a time.
    Serial.print(c);                       // Print the character.
  }
  float x = Wire.read();                   // receive byte as an integer
  Serial.println();
}

