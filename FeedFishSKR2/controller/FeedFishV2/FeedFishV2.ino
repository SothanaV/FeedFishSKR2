#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
////////////////////////////////////////////////////////////////////////////////
//////////Define Pin///////////////////////////////////////////////////////////
#define USE_SERIAL Serial
#include "DHT.h"
int DHTPIN = 2;          //setpin of DHT at D4
int DHTTYPE = DHT11;     //set Dht Type
/////////////////-----------------------------------//////////////////////////
int s = 4;             //setpin of solinoid at D2
int pump = 5;                 //set pint to D1 
int Led_Red = 14;            //setpin of Led Red at D5
int Led_Green = 12;            //setpin of Led Red at D6
int Led_Blue = 13;            //setpin of Led Red at D7
///////////////////////////////////////////////////////////////////////////////
void SendData(String c);     //Define Senddata Voide
////////////Setup//////////////////////////////////////////////////////////////
const char* ssid     = "iot";            //Set ssid
const char* password = "12345678";                    //Set Password
const char* Server   = "128.199.198.154";           //set Server Domain or Server ip
ESP8266WiFiMulti WiFiMulti;
DHT dht(DHTPIN, DHTTYPE);

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
    dht.begin();
    ////////////////////////////////SetuoPinMode//////////////////////////////////////////
    pinMode(s,OUTPUT);
    pinMode(pump,OUTPUT);                
    pinMode(Led_Red, OUTPUT);
    pinMode(Led_Green, OUTPUT);
    pinMode(Led_Blue, OUTPUT);
    digitalWrite(pump,HIGH);               
}

////////////////////Loop////////////////////////////////////////////////////////////////////

void loop() 
{ 
    float h = dht.readHumidity();      //Read Humidity
    float t = dht.readTemperature();   //Read Temperature
    String c = "OK";
    if (isnan(t) || isnan(h)) 
  {
    Serial.println("FaiLed to read from DHT");
  } 
  else
  {
    SendData(t,h);
  }
}
///////////////////////SednData//////////////////////////////////////////////////////////////////
void SendData(float t,float h) 
{
  
  // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) 
    {
        HTTPClient http;
        String str = "http://" +String(Server)+":5000" +"/data/"+t+"/"+h;
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
                  digitalWrite(s,HIGH);
                  digitalWrite(pump,LOW);
                }
                else
                { 
                  digitalWrite(s,LOW);
                  digitalWrite(pump,HIGH);
                  if(payload.substring(0,1)=="2")
                  { 
                    digitalWrite(pump,LOW);
                    int d = (payload.substring(2,3).toInt())*1000;
                    Serial.print("delaytime");
                    Serial.println(d);
                    digitalWrite(s,HIGH);
                    delay(d);
                    digitalWrite(s,LOW);
                    digitalWrite(pump,LOW);
                  }
                }
                String Red = payload.substring(4,8);
                String Green = payload.substring(8,12);
                String Blue = payload.substring(12,17);
                int v_red = Red.toInt();
                int v_green = Green.toInt();
                int v_blue = Blue.toInt();
                analogWrite(Led_Red,v_red);
                analogWrite(Led_Green,v_green);
                analogWrite(Led_Blue,v_blue);
                //Serial.println("Red%03d,Green%03d,Blue%03d"(Red,Green,Blue));
                Serial.print("Red"); Serial.print(v_red);
                Serial.print("Green"); Serial.print(v_green);
                Serial.print("Blue"); Serial.println(v_blue);
                
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





