#define RST D7
#define RX D6
#define TX D5
#include <rn2xx3.h>
#include <SoftwareSerial.h>
#include <string>
#include <vector>
#include <sstream>

String hexToAscii( String hex )
{
  uint16_t len = hex.length();
  String ascii = "";

  for ( uint16_t i = 0; i < len; i += 2 )
    ascii += (char)strtol( hex.substring( i, i + 2 ).c_str(), NULL, 16 );

  return ascii;
}

SoftwareSerial mySerial(TX, RX);
rn2xx3 myLora(mySerial);
String deviceId = "";
String humidity = "";
String temperature = "";
String CO2Value = "";
String windowStatus = "";

String str = "";
String hexData = "";
String stringData = "";
char * pch;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
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
    if(stringData != ""){
      pch = strtok (const_cast<char*>(stringData.c_str()), ",");
      deviceId = pch;
      pch = strtok (NULL, ",");
      humidity = pch;
      pch = strtok (NULL, ",");
      temperature = pch; 
      pch = strtok (NULL, ",");
      CO2Value = pch;
      pch = strtok (NULL, ",");
      windowStatus = pch;
      pch = strtok (NULL, ",");
      Serial.println(deviceId  + "," + humidity + "," + temperature + "," + CO2Value + "," + windowStatus);
    }
    else{
      Serial.println("Data not received correctly");
    }
  }
  else
  {
    Serial.println("radio not going into receive mode");
    delay(1000);
  }
  delay(5000);

}

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
