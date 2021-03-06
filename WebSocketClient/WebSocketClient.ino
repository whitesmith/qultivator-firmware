#include <ESP8266WiFi.h>
#include <WebSocketClient.h>
#include "./passwords.h"

#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"
#include <Wire.h>
#include "Adafruit_TCS34725.h"

String id = "p01";

WebSocketClient webSocketClient;

// Use WiFiClient class to create TCP connections
WiFiClient client;

#define SOIL_TEMPERATURE_PIN 14
OneWire soilTemperatureWire(SOIL_TEMPERATURE_PIN);
DallasTemperature soilTemperatureSensor(&soilTemperatureWire);

#define ENVIRONMENT_PIN 2
#define ENVIRONMENT_TYPE DHT22
DHT environmentSensor(ENVIRONMENT_PIN, ENVIRONMENT_TYPE);

Adafruit_TCS34725 lightSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);
uint16_t r, g, b, c, lightTemperature, lux;

#define SOIL_PIN A0
uint16_t soilHumidity;

#define LIGHT_PIN 12

#define WATER_PIN 13

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(5000);


  // Connect to the websocket server
  if (client.connect(host, 80)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
    while (1) {
      // Hang on failure
    }
  }

  // Handshake with the server
  webSocketClient.path = path;
  webSocketClient.host = host;
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {
    Serial.println("Handshake failed.");
    while (1) {
      // Hang on failure
    }
  }


  soilTemperatureSensor.begin();
  environmentSensor.begin();
  lightSensor.begin();
  pinMode(SOIL_PIN, INPUT);
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(WATER_PIN, OUTPUT);

}

boolean waterON = false;
unsigned long previousMillis = 0;
const long interval = 3000;
void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    if (waterON) {
      digitalWrite(WATER_PIN, 0);
      waterON = false;
    }
  }

  String data;
  String message = "{\"id\": \"" + id + "\",";

  if (client.connected()) {

    webSocketClient.getData(data);
    if (data.length() > 0) {
      Serial.print("Received data: ");
      Serial.println(data);
      if (data == "lightON") {
        digitalWrite(LIGHT_PIN, 0);
      }
      else if (data == "lightOFF") {
        digitalWrite(LIGHT_PIN, 1);
      }
      else if (data == "waterON") {
        digitalWrite(WATER_PIN, 1);
        waterON = true;
      }
    }

    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;


      soilTemperatureSensor.requestTemperatures();

      message += "\"sT\":" + String( soilTemperatureSensor.getTempCByIndex(0) ) + ",";
      message += "\"eH\":" + String( environmentSensor.readHumidity() ) + ",";
      message += "\"eT\":" + String( environmentSensor.readTemperature() ) + ",";

      lightSensor.getRawData(&r, &g, &b, &c);
      lightTemperature = lightSensor.calculateColorTemperature(r, g, b);
      lux = lightSensor.calculateLux(r, g, b);

      message += "\"lT\":" + String( lightTemperature ) + ",";
      message += "\"lI\":" + String( lux ) + ",";

      soilHumidity = analogRead(SOIL_PIN);
      message += "\"sH\":" + String( soilHumidity );
      message += "}";

      Serial.println(message);

      webSocketClient.sendData(message);
    }

  } else {
    Serial.println("Client disconnected.");
    while (1) {
      // Hang on disconnect.
    }
  }

  yield();
}
