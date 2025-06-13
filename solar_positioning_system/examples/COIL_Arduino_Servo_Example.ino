//Example for the use of servo motors with Arduino UNO R4 Wifi
// M. Arndt, March 2025, TH Mittelhessen

/*
The following parameters have to be set:
I/O Pins
Servo Pins
*/

//Libraries
#include <Servo.h>            // Library for Servo Control
#include <Wire.h>             // Wire Library

// Initialization of Global Variables and Constants
int pos=0;                    // Initial servo position in degrees
int t=0;

#define SERVOPIN_H 9 //Pin for the horizontal servo
#define SERVOPIN_V 10 //Pin for the vertical servo

// Object definitions
Servo myservo_v;                  // Create a Servo object to control the vertical servo motor
Servo myservo_h;                  // Create a Servo object to control the horizontal servo motor

// Setup Function --------------------------------------------------------
void setup() 
{
  Serial.begin(115200); //define the baudrate of the serial interface to the computer
  while (!Serial) {
  delay(100); // wait for serial port to connect. Needed for native USB port only
  }

  // Initialization of the servo motors
  myservo_h.attach(SERVOPIN_H); //attach the servo signal pin. Pin 9-13 can be used.
  myservo_v.attach(SERVOPIN_V); //attach the servo signal pin. Pin 9-13 can be used.
  // Initialization of servos END
}

// Main Program ----------------------------------------------------------
void loop() {

  //Servo Test Sequence Start
  if(t==0) //Sequenz wird nur einmal durchlaufen
  {
  //Horizontal servo first. 0..180 degrees are allowed to be used
  myservo_h.write(0); //set the position of the horizontal servo motor to 0 degrees
  delay(3000);
  myservo_h.write(90);
  delay(3000);
  myservo_h.write(180);
  delay(3000);
  
  for (pos=0;pos<=180;pos+=5)
  {
    myservo_h.write(pos); //increase the position from 0 to 180 degrees in single steps and write the positon to the servo motor
    delay(100); //wait for 10 milliseconds
  }
  
  myservo_h.write(0); //set the position of the horizontal servo motor to 0 degrees
  delay(3000);

  //Vertical servo second. 0..90 degrees are allowed to be used
  myservo_v.write(0); //set the position of the horizontal servo motor to 0 degrees
  delay(3000);
  myservo_v.write(90);
  delay(3000);
  
  for (pos=0;pos<=90;pos+=5)
  {
    myservo_v.write(pos); //increase the position from 0 to 180 degrees in single steps and write the positon to the servo motor
    delay(100);           //wait for 10 milliseconds
  }
  
  myservo_v.write(0); //set the position of the horizontal servo motor to 0 degrees
  delay(1000);
  // Servo Test Sequence End
  t=1;
}

}
