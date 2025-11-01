#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

//#include <esp32-hal-ledc.h>

const char* ssid = "Zicore Technologies";
const char* password = "62292723";


const char* mqtt_server = "7ebfc64c2ccc4f7682d8677e511e3319.s1.eu.hivemq.cloud"; 
const int mqtt_port = 8883;
const char* mqtt_user = "Brightness_control";
const char* mqtt_pass = "Esp8266Esp32";

WiFiClientSecure espClient;
PubSubClient client(espClient);

const int ledPin = 5;
//const int pwmChannel=0;
//const int freq= 5000;
//const int resolution=8;


void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  char msg[5];
  memcpy(msg, payload, length);
  msg[length] = '\0';
  int brightness = atoi(msg);
  Serial.println(brightness);
  analogWrite(ledPin, brightness);
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client", mqtt_user, mqtt_pass)) 
    {
      client.subscribe("led/brightness");
    } 
    else 
    {
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  //ledcAttachChannel(ledPin, 5000, 8);
  setup_wifi();

  espClient.setInsecure();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  analogWrite(ledPin,0);
}

void loop() {
  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();
}
