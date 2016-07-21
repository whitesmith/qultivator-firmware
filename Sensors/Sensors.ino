#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"
#include <Wire.h>
#include "Adafruit_TCS34725.h"

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

void setup() {
  Serial.begin(115200);

  soilTemperatureSensor.begin();
  environmentSensor.begin();
  lightSensor.begin();

  pinMode(SOIL_PIN, INPUT);
}

void loop() {
  soilTemperatureSensor.requestTemperatures();
  Serial.print("Soil Temperature: ");
  Serial.println(soilTemperatureSensor.getTempCByIndex(0));

  Serial.print("Environment Humidity: ");
  Serial.println(environmentSensor.readHumidity());

  Serial.print("Environment Temperature: ");
  Serial.println(environmentSensor.readTemperature());
  
  lightSensor.getRawData(&r, &g, &b, &c);
  lightTemperature = lightSensor.calculateColorTemperature(r, g, b);
  lux = lightSensor.calculateLux(r, g, b);
  Serial.print("Light Temperature: ");
  Serial.print(lightTemperature, DEC);
  Serial.println(" K");
  Serial.print("Light Intensity: ");
  Serial.println(lux, DEC);

  soilHumidity = analogRead(SOIL_PIN);
  Serial.print("Soil Humidity: ");
  Serial.println(soilHumidity);

  delay(3000);
}
