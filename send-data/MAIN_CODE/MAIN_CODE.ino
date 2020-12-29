#include <UIPEthernet.h>
#include <PubSubClient.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Wire.h>
#include "DHT.h"
#include "LCDSPI_SCDT.h"

#define   TIME01S     1000
#define   TIME05S     5000
#define   TIME10S     10000
#define   TIME01M     60000
#define   TIME01DAY   86400
#define   TIME30D     2592000
#define   ERROR       9999

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

int     pinout[] = {8,9,10,11,12,6,7};
byte    dem=0 ;
int     check_motor = 0;
int     stt_dc = 8888;
int     t2;
int     giay,phut,gio,ngay,thang,nam,wd;
char    tg[50];
unsigned long t1;

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
  (!client.connected())?MQTTreconnect():client.loop();
  if(check_rtc()==1)
  {
    if((phut-t2)>=1  || (phut-t2)<0)
    {
      Serial2.write("MEGA2560");
      rtc_display();
      t2=phut;
    }
  }
  else
  {
    if(millis()-t1>=TIME01M)
    {
      Serial1.write("MEGA2560");
      t1=millis();
    }
    if(millis()==TIME30D)
    {
      milirst();
    }
    lcd_print(0, 0, "      RTC not run    ");
  }
  if((check_motor==1))
  {
    delay(500);
    if((stt_dc == 1) && (YHDC30()>0.0))
    {
      check_motor=ERROR;
    }
    else if((stt_dc == 0) && (YHDC30()<=0.05))
    {
      client.publish("on_off_YHDC","STOP");
      check_motor=ERROR;
    }
    else if(check_motor>10)
    {
      check_motor=ERROR;
      client.publish("on_off_YHDC","ERROR");
    }
    check_motor++;
  }
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
