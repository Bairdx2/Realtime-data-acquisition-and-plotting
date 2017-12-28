///////////////////////////////////////////////////////////////
// Script:   I2C_connection-logger.ino                        /
// Description: This file allows an arduino to receive        /
//              temperature, strain and resistance data from  /
//              sensors located on a separate arduino with    /
//              the sensors and ability to connect to the     /
//              channel the slave is recieving from. The      /
//              data is also saved on SD storage for later    /
//              manipulation.                                 /
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

//////////////////////////////////////Imports////////////////////////////////////////////

#include <SPI.h>
#include <SD.h>
#include <Wire.h>


//////////////////////////////////////Variables//////////////////////////////////////////

const int chipSelect = 10;                 // Pin used to connect to the SD card. 
char filename[] = "dat00.txt";             // Name of the data file. 
File dataFile;                             // File object created to save data.
char data[25];


//////////////////////////////////////Functions//////////////////////////////////////////

void setup() {
  
  Wire.begin(8);                           // join I2C bus as slave #8
  Serial.begin(9600);                      // Serial connection with baud rate of 9600


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

void loop() {

  File dataFile = SD.open(filename, FILE_WRITE);
  Wire.onReceive(request);                 // Method called in order to extract info sent on I2C

  if (dataFile){                           // Checks to see if the file was opened and writes
    dataFile.println(data);                // the combined data to it.
    dataFile.close();
  }
  else {}

  memset(data, 0, sizeof(data));           // Clearing the contents of the data array.
  delay(1000);
}

// Method that will be called by the slave in order to collect data from the master.
void request(int howMany) {
  int cp = 0;
  char w[2];
  while (cp < 22) {                        // loop through all but the last
    char c = Wire.read();                  // Extract on character at a time.
    if(cp <= 0){}
    else{
      w[0] = c;
      strcat(data,w);                      // Concatenation of char into the data array.
    }
   cp++;
   memset(w, 0, sizeof(w));                // Clearing the contents of the w char array
  }
  cp = 0;
}
