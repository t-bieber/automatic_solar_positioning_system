#include <Wire.h>
#include <Adafruit_INA219.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <WiFiS3.h>
#include <PubSubClient.h>

// Objets
Adafruit_INA219 ina219;
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servoH;
Servo servoV;
WiFiClient wifiClient;
PubSubClient client(wifiClient);

// Pins
#define SERVO_H_PIN 9
#define SERVO_V_PIN 10

// WiFi
const char* ssid = "test";
const char* password = "jadjadjad";
#define HOSTNAME "Arduino_UNO_R4"

// MQTT
const char* mqtt_server = "test.mosquitto.org";
const char* mqtt_topic_voltage = "cesithmcoil2025/equipe23/panel-voltage";
const char* mqtt_topic_current = "cesithmcoil2025/equipe23/panel-current";
const char* mqtt_topic_power = "cesithmcoil2025/equipe23/panel-power";
const char* mqtt_topic_servoH = "cesithmcoil2025/equipe23/servo/h";
const char* mqtt_topic_servoV = "cesithmcoil2025/equipe23/servo/v";
const char* mqtt_topic_mode = "cesithmcoil2025/equipe23/mode";
const char* mqtt_announce = "cesithmcoil2025/equipe23/announcement";

// Variables
int bestH = 0;
int bestV = 0;
int manualH = 0;
int manualV = 0;
float maxPower = 0;
bool modeAuto = true;

// Fonctions
// void connectWiFi() {
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(100);
//   }
//   WiFi.setHostname(HOSTNAME);
// }

// void connectMQTT() {
//   while (!client.connected()) {
//     if (client.connect(HOSTNAME)) {
//       client.publish(mqtt_announce, "Tracker connecté");
//       client.subscribe(mqtt_topic_mode);
//       client.subscribe(mqtt_topic_servoH);
//       client.subscribe(mqtt_topic_servoV);
//       // Publier l'état actuel du mode
//       client.publish(mqtt_topic_mode, modeAuto ? "auto" : "manuel");
//     } else {
//       delay(500);
//     }
//   }
// }

// void mqttCallback(char* topic, byte* payload, unsigned int length) {
//   String msg;
//   for (unsigned int i = 0; i < length; i++) {
//     msg += (char)payload[i];
//   }

//   if (String(topic) == mqtt_topic_mode) {
//     if (msg == "auto") {
//       modeAuto = true;
//       client.publish(mqtt_topic_mode, "auto");
//     }
//     if (msg == "manuel") {
//       modeAuto = false;
//       client.publish(mqtt_topic_mode, "manuel");
//     }
//   }

//   if (String(topic) == mqtt_topic_servoH) {
//     manualH = msg.toInt();
//     if (!modeAuto) {
//       servoH.write(manualH);
//     }
//   }

//   if (String(topic) == mqtt_topic_servoV) {
//     manualV = msg.toInt();
//     if (!modeAuto) {
//       servoV.write(manualV);
//     }
//   }
// }

// void publishMQTT(float voltage, float current, float power, int angleH, int angleV) {
//   char msg[10];

//   dtostrf(voltage, 5, 2, msg);
//   client.publish(mqtt_topic_voltage, msg);

//   dtostrf(current, 5, 2, msg);
//   client.publish(mqtt_topic_current, msg);

//   dtostrf(power, 5, 2, msg);
//   client.publish(mqtt_topic_power, msg);

//   itoa(angleH, msg, 10);
//   client.publish(mqtt_topic_servoH, msg);

//   itoa(angleV, msg, 10);
//   client.publish(mqtt_topic_servoV, msg);
// }

float readPower(float& voltage, float& current) {
  float bus = ina219.getBusVoltage_V();
  float shunt = ina219.getShuntVoltage_mV() / 1000.0;
  voltage = bus + shunt;
  current = ina219.getCurrent_mA();
  return voltage * current; // mW
}

void display(float voltage, float current, float power) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("U:");
  lcd.print(voltage, 1);
  lcd.print("V I:");
  lcd.print(current, 1);

  lcd.setCursor(0, 1);
  lcd.print("P:");
  lcd.print(power, 1);
  lcd.print("mW");
}

void searchBestPosition() {
  maxPower = 0;
  bestH = 0;
  bestV = 0;
  float voltage, current;

  for (int h = 0; h <= 180; h += 30) {
    servoH.write(h);
    delay(500);
    for (int v = 0; v <= 90; v += 30) {
      servoV.write(v);
      delay(500);
      float power = readPower(voltage, current);
      if (power > maxPower) {
        maxPower = power;
        bestH = h;
        bestV = v;
      }
    }
  }
}

// Setup
void setup() {
  Serial.begin(115200);
  ina219.begin();
  ina219.setCalibration_16V_400mA();
  lcd.init();
  lcd.backlight();

  servoH.attach(SERVO_H_PIN);
  servoV.attach(SERVO_V_PIN);

  //connectWiFi();
  // client.setServer(mqtt_server, 1883);
  // client.setCallback(mqttCallback);
  // connectMQTT();
}

// Loop
void loop() {
  // if (!client.connected()) {
  //   connectMQTT();
  // }
  // client.loop();

  if (modeAuto) {
    searchBestPosition();
    servoH.write(bestH);
    servoV.write(bestV);
    delay(2000);
  }

  float voltage, current;
  float power = readPower(voltage, current);

  int angleH = modeAuto ? bestH : manualH;
  int angleV = modeAuto ? bestV : manualV;

  display(voltage, current, power);
  //publishMQTT(voltage, current, power, angleH, angleV);

  Serial.print("Mode: ");
  Serial.print(modeAuto ? "AUTO" : "MANUEL");
  Serial.print(" H: ");
  Serial.print(angleH);
  Serial.print(" V: ");
  Serial.print(angleV);
  Serial.print(" Power: ");
  Serial.println(power);

  delay(1000);
}
