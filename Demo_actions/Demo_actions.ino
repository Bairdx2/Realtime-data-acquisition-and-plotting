///////////////////////////////////////////////////////////////
// Script:   Strain_dR-I2C.ino                                /
// Description: This file is used to excite temperature and   /
//        and a strain sensor using a heating pad and         /
//        servo.                                              /
//                                                            /
// Authors:  Kevin Rivera                                     /
// Creation Date:  12.04.17 v1.0                              /
///////////////////////////////////////////////////////////////

//Circuit diagrams:
//  Heating pad = 
//
//                         padPin1
//                            |
//                            |
//      5-9V+ --------------Relay1-------------- Pad+  
//      5-9V- --------------Relay2-------------- Pad-
//                            |
//                            |
//                         padPin2
//
//
//  Servo = 
//
//      5V ---------------- Red
//      pin10 ------------- Yellow
//      Gnd --------------- Brown


//////////////////////////////////////Imports////////////////////////////////////////////

#include <Servo.h>


//////////////////////////////////////Variables//////////////////////////////////////////

int counter = 0;                            // Counter that will activate the conditional which turns the heating pad on or off.
const int padPin1 = 7;                      // Pin that will control the switch connected to the positive lead of the heating pad.
const int padPin0 = 6;                      // Pin that will control the switch connected to the negative lead of the heating pad.
bool padState = LOW;                        // Boolean to determine if the heating pad should be on or off.
Servo S1;                                   // Object created that will have the functions available
                                            // from the Servo library.

//////////////////////////////////////Functions//////////////////////////////////////////

void setup() {

  pinMode(padPin1,OUTPUT);                   // Initialize the padPin1 pin as a digital output.                     
  pinMode(padPin0,OUTPUT);                   // Initialize the padPin0 pin as a digital output.
  S1.attach(10);                             // The actions for the servo will come from pin 10.

}

void loop() {

  
  for (int i = 0; i < 180; i++) {            // This loop will move the servo down 45 degrees.
    S1.write(map(i, 180, 90, 110, 180)); 
    delay(20);
 }
  for (int i = 180; i != 0; i--) {           // This loop will move the servo up 45 degrees.
    S1.write(map(i, 180, 90, 110, 180));
    delay(20);
 }

  if(counter >= 1000){                       // Conditional which will control whether the heating pad is on or off.
    padState = !padState;
    digitalWrite(padPin1,padState);
    digitalWrite(padPin0,padState);
    counter = 0;
  }

 counter++;
}
