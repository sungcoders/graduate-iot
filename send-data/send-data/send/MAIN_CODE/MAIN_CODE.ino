#include <UIPEthernet.h>
#include <PubSubClient.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <avr/wdt.h>
#include <Wire.h>
#include "DHT.h"
#include "LCDSPI_SCDT.h"
#define   TdsPin A8
#define   TIME1 1000
#define   TIMEONEDAY  86400
          OneWire onewire(37);
          DallasTemperature sensors_ds(&onewire);
          EthernetClient Ethclient;
          PubSubClient client(Ethclient);
          DHT dht(39, DHT22);

const byte      mac[] = {0x54,0x34,0x41,0x30,0x30,0x35};
const uint8_t   IP[] = {192,168,1,198};
const uint8_t   dns1[] = {8,8,8,8};
const uint8_t   gateway[] = {192,168,1,1};
const uint8_t   subnetmsk[] = {255,255,255,0};
const char* mqtt_server = "rauthuycanh.ddns.net";

int   pinout[7]={8,9,10,11,12,6,7};
byte  dem=0 ;
unsigned long ts, tar, t_noti,t1,t2,t3,t4,t5;

extern volatile unsigned long timer0_millis;
void(* resetFunc) (void) = 0;               //cài đặt hàm reset
void milirst();
char* num_to_char(float num, String str1);
char* float_to_char(float num, String str1);
char* string_to_char(String str);

void setup(void)
{
  init_startup();
  delayMicroseconds(100);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  init_send_once();
}

void loop(void)
{
  wdt_enable(WDTO_8S);
  if(!client.connected()) { MQTTreconnect();}
  client.loop();
  if(millis()-t1>=TIME1)
  {
    (check_rtc()==1)?read_rtc(),rtc_display():lcd_print(0,0,"      RTC not run    ");
    BH1750_display();
    DHT_read();
    ds18b20(1);
    ds18b20(2);
    t1=millis();
  }
  if(millis()-t3>=TIME1*3)
  {
    readTds();
    YHDC30();
    t3=millis();
  }
  (millis() >= TIMEONEDAY*30)?milirst():ar_other();  // 30 day
  if(millis()-t5>TIME1*60) // 1phut
  {
    Serial2.write("MEGA2560");
    Serial.println(F("da gui mega2560"));
    t5=millis();
  }
  wdt_disable();
}

void milirst()
{
  noInterrupts();
  timer0_millis = 0;
  interrupts();
}

char* string_to_char(String str)
{
  static char cvc[50];
  str.toCharArray(cvc,str.length()+1);
  return (cvc);
}

char* float_to_char(float num, String str1)
{
  String str=(String)num+str1;
  static char cvc[50];
  str.toCharArray(cvc,str.length()+1);
  return (cvc);
}

char* num_to_char(int num, String str1)
{
  String str=(String)num+str1;
  static char cvc[50];
  str.toCharArray(cvc,str.length()+1);
  return (cvc);
}
