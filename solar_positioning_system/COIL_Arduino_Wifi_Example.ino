//Example for the use of WiFi with Arduino UNO R4 Wifi
// M. Arndt, March 2025, TH Mittelhessen

/*
The following parameters have to be set:
ssid = "*****";
password = "*****";
devicename
*/

//Libraries
#include <WiFiS3.h>               // Library for Wifi with ESP32 module

// Wifi Credentials
const char* ssid = "xxxx";        // Your Wi-Fi SSID
const char* password = "xxxx";    // Your Wi-Fi password


// Initialization of Global Variables and Constants
int i=0;                          // General counting variable
#define HOSTNAME "Arduino_UNO_R4" //Define the hostname of the device for Wifi

// Object definitions
WiFiClient wifiClient;            // Create Wifi Object

// Setup Function --------------------------------------------------------
void setup() 
{
  Serial.begin(115200); //define the baudrate of the serial interface to the computer
  while (!Serial) {
  delay(100); // wait for serial port to connect. Needed for native USB port only
  }

  //Initialize the Wifi Network
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("WiFi module not ready! Programm stopped.");
    while (true); // Do not continue!
  }

  // Set the hostname of your device
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(HOSTNAME); //define hostname

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(100);
  }
  Serial.println("WiFi Connected!");
 
}

// Main Program ----------------------------------------------------------
void loop() 
{

  Serial.println("WiFi connected - now it can be used.");
 
}
