///////////////////////////////////////////////////////////////
// Script:   Temp-Strain-dR_logger_I2C.ino                    /
// Description: This arduino file takes the inputs from a     /
//        thermocouple, thermistor and loadcell and           /
//        transmits the data using I2C connection to another  /
//        arduino acting as a slave reciever. The data is     /
//        also stored in SD storage for later manipulation.   /
//                                                            /
//                                                            /
// Authors:  Kevin Rivera                                     /
// Creation Date:  12.05.18 v1.0                              /
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
//
//
//  Thermocouple = follow the pins below.
//  Loadcell = follow the pins below.

//////////////////////////////////////Imports////////////////////////////////////////////

#include <Math.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "Adafruit_MAX31855.h"
#include "HX711.h"


//////////////////////////////////////Variables//////////////////////////////////////////

const int aPin0 = 0;                       // Analog Pin 0 will read voltage data
float thermRes;                            // Thermistor resistance
int data1;                                 // Data relayed from ADC pin A0 will be mapped here
const float R2 = 92000;                    // R2 resistance
float T;                                   // Temperature reported by the thermistor
char TC[8];                                // Thermocouple data will be stored here.
char TM[10];                               // Thermistor data will be stored here.
char lbs[20];                              // Weight data will be stored here.
char tot[60];                              // All tempertaure, themistor and weight data will be stored here.

const int chipSelect = 10;                 // Pin used to connect to the SD card. 
char filename[] = "dat00.txt";             // Name of the data file. 
const int CF = -7050.0;                    // Calibration value obtained for this load cell
const int DO = 3;                          // Pin connected to "data out" for the HX711 amplifier.
const int CLK = 2;                         // Pin connected to the "clock" for the HX711 amplifier.
const int DO_ = 4;                         // Pin connected to "data out" for the Max31855 amplifier.
const int CS = 5;                          // Pin connected to "chip select" for the Max31855 amplifier.
const int CLK_ = 6;                        // Pin connected to "clock" for the Max31855 amplifier.
HX711 loadcell(DO, CLK);                   // Object created that will have the functions available                                          
Adafruit_MAX31855 typeK(CLK_, CS, DO_);    // from the HX711 and Max31855 libraries respectively.
File dataFile;                             // File object created to save data.                                                                     


//////////////////////////////////////Functions//////////////////////////////////////////

void setup(){
  
  Wire.begin();                            // Begin I2C connection.
  loadcell.set_scale(CF);                  // Setting the calibration factor of the load cell.
  loadcell.tare();                         // Resetting the load cell to a weight of 0.
  
  if (!SD.begin(chipSelect)) {             // Conditional to see if the SD card is connected.
    return;
  }
  for (uint8_t i = 0; i < 100; i++) {      // Loops and changes the last portion of the file until the name is unique.
    filename[3] = i/10 + '0';
    filename[4] = i%10 + '0';
    if (! SD.exists(filename)) {           // Makes a new file if the filename does not exist currently.
      dataFile = SD.open(filename, FILE_WRITE);
      break;
    }
  }
  
  dataFile.println("Weight(lbs),Resistance(Ohms),Temperature(K)");// Prints the column header for the .txt file
  dataFile.close();
}

void loop(){
	
  File dataFile = SD.open(filename, FILE_WRITE);
  data1 = analogRead(aPin0);               // Analog data from A0 is read.
  thermRes=R2*((1023.0 / data1) - 1.0);    // Thermistor resistance measured.
  dtostrf(thermRes, 6, 2, TM);             // Conversion of the thermistor float data to a char string.
  dtostrf(typeK.readCelsius(), 6, 2, TC);  // Conversion of the thermocouple float data to a char string.
  dtostrf(loadcell.get_units(), 6, 2, lbs);// Conversion of the load cell float data to a char string.

  strcat(tot,lbs);                         // Concatenation of weight data into the total data array.
  strcat(tot,",");                         // Concatenation of "," into the total data array.
  strcat(tot,TM);                          // Concatenation of thermistor data into the total data array.
  strcat(tot, ",");                        // Concatenation of "," into the total data array.
  strcat(tot,TC);                          // Concatenation of thermocouple data into the total data array.


  Wire.beginTransmission(8);               // Begin I2C transmission to slave #8.
  Wire.write(tot, strlen(tot));            // I2C transmission of the total data array.
  Wire.endTransmission();                  // stop transmitting.

  if (dataFile){                           // Checks to see if the file was opened and writes
    dataFile.println(tot);                 // the combined data to it.
    dataFile.close();
  }
  else{}
  
  memset(TM, 0, sizeof(TM));               // Clearing the contents of the thermistor data array.
  memset(TC, 0, sizeof(TC));               // Clearing the contents of the thermocouple data array.
  memset(lbs, 0, sizeof(lbs));             // Clearing the contents of the weight data array.
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
