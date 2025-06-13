//Libraries
#include <Servo.h>            // Library for Servo Control
#include <Adafruit_INA219.h> // Library for INA219 Board
#include <Wire.h>             // Wire Library
#include <LiquidCrystal_I2C.h>   // Library for the LCD Display

// Initialization of Global Variables and Constants
int init_pos_horizontal = 90;
int init_pos_vertical = 0;

int t=0;

#define SERVOPIN_H 8 //Pin for the horizontal servo
#define SERVOPIN_V 9 //Pin for the vertical servo

float shuntvoltage = 0; // INA219 output values
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float power_mW = 0;

// Object definitions
Adafruit_INA219 ina219;
Servo servo_horizontal;     // Create a Servo object to control the horizontal servo motor
Servo servo_vertical;       // Create a Servo object to control the vertical servo motor
LiquidCrystal_I2C lcd(0x27,20,2); // LCD Display 4x20, first parameter is the I2C adress
String lcd_string1 = "";
String lcd_string2 = "";
// Setup Function --------------------------------------------------------
void setup() 
{
  Serial.begin(9600); //define the baudrate of the serial interface to the computer
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

  // Initialize LCD Display 4x20
  lcd.init();
  lcd.begin(20,2);
  lcd.backlight();
  lcd.clear();
  // Initialization LCD Display END

  lcd.setCursor(0,0);lcd.print("Test Line 1"); // Print on LCD Display
  lcd.setCursor(0,1);lcd.print("Test Line 2");
  delay(1000);
  lcd.clear();

  // Initialization of the servo motors
  servo_horizontal.attach(SERVOPIN_H); //attach the servo signal pin. Pin 9-13 can be used.
  servo_vertical.attach(SERVOPIN_V); //attach the servo signal pin. Pin 9-13 can be used.
  // Initialization of servos END

  move_horizontal(70);
  move_vertical(45);

  // servo_test();
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
  lcd_string1 = "Voltage = " + String(busvoltage) + "V";
  writeToLCD(lcd_string1);
}

void writeToLCD(String text) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(text);
}

void move_horizontal(int degrees) {
  servo_horizontal.write(degrees);
}

void move_vertical(int degrees) {
  servo_vertical.write(degrees);
}

void servo_test() {  
  int pos;
  Serial.println("Servo test routine");
  servo_vertical.write(0);
  delay(100);
  servo_horizontal.write(0);
  delay(100);

  for (pos=0;pos<=180;pos+=5)
  {
    Serial.println("Moving horizontally in 5 degree steps");
    servo_horizontal.write(pos); //increase the position from 0 to 180 degrees in single steps and write the positon to the servo motor
    delay(100); //wait for 10 milliseconds
  }
  delay(900);

  Serial.println("Moving horizontal 90");
  servo_horizontal.write(90); //set the position of the horizontal servo motor to 0 degrees
  delay(3000);

  for (pos=0;pos<=90;pos+=5)
  {
    Serial.println("Moving vertical in 5 degree steps");
    servo_vertical.write(pos); //increase the position from 0 to 180 degrees in single steps and write the positon to the servo motor
    delay(100);           //wait for 10 milliseconds
  }
  delay(900);


  Serial.println("Moving vertical 0");
  servo_vertical.write(0); //set the position of the horizontal servo motor to 0 degrees
  delay(100);
  servo_horizontal.write(0);
  delay(100);
  Serial.println("Servo test routine ended");
}
