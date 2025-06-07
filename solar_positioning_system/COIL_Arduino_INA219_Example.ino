//Example for using Arduino UNO R4 Wifi with a INA219 board
// M. Arndt, March 2025, TH Mittelhessen

/*
The following parameters have to be set:
ssid = "*****";
password = "*****";
mqtt server and topics
devicename
I/O Pins
Servo Pins
*/

//Libraries
#include <Wire.h>             // Wire Library
#include <Adafruit_INA219.h>  // Library for INA219 Board

// Initialization of Global Variables and Constants
int i=0;                // General counting variable
float shuntvoltage = 0; // INA219 output values
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float power_mW = 0;

// Object definitions
Adafruit_INA219 ina219;           // Create INA219 Object


// Setup Function --------------------------------------------------------
void setup() {
  Serial.begin(115200); //define the baudrate of the serial interface to the computer
  while (!Serial) {
  delay(100); // wait for serial port to connect. Needed for native USB port only
  }

  // Initialization the INA219 board
  if (! ina219.begin()) 
  {
    Serial.println("Failed to find INA219 chip. No Measurement possible!");
  }
  else 
  {
  // To use a lower 16V, 400mA range (higher precision on volts and amps):
    ina219.setCalibration_16V_400mA();
    Serial.println("Measuring voltage and current with INA219: Vmax=16V, Imax=400mA");
  }
  // Initialization of INA219 END
  
 }

// Main Program ----------------------------------------------------------
void loop() {

  // INA219 Test Sequence Start
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();

  loadvoltage = busvoltage + (shuntvoltage/1000);
  power_mW = loadvoltage*current_mA;
  
  //Print on Serial Monitor
  Serial.print("U_Bus= "); Serial.print(busvoltage); Serial.print(" V | ");
  Serial.print("U_Shunt= "); Serial.print(shuntvoltage); Serial.print(" mV | ");
  Serial.print("U_Load= "); Serial.print(loadvoltage); Serial.print(" V | ");
  Serial.print("I= "); Serial.print(current_mA); Serial.print(" mA | ");
  Serial.print("P= "); Serial.print(power_mW); Serial.print(" mW | ");
  Serial.println("");
  //Print on LCD Display
  // lcd.setCursor(0,0);lcd.print("Voltage=");lcd.print(loadvoltage);lcd.print(" V"); // Print Loadvoltage on LCD Display
  // lcd.setCursor(0,1);lcd.print("Current=");lcd.print(current_mA);lcd.print(" mA");  // Print Current on LCD Display
  // lcd.setCursor(0,2);lcd.print("Power=");lcd.print(power_mW);lcd.print(" mW");  // Print Current on LCD Display
  // delay(1000);
  // lcd.clear();
  // INA219 Test Sequence End 
 
}
