#include <rn2xx3.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "communications.hpp"
#include "globals.hpp"

SoftwareSerial mySerial(TX, RX);
rn2xx3 myLora(mySerial);
WiFiClient client;
HTTPClient http;
String fakeDataId[9] = {"9904A30B00F24385","8804A30B00F24385","7704A30B00F24385","6604A30B00F24385","5504A30B00F24385","4404A30B00F24385","3304A30B00F24385","2204A30B00F24385","1104A30B00F24385"};
struct deviceInfo {
  String humidity = "";
  String temperature = "";
  String CO2Value = "";
  String windowState = "";
};

std::map<String, deviceInfo> resultMap;

void setup() {
  Serial.begin(57600);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  mySerial.begin(9600);

  //reset rn2483
  pinMode(RST, OUTPUT);
  digitalWrite(RST, LOW);
  delay(500);
  digitalWrite(RST, HIGH);

  delay(100); //wait for the RN2xx3's startup message
  mySerial.flush();

  //initialise the rn2483 module
  myLora.autobaud();
  setupMySerial();

  //print out the HWEUI so that we can register it via ttnctl
  deviceId = String( myLora.hweui() );
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("waiting for a message");
  mySerial.println("radio rx 0"); //wait for 60 seconds to receive

  str = mySerial.readStringUntil('\n');
  if ( str.indexOf("ok") == 0 )
  {
    str = String("");
    while (str == "")
    {
      str = mySerial.readStringUntil('\n');
    }

    pch = strtok (const_cast<char*>(str.c_str()), " ");
    pch = strtok (NULL, " ");
    hexData = pch;
    stringData = hexToAscii(hexData);
    if (stringData != "") {
      deviceInfo newDevice;
      pch = strtok (const_cast<char*>(stringData.c_str()), ",");
      deviceId = pch;
      pch = strtok (NULL, ",");
      newDevice.humidity = pch;
      pch = strtok (NULL, ",");
      newDevice.temperature = pch;
      pch = strtok (NULL, ",");
      newDevice.CO2Value = pch;
      pch = strtok (NULL, ",");
      newDevice.windowState = pch;
      pch = strtok (NULL, ",");
      resultMap[deviceId] = newDevice;
      fakeData();
      sendData();
      Serial.println(generateJson());
    }
    else {
      Serial.println("Data not received correctly");
    }
  }
  else
  {
    Serial.println("radio not going into receive mode");
    delay(1000);
  }

  delay(30000);

}

//Sets up LoRa p2p device
void setupMySerial() {
  Serial.println("Initing LoRa");

  mySerial.println("mac pause");
  str = mySerial.readStringUntil('\n');
  Serial.println(str);

  mySerial.println("radio set mod lora");
  str = mySerial.readStringUntil('\n');
  Serial.println(str);

  mySerial.println("radio set freq 869100000");
  str = mySerial.readStringUntil('\n');
  Serial.println(str);

  mySerial.println("radio set pwr 14");
  str = mySerial.readStringUntil('\n');
  Serial.println(str);

  mySerial.println("radio set sf sf7");
  str = mySerial.readStringUntil('\n');
  Serial.println(str);

  mySerial.println("radio set afcbw 41.7");
  str = mySerial.readStringUntil('\n');
  Serial.println(str);

  mySerial.println("radio set rxbw 125");
  str = mySerial.readStringUntil('\n');
  Serial.println(str);

  mySerial.println("radio set prlen 8");
  str = mySerial.readStringUntil('\n');
  Serial.println(str);

  mySerial.println("radio set crc on");
  str = mySerial.readStringUntil('\n');
  Serial.println(str);

  mySerial.println("radio set iqi off");
  str = mySerial.readStringUntil('\n');
  Serial.println(str);

  mySerial.println("radio set cr 4/5");
  str = mySerial.readStringUntil('\n');
  Serial.println(str);

  /*mySerial.println("radio set wdt 60000"); //disable for continuous reception
    str = mySerial.readStringUntil('\n');
    Serial.println(str);*/

  mySerial.println("radio set sync 12");
  str = mySerial.readStringUntil('\n');
  Serial.println(str);

  mySerial.println("radio set bw 125");
  str = mySerial.readStringUntil('\n');
  Serial.println(str);
}

//Creates post petition and sends it to the server app
void sendData() {
  http.begin(client, serverName);
  http.addHeader("Content-Type", "application/json"); //maybe plain/text instead of application/json
  String json = generateJson();

  int httpCode = http.POST(json);   //Send the request
  String payload = http.getString();                  //Get the response payload

  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  http.end();
}

//Generates JSON file with the parameters obtained from sensor device
String generateJson() {
  String json = "{\"devices\": [";

  std::map<String, deviceInfo>::iterator it;

  for (it = resultMap.begin(); it != resultMap.end(); it++)
  {
    json += "{\"deviceId\":\"" + it->first + "\"," ;
    json += "\"temperature\":\"" + it->second.temperature + "\"," ;
    json += "\"humidity\":\"" + it->second.humidity + "\"," ;
    json += "\"CO2\":\"" + it->second.CO2Value + "\"," ;
    json += "\"windowState\":\"" + it->second.windowState + "\"" ;
    if (std::next(it) == resultMap.end()) {
      json += "}";
    } else {
      json += "},";
    }
  }
  json += "]}";
  return json;
}

//Creates random data
void fakeData() {
  for(int i = 0; i < 9; i++){
    deviceInfo newDevice;
    deviceId = fakeDataId[i];
    newDevice.humidity = String(float(random(100,10000))/100);
    newDevice.temperature = String(float(random(1800,4000))/100);
    newDevice.CO2Value = String(float(random(100,200000))/100);
    newDevice.windowState = String(random(0,2));
    resultMap[deviceId] = newDevice;
  }
}
