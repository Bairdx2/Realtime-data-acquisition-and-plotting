///////////////////////////////////////////////////////////////
// Script:   Temp_dR-wireless.ino                             /
// Description: This arduino file takes the inputs from a	    /
//				thermocouple and thermistor and transmits           /
//				the data wirelessly using a wireless                /
//				transmitter of either 315MHz or 433MHz to           /
//				another arduino with a reciever of the same         /
//				frequency.                                          /
//                                                            /
// Authors:  Kevin Rivera                                     /
// Creation Date:  12.02.18 v1.0                              /
///////////////////////////////////////////////////////////////

//Circuit diagrams:
//	Thermistor = 
//
//          A0             A2
//      5V-----thermistor-----resistor-----ground
//    
//      notes: makes sure resistor has a low temperature
//             coefficient to ensure that resistance measurements
//             are accurate for the thermistor.
//
//
//  Transmitter = 
//               [  ]
//               ||||
//    5V-----------^
//    Gnd--------^
//    pin12-------^
//
//
//	Thermocouple = follow the pins below.


//////////////////////////////////////Imports////////////////////////////////////////////

#include <Math.h>
#include <SPI.h>
#include <VirtualWire.h>
#include "Adafruit_MAX31855.h"


//////////////////////////////////////Variables//////////////////////////////////////////

const int aPin0 = 0;                       // Analog Pin 0 will read voltage data of the thermistor.
float thermRes;                            // Thermistor resistance.
int data1;                                 // Data relayed from ADC pin A0 will be mapped here.
const float R2 = 92000;                    // R2 resistance.
float T;                                   // Temperature reported by the thermistor.
char TC[8];							                   // Thermocouple data will be stored here.
char TM[10];						                   // Thermistor data will be stored here.
char tot[20] = "";                         // Message buffer that will contain all of the temperature and dR data
                                           // and it should be kept below 30 byte in size for the wireless transmission.

const int DO = 4;                          // Pin connected to "data out" for the Max31855 amplifier.
const int CS = 5;                          // Pin connected to "chip select" for the Max31855 amplifier.
const int CLK = 6;                         // Pin connected to "clock" for the Max31855 amplifier.
Adafruit_MAX31855 typeK(CLK, CS, DO);      // Object created that will have the functions available 
                                           // from the Max31855 library.


//////////////////////////////////////Functions//////////////////////////////////////////

void setup(){
	
  vw_setup(2000);                          // Wireless communication bits per second.
} 

void loop(){

  data1 = analogRead(aPin0);               // Analog data from A0 is read.
  thermRes=R2*((1023.0 / data1) - 1.0);    // Thermistor resistance measured.
  dtostrf(thermRes, 6, 2, TM);             // Conversion of the thermistor float data to a char string.
  dtostrf(typeK.readCelsius(), 6, 2, TC);  // Conversion of the thermocouple float data to a char string.
  
  strcat(tot,TM);                          // Concatenation of thermistor data into the total data array.
  strcat(tot, ",");                        // Concatenation of a "," into the total data array.
  strcat(tot,TC);                          // Concatenationof the thermocouple data into the total data array.
  
  send(tot);                               // Wireless transmission of the total data array.
  
  memset(TM, 0, sizeof(TM));               // Clearing the contents of the thermistor data array.
  memset(TC, 0, sizeof(TC));               // Clearing the contents of the thermocouple data array.
  memset(tot, 0, sizeof(tot));             // Clearing the contents of the total data array.
  delay(1000);                             // A second is waited before the loop is executed again.
}

// Method which takes in a char * and transmits the contents it points to.
void send(char *Tdata){
  vw_send((uint8_t *)Tdata, strlen(Tdata));// Function from virtual wire library that transmits a message.
  vw_wait_tx();                            // Waits until the whole message is sent.
}

// Method that can convert the thermistor resistance to temperature directly.
// Use it with the thermRes data if needed.
float thermT(float a){
  T = 1.0 / (0.001129148 + (0.000234125 * log(a)) + (0.0000000876741 * log(a) * log(a) * log(a)));
  T = T - 273.15;                          // Conversion from Kelvin to Celsius
  return T;
}
