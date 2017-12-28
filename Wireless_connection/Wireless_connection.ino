///////////////////////////////////////////////////////////////
// Script:   Wireless_connection.ino    					            /                                 
// Description: This file uses a 315MHz or 434MHz Wireless 	  /
//				reciever in order to get temperature, strain        /
//				and resistance data from sensors located on	        /
//				a separate arduino with the sensors and             /
//				wireless transmitter.   					                  /
//                                                            /
// Authors:  Kevin Rivera                                     /
// Creation Date:  12.02.18 v1.0                              /
///////////////////////////////////////////////////////////////

//Circuit diagram:
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
//	Receiver = 
//	
//				      ____________
//			       [	 _____	  ]
//				      ||||		||||
// Gnd----------^
// pin11---------^
// 5V--------------^
// 5V-------------------^
// Gnd-------------------^
// Gnd--------------------^

//////////////////////////////////////Imports////////////////////////////////////////////

#include <VirtualWire.h>


//////////////////////////////////////Variables//////////////////////////////////////////

byte message[VW_MAX_MESSAGE_LEN];          // a buffer to hold the incoming messages.
byte msgLength = VW_MAX_MESSAGE_LEN;       // the size of the message.


//////////////////////////////////////Functions//////////////////////////////////////////

void setup(){
  
  Serial.begin(9600);                      // Serial connection with baud rate of 9600.
  vw_setup(100);                          // Wireless communication bits per second.
  vw_rx_start();                           // Starts the receiver.
}

void loop(){
  if (vw_get_message(message, &msgLength)){ // Conditional to see if a message has been recieved
    for (int i = 0; i < msgLength; i++){    // Loop to extract the recieved message.
      Serial.write(message[i]);
    }
    Serial.println();
  }
}
