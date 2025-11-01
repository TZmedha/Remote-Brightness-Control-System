#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>


const char* ssid = "Zicore Technologies";
const char* password = "62292723";


const char* mqtt_server = "7ebfc64c2ccc4f7682d8677e511e3319.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_user = "Brightness_control";
const char* mqtt_pass = "Esp8266Esp32";


WiFiClientSecure espClient;
PubSubClient client(espClient);


void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}


void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP8266Client", mqtt_user, mqtt_pass)) {
    } else {
      delay(5000);
    }
  }
}


void setup() {
  Serial.begin(115200);
  setup_wifi();

  espClient.setInsecure();

  client.setServer(mqtt_server, mqtt_port);
}


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int potValue = analogRead(A0);
  int brightness = map(potValue, 0, 1023, 0, 255);
  
  StaticJsonDocument<128> doc;
  doc["brightness"]= brightness;

  char sndmsg[100];
  serializeJson(doc, sndmsg);

  // char msg[4];
  // snprintf(msg, sizeof(msg), "%d", brightness);
  client.publish("led/brightness", sndmsg);

  Serial.print("sent :");
  Serial.println(sndmsg);

  delay(500);
}
