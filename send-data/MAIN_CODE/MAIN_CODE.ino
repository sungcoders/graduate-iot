#include <UIPEthernet.h>
#include <PubSubClient.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <avr/wdt.h>
#include <Wire.h>
#include "DHT.h"
#include "LCDSPI_SCDT.h"

#define   TIME01S     1000
#define   TIME05S     5000
#define   TIME10S     10000
#define   TIME01M     60000
#define   TIME01DAY   86400
#define   TIME30DAY   2592000

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
const char*     mqtt_server = "rauthuycanh.ddns.net";

int   pinout[] = {8,9,10,11,12,6,7};
byte  dem=0 ;
int    check_motor = 0;
unsigned long ts, tar, t_noti,t1,t2,t3,t4,t5;
int giay,phut,gio,ngay,thang,nam,wd;
char tg[50];

extern volatile unsigned long timer0_millis;
void(* resetFunc) (void) = 0;               //cài đặt hàm reset

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
  (!client.connected())?MQTTreconnect():client.loop();  
  if(millis()-t1>=TIME01S)
  {
    if(check_rtc()==1)
    {
      read_rtc();
      rtc_display();
      BH1750_display();
      ds18b20(1);
      ds18b20(2);
      t1=millis();
    }
  }
  if(check_motor==1  && ((millis()-t3)>TIME05S))
  {
      YHDC30();
      t3=millis();
  }
  if(millis()-t2>=TIME01M)
  {
    Serial2.write("MEGA2560");
    client.publish("send_update","no request");
    YHDC30();
    readTdsQuick();
    DHT_read();
    t2=millis();
  }
  (millis() >= TIME30DAY)?milirst():ar_other();                   // 30 day
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
