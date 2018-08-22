///////////////////////////////////////////////////////////////
// Script:   Strain_dR-wireless.ino                           /
// Description: This arduino file takes the inputs from a	    /
//				load cell and transmits the data wirelessly         /
//				using a wireless transmitter of either 315MHz       /
//				or 434MHz to another arduino with a reciever        /
//				of the same frequency.                              /
//                                                            /
// Authors:  Kevin Rivera                                     /
// Creation Date:  12.02.17 v1.0                              /
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
//	Transmitter = 
//						   [	]
//							 ||||
//		5V-----------^
//		Gnd--------^
//		pin12-------^
//
//
//	Loadcell = follow the pins below.

//////////////////////////////////////Imports////////////////////////////////////////////

#include <VirtualWire.h>
#include "HX711.h"


//////////////////////////////////////Variables//////////////////////////////////////////

char lbs[20];                           // Weight data will be stored here.
char tot[20] = "";                      // Message buffer that will contain all of the weight data and it
                                        // should be kept below 30 byte in size for the wireless transmission.

const int CF = -7050.0;                 // Calibration value obtained for this load cell
const int DO = 3;                       // Pin connected to "data out" for the HX711 amplifier.
const int CLK = 2;                      // Pin connected to the "clock" for the HX711 amplifier.
HX711 loadcell(DO, CLK);                // Object created that will have the functions available 
                                        // from the HX711 library. 


//////////////////////////////////////Functions//////////////////////////////////////////

void setup() {
  
  vw_setup(2000);                          // Wireless communication bits per second.
  loadcell.set_scale(CF);                  // Setting the calibration factor of the load cell.
  loadcell.tare();                         // Resetting the load cell to a weight of 0.
}

void loop() {

  dtostrf(loadcell.get_units(), 6, 2, lbs);// Conversion of the load cell float data to a char string.
  send(lbs);                               // Wireless transmission of the weight data array.
  memset(lbs, 0, sizeof(lbs));             // Clearing the contents of the weight data array.
  delay(1000);                             // A second is waited before the loop is executed again.

}

// Method which takes in a char * and transmits the contents it points to.
void send(char *Wdata){
  vw_send((uint8_t *)Wdata, strlen(Wdata));// Function from virtual wire library that transmits a message.
  vw_wait_tx();                            // Waits until the whole message is sent.
}
