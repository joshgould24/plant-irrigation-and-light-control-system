#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>

#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;    // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

// To connect with SSL/TLS:
// 1) Change WiFiClient to WiFiSSLClient.
// 2) Change port value from 1883 to 8883.
// 3) Change broker value to a server with a known SSL/TLS root certificate 
//    flashed in the WiFi module.

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "192.168.1.220";
int        port     = 1883;

//topics
const char moisture0_topic[]     = "sensor_data/moisture0";
const char moisture1_topic[]     = "sensor_data/moisture1";
const char moisture2_topic[]     = "sensor_data/moisture2";
const char moisture3_topic[]     = "sensor_data/moisture3";

const char humidity_topic[]     = "sensor_data/humidity";
const char temperature_topic[]  = "sensor_data/temperature";

//inputs
int moisture_ain0=A0;
int moisture_ain1=A1;
int moisture_ain2=A2;
int moisture_ain3=A3;
Adafruit_AM2320 humidity_and_temp_sensor = Adafruit_AM2320();

//values
int moisture0_value;
int moisture1_value;
int moisture2_value;
int moisture3_value;
int humidity_value;
int temperature_value;

const long interval = 15000;//number of milliseconds between each iteration of loop
unsigned long previousMillis = 0;


void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  // You can provide a unique client ID, if not set the library uses Arduino-millis()
  // Each client must have a unique client ID
  // mqttClient.setId("clientId");

  // You can provide a username and password for authentication
  // mqttClient.setUsernamePassword("username", "password");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  //setting up moisture sensor
  // pinMode(moisture_ain0,INPUT);---------------CHANGED SOMETHING

  //setting up humidity and temp sensor
  humidity_and_temp_sensor.begin();
}

void(* resetFunc) (void) = 0;

void loop() {
  // call poll() regularly to allow the library to send MQTT keep alives which
  // avoids being disconnected by the broker
  mqttClient.poll();

  // to avoid having delays in loop, we'll use the strategy from BlinkWithoutDelay
  // see: File -> Examples -> 02.Digital -> BlinkWithoutDelay for more info
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    
    if (!mqttClient.connect(broker, port)) {
      Serial.print("MQTT connection failed! Error code = ");
      Serial.println(mqttClient.connectError());

      resetFunc();
    }

    Serial.println("You're connected to the MQTT broker!");
    Serial.println(millis());
    Serial.println();

    // save the last time a message was sent
    previousMillis = currentMillis;

    //read moisture data
    moisture0_value    = analogRead(moisture_ain0);
    moisture1_value    = analogRead(moisture_ain1);
    moisture2_value    = analogRead(moisture_ain2);
    moisture3_value    = analogRead(moisture_ain3);
    
    humidity_value    = humidity_and_temp_sensor.readHumidity();

    temperature_value = humidity_and_temp_sensor.readTemperature();


    //moisture
    // Serial.print("Sending message to topic: ");
    // Serial.println(moisture0_topic);
    Serial.print("moisture 0: ");
    Serial.println(moisture0_value);

    mqttClient.beginMessage(moisture0_topic);
    mqttClient.print(moisture0_value);
    mqttClient.endMessage();


    // Serial.print("Sending message to topic: ");
    // Serial.println(moisture1_topic);
    Serial.print("moisture 1: ");
    Serial.println(moisture1_value);

    mqttClient.beginMessage(moisture1_topic);
    mqttClient.print(moisture1_value);
    mqttClient.endMessage();


    // Serial.print("Sending message to topic: ");
    // Serial.println(moisture2_topic);
    Serial.print("moisture 2: ");
    Serial.println(moisture2_value);

    mqttClient.beginMessage(moisture2_topic);
    mqttClient.print(moisture2_value);
    mqttClient.endMessage();


    // Serial.print("Sending message to topic: ");
    // Serial.println(moisture3_topic);
    Serial.print("moisture 3: ");
    Serial.println(moisture3_value);

    mqttClient.beginMessage(moisture3_topic);
    mqttClient.print(moisture3_value);
    mqttClient.endMessage();


    //humidity
    // Serial.print("Sending message to topic: ");
    // Serial.println(humidity_topic);
    Serial.print("humidity: ");
    Serial.println(humidity_value);

    mqttClient.beginMessage(humidity_topic);
    mqttClient.print(humidity_value);
    mqttClient.endMessage();

    //temperature
    // Serial.print("Sending message to topic: ");
    // Serial.println(temperature_topic);
    Serial.print("temperature: ");
    Serial.println(temperature_value);

    mqttClient.beginMessage(temperature_topic);
    mqttClient.print(temperature_value);
    mqttClient.endMessage();

    Serial.println();
  }
}