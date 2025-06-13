//Example for using Arduino UNO R4 Wifi with with MQTT
// M. Arndt, March 2025, TH Mittelhessen

/*
The following parameters have to be set:
ssid = "*****";
password = "*****";
mqtt server and topics
devicename
*/

//Libraries
#include <WiFiS3.h>           // Library for Wifi with ESP32 module
#include <PubSubClient.h>     // Library for MQTT
#include <Wire.h>             // Wire Library

// Wifi Credentials
const char* ssid = "xxxx";                // Your Wi-Fi SSID
const char* password = "xxxx";            // Your Wi-Fi password

// MQTT Broker Details
const char* mqtt_server = "test.mosquitto.org";  // MQTT broker (you can replace it with your MQTT broker IP)
const char* mqtt_topic_gen = "cesithmcoil2025/test/#";                      // Testtopic for Listening
const char* mqtt_topic_h = "cesithmcoil2025/test/testclient/angle-h";       // Topic to control Servo 1
const char* mqtt_topic_v = "cesithmcoil2025/test/testclient/angle-v";       // Topic to control Servo 2
const char* mqtt_topic_panel_voltage = "cesithmcoil2025/test/panel-voltage";    // Topic to send values
const char* mqtt_topic_panel_current = "cesithmcoil2025/test/panel-current";    // Topic to send values
const char* mqtt_topic_panel_power = "cesithmcoil2025/test/panel-power";    // Topic to send values
const char* mqtt_announce = "cesithmcoil2025/announcement";                 // Topic to send Announcements


// Initialization of Global Variables and Constants
int pos=0;              // Initial servo position in degrees
int i=0;                // General counting variable
float shuntvoltage = 0.2; // INA219 output values
float busvoltage = 5.0;
float current_mA = 2.30;
float loadvoltage = 0;
float power_mW = 0;
char value[10]="";
char msg_out[20]="";
char msg_in[20]="";
int t=0;

#define HOSTNAME "Arduino_UNO_R4_Test" //Define the hostname of the device for Wifi

// Object definitions
WiFiClient wifiClient;            // Create Wifi Object
PubSubClient client(wifiClient);  // Create MQTT object

// Functions ---------------------------------------------------------------------


// Reconnect to MQTT broker if disconnected due to timeout
void reconnect() 
{
  while (!client.connected()) //Loop until reconnected
  {
    if (client.connect(HOSTNAME)) 
    {
      // Publish Announcement
      client.publish(mqtt_announce, HOSTNAME);
      // Publish Values
      dtostrf(loadvoltage,5,2,msg_out);
      client.publish(mqtt_topic_panel_voltage, msg_out);
      dtostrf(current_mA,5,2,msg_out);
      client.publish(mqtt_topic_panel_current, msg_out);
      dtostrf(power_mW,5,2,msg_out);
      client.publish(mqtt_topic_panel_power, msg_out);
      // resubscribe to the topics for the solar tracker
      client.subscribe(mqtt_topic_gen);  // Subscribe to the general topics
     } else {
      Serial.print("Failed");
      //Serial.print(client.state());
      Serial.println(" Trying again in 5 seconds...");
      delay(5000);
    }
  }
}

// MQTT Callback function to listen to messages from the broker
void mqttcallback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


// Setup Function --------------------------------------------------------
void setup() {
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
  // Initialization of Wifi End
    
  //Initialize MQTT
  client.setServer(mqtt_server, 1883);  // Set the MQTT server
  client.setCallback(mqttcallback);     // Set the callback function for incoming messages
  Serial.println("MQTT Connected!");
  delay(1000);
  // Initialization of MQTT END

}

// Main Program ----------------------------------------------------------
void loop() {
  
  //MQTT Test Sequence
    if (!client.connected()) // Reconnect to MQTT if the connection is lost
  {
    reconnect();
  }
  // Publish Values
      dtostrf(loadvoltage,5,2,msg_out);
      client.publish(mqtt_topic_panel_voltage, msg_out);
      dtostrf(current_mA,5,2,msg_out);
      client.publish(mqtt_topic_panel_current, msg_out);
      dtostrf(power_mW,5,2,msg_out);
      client.publish(mqtt_topic_panel_power, msg_out);
  client.loop();  // Listen for incoming MQTT messages
  //MQTT Test END

}
