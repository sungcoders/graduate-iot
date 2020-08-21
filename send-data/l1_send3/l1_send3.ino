#include <Wire.h>
#include <UIPEthernet.h>
#include <PubSubClient.h> 
#include <LiquidCrystal_I2C.h>
          LiquidCrystal_I2C lcd(0x27, 16, 2);
#include "RTClib.h"
          RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
#include <DallasTemperature.h>
#include <OneWire.h>
#define ds1 50
          OneWire onewire(ds1);
          DallasTemperature sensords(&onewire);
#include "DHT.h"
const int dhtpin = 49;                                        // pin dht chân 49 mega
const int dhttype = DHT11;
          DHT dht(dhtpin,dhttype);

byte mac[] = { 0x54, 0x34, 0x41, 0x30, 0x30, 0x35 };
uint8_t IP[]={192,168,1,67};
const char* mqtt_server = "192.168.1.99";                      //172.26.80.17      172.26.80.100         
          EthernetClient Ethclient;
          PubSubClient client(Ethclient);

unsigned long tsensor=0, ts;
byte countMQTT=0;                     
void(* resetFunc) (void) = 0;//cài đặt hàm reset

// ------------------------------setup------------------------------------------

void setup() 
{
    Serial.begin(9600);
    lcd.begin();
    lcd.backlight();
    lcd.print("   start lcd");
    Serial.println("\nserial started in setup");
    pinMode(46, OUTPUT);
    pinMode(13,OUTPUT);
    Wire.begin();
    dht.begin();
//    if (! rtc.begin()) {  Serial.print("Couldn't find RTC");  lcd.clear();  lcd.print("cannot find RTC");   while (1); }
//    if (! rtc.isrunning())  {Serial.print("RTC is NOT running! \n");  lcd.clear();  lcd.print("RTC not run");}
// --------------???----------???-----------------------------------------------------
    //rtc.adjust(DateTime(2020, 8, 3, 7, 39, 0));
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
// --------------!!!------------!!!---------------------------------------------------
    delay(500);
    client.setServer(mqtt_server, 1883);
}

//-----------------------------------------loop---------------------------------

void loop() 
{
   if(Ethernet.linkStatus()!=LinkON)    {   ethernet(); }
   if(!client.connected())  {  MQTTreconnect(); }
   if(!client.loop()) client.connect("arduinoClient");
   if((millis()-tsensor)>=8000)
    {
       DHT11sensor();
       tsensor=millis();
       delay(200);
    }
   if((millis()-ts)>=1000)
    {
    ts=millis();
    rtcds1307();
    }
   DateTime now = rtc.now();
   if((now.hour()==14 || now.hour()==2) && now.minute()==00)
    {
      Serial.println("reset board after for 12h");
      client.publish("arduino","arduino taking a rest");
      lcd.clear();
      lcd.print("reset board 12h");
      lcd.setCursor(0,1);
      lcd.print("reset board 12h");
      delay(20000);
      client.publish("arduino","arduino taking a rest");
      delay(10000);
      client.publish("arduino","arduino taking a rest");
      delay(15000);
      client.publish("arduino","arduino taking a rest");
      delay(1000);
      resetFunc();
    }
} // end loop
