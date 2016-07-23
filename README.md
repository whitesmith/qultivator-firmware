# Qultivator Firmware

Qultivator is a project built under 32h for the Whitesmith Hackathon (July 2016).
It is a IoT project that allows plants to be individually monitored and taken cared of in real-time.
Several environmental data can be gathered using sensors, and both a light and irrigation system can be controlled in real-time using the Qultivator website.

![Demo Unit](https://raw.githubusercontent.com/whitesmith/qultivator-firmware/master/qultivator.jpg)

_Demo Unit_


### LINKS

https://qultivator.whitesmith.co

https://github.com/whitesmith/qultivator-website

https://github.com/whitesmith/qultivator-websockets


### B.O.M.

- NodeMCU ESP8266 with Arduino Core
- 1-WIRE Digital Thermometer (DS18B20)
- DHT22 Temperature and Humidity Sensor
- TCS34725 Light Sensor
- Soil Moisture Sensor
- 2 Relays
- LED Lamp
- Irrigation System


### Instructions

- Check the code in WebSocketClient.ino for the correct pins for each sensor.
- Copy the file `passwords.h.template` into `passwords.h` and insert the correct values.
- Flash the NodeMCU with the code.
- Start the Server at https://github.com/whitesmith/qultivator-websockets
- Start the Web Interface at https://github.com/whitesmith/qultivator-website
