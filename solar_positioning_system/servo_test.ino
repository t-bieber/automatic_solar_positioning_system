#include <WiFiS3.h>
#include <PubSubClient.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <LiquidCrystal_I2C.h>

Servo tiltServo;
Servo orientationServo;

const int TILT_PIN = 8;         // PWM pin for tilt servo
const int ORIENTATION_PIN = 9;  // PWM pin for orientation servo

void setup() {
  tiltServo.attach(TILT_PIN);
  orientationServo.attach(ORIENTATION_PIN);
}

void loop() {
  int orientationAngles[] = {0, 90, 180};
  int tiltAngles[] = {0, 45, 90};

  for (int i = 0; i < 3; i++) {           // Loop over orientation angles
    for (int j = 0; j < 3; j++) {         // Loop over tilt angles
      orientationServo.write(orientationAngles[i]);
      tiltServo.write(tiltAngles[j]);
      delay(5000); // Wait for 5 seconds
    }
  }

  while (true); // Stop the loop after one full pass
}
