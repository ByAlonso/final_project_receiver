# 34365 IoT Prototyping Elderwell: Smart Platform For Elder Care
## Receiver gateway setup
In order to start using the gateway device, some things must be taken into account.
### Components
The following components are required to successfully get all device functionality:
- NodeMCU esp8266 wifi https://tienda.bricogeek.com/wifi/1033-nodemcu-v3-wifi-esp8266-ch340.html
- rn2483 LoRa Module https://www.tindie.com/products/drazzy/lorawan-rn2483rn2903-breakout-board-assembled/

### Libraries

This libraries should be installed in order to run the code successfully:
- <rn2xx3.h> https://github.com/jpmeijers/RN2483-Arduino-Library/blob/master/src/rn2xx3.h
- <SoftwareSerial.h> https://www.arduino.cc/en/Reference/SoftwareSerial
- <ESP8266WiFi.h> https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html
- <ESP8266HTTPClient.h> https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266HTTPClient
- <WiFiClient.h> https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi

### Setup Code
This variables placed in globals.cpp should be replaced for each user in order to enable WiFi connection:

- const char* ssid = "XXXXX"; // Router SSID
- const char* password = "XXXXXXXX"; // Router password
- String serverName = "http://www.xxxxxxx.xxxxxxx/xxxx.php"; // Server name or API

