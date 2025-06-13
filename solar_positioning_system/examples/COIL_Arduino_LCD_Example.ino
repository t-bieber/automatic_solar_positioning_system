//Example for using Arduino UNO R4 Wifi with a LCD Display
// M. Arndt, March 2025, TH Mittelhessen


//Libraries
#include <LiquidCrystal_I2C.h>   // Library for the LCD Display (optional)

// Initialization of Global Variables and Constants
int t=0; // Counting Variable

// Object definitions
LiquidCrystal_I2C lcd(0x27,20,4); // LCD Display 4x20, first parameter is the I2C adress

// Setup Function --------------------------------------------------------
void setup() {
  Serial.begin(115200); //define the baudrate of the serial interface to the computer
  while (!Serial) {
  delay(100); // wait for serial port to connect. Needed for native USB port only
  }
  
  // Initialize LCD Display 4x20
  lcd.init();
  lcd.begin(20,4);
  lcd.backlight();
  lcd.clear();
  // Initialization LCD Display END

}

// Main Program ----------------------------------------------------------
void loop() {

  //Print on LCD Display
  lcd.setCursor(0,0);lcd.print("Test Line 1"); // Print on LCD Display
  lcd.setCursor(0,1);lcd.print("Test Line 2");
  lcd.setCursor(0,2);lcd.print("Test Line 3");
  delay(1000);
  lcd.clear();
 
}
