#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
////////////////////////////////////////////////////////////////////////////////
//////////Define Pin///////////////////////////////////////////////////////////
#define USE_SERIAL Serial
/////////////////-----------------------------------//////////////////////////
int solinoid = 4;             //setpin of solinoid at D2
int pump = 5;                 //set pint to D1 
int Led_Red = 14;            //setpin of Led Red at D5
int Led_Green = 12;            //setpin of Led Red at D6
int Led_Blue = 13;            //setpin of Led Red at D7
///////////////////////////////////////////////////////////////////////////////
void SendData(String c);     //Define Senddata Voide
////////////Setup//////////////////////////////////////////////////////////////
const char* ssid     = "iot";            //Set ssid
const char* password = "12345678";                    //Set Password
const char* Server   = "188.166.223.204";           //set Server Domain or Server ip
ESP8266WiFiMulti WiFiMulti;


void setup() 
{
    USE_SERIAL.begin(115200);
      for(uint8_t t = 6; t > 0; t--) 
      {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
      }
    WiFiMulti.addAP(ssid, password);    //Set SSID and Password (SSID, Password)
    WiFi.begin(ssid, password);         //Set starting for Wifi
    Serial.println(WiFi.localIP());
    ////////////////////////////////SetuoPinMode//////////////////////////////////////////
    pinMode(solinoid,OUTPUT);
    pinMode(pump,OUTPUT);                
    pinMode(Led_Red, OUTPUT);
    pinMode(Led_Green, OUTPUT);
    pinMode(Led_Blue, OUTPUT);
    digitalWrite(pump,HIGH);               
}

////////////////////Loop////////////////////////////////////////////////////////////////////

void loop() 
{ 
    String c = "OK";
    SendData(c);
}
///////////////////////SednData//////////////////////////////////////////////////////////////////
void SendData(String c) 
{
  
  // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) 
    {
        HTTPClient http;
        String str = "http://" +String(Server)+":5005" +"/data/" +c;
        Serial.println(str);
        http.begin(str);
        int httpCode = http.GET();
        USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
        if(httpCode > 0) 
        {
            if(httpCode == HTTP_CODE_OK) 
              {
                String payload = http.getString();
      //-------------------Control----------------------------//
                USE_SERIAL.print("payload");
                USE_SERIAL.println(payload);
                if(payload.substring(0,1)=="1")
                {
                  digitalWrite(solinoid,HIGH);
                  digitalWrite(pump,LOW);
                }
                else
                { 
                  digitalWrite(solinoid,LOW);
                  digitalWrite(pump,HIGH);
                  if(payload.substring(0,1)=="2")
                  { 
                    digitalWrite(pump,LOW);
                    int d = (payload.substring(2,3).toInt())*1000;
                    digitalWrite(solinoid,HIGH);
                    delay(d);
                    digitalWrite(solinoid,LOW);
                    digitalWrite(pump,LOW);
                  }
                }
                int Red = payload.substring(20,23).toInt();   //edit subString
                int Green = payload.substring(24,27).toInt();
                int Blue = payload.substring(28,31).toInt();
                //Serial.println("Red%03d,Green%03d,Blue%03d"(Red,Green,Blue));
                Serial.print("Red"); Serial.print(Red);
                Serial.print("Green"); Serial.print(Green);
                Serial.print("Blue"); Serial.println(Blue);
                analogWrite(Led_Red,Red);
                analogWrite(Led_Green,Green);
                analogWrite(Led_Blue,Blue);
                
              }
              else
                {
                //go_auto(w);
                }
        http.end();   
      }
      else
      {
     // go_auto(w);
      }
    
    delay(1000);
  }
}





