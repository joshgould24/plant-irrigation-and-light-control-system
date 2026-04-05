#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>

#include "arduino_secrets.h"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char wifi_ssid[] = SECRET_WIFI_SSID;    // network SSID (name)
char wifi_pass[] = SECRET_WIFI_PASS;    // network password (use for WPA, or use as key for WEP)
char mqtt_broker_ip[] = SECRET_MQTT_BROKER_IP;  // IP address of the MQTT broker
char mqtt_username[] = SECRET_MQTT_USERNAME;  // MQTT username
char mqtt_password[] = SECRET_MQTT_PASSWORD;  // MQTT password

// To connect with SSL/TLS:
// 1) Change WiFiClient to WiFiSSLClient.
// 2) Change port value from 1883 to 8883.
// 3) Change broker value to a server with a known SSL/TLS root certificate 
//    flashed in the WiFi module.

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = SECRET_MQTT_BROKER_IP;
int        port     = 1883;

//topics
const char pump_topic[]     = "arduino/pump";

//inputs
int moisture_ain0=A0;

//values
int moisture0_value;


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
  Serial.println(wifi_ssid);
  while (WiFi.begin(wifi_ssid, wifi_pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  // You can provide a unique client ID, if not set the library uses Arduino-millis()
  // Each client must have a unique client ID
  // mqttClient.setId("clientId");

  // provide a username and password for authentication
  mqttClient.setUsernamePassword(mqtt_username, mqtt_password);

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

    // set the message receive callback
  mqttClient.onMessage(onPumpMessage);

  Serial.print("Subscribing to topic: ");
  Serial.println(pump_topic);
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe(pump_topic);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(pump_topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(pump_topic);
  Serial.println();

}

void(* resetFunc) (void) = 0;

void loop() {
  // call poll() regularly to allow the library to send MQTT keep alives which
  // avoids being disconnected by the broker
  mqttClient.poll();
}

void onPumpMessage(int messageSize) {
  // lets pump that fucking plant!
  Serial.println("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");
}