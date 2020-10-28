#include <Arduino_FreeRTOS.h>
#include <avr/wdt.h>
#include <UIPEthernet.h>
#include <PubSubClient.h>
#include "RTClib.h"
#include <DallasTemperature.h>
#include <OneWire.h>
#include "LCDSPI_SCDT.h"
#include "GravityTDS.h"
#define   ONE_WIRE_BUS 3
#define   pinSCT A3
#define   TdsSensorPin A1
#define   VREF 5.0
#define   SCOUNT  30
          RTC_DS1307 rtc;
          OneWire onewire(ONE_WIRE_BUS);
          DallasTemperature sensors_ds(&onewire);
          GravityTDS gravityTds;
          EthernetClient Ethclient;
          PubSubClient client(Ethclient);
// thông tin kết nối
const byte        mac[] = {0x54,0x34,0x41,0x30,0x30,0x35};
const uint8_t     IP[] = {192,168,1,198};
const uint8_t     dns1[] = {8,8,8,8};
const uint8_t     gateway[] = {192,168,1,1};
const uint8_t     subnetmsk[] = {255,255,255,0};
const char* mqtt_server = "mohinhrauthuycanh.ddns.net";
const char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tueday", "Wed", "Thuday", "Friday", "Satuday"};
// ********************************** khai báo biến ***************************************//
int   deviceCount;                    // biến nhận số lượng ds18b20
byte  count_ar=0 ;                    // biến thời gian
char cvt_c[50];
unsigned long ts, tar, t_noti;
// ********************************** khai báo hàm con ***************************************//
extern volatile unsigned long timer0_millis;

void(* resetFunc) (void) = 0;               //cài đặt hàm reset
void ethernet();
void MQTTreconnect();
void init_startup();
void init_send_once();
void send_ar();
void ar_other();
void multi_ds18b20();
void sd_card();
void YHDC100();
void tds();
void rtcds1307();
void milirst();

// ------------------------------ setup ----------------------------------------
void setup()
{
  init_startup();
  delayMicroseconds(100);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  init_send_once();
  xTaskCreate([](void *prm) {
    while(1)
    {
      wdt_enable(WDTO_8S);
      if (!client.connected())              { MQTTreconnect();    }
//      if(!client.loop()) { client.connect("arduinoClient"); }
      client.loop();
      if(millis()-ts>=5000)
      {
        Serial.println("tds");
        tds();
        Serial.println("yhdc");
        YHDC100();
        ds18b20(1);
        Serial.println("millis");
        ts=millis();
      }
      if(millis() >= 2592000000) //30 day
      {
        send_ar();
        delayMicroseconds(500);
        milirst();
      }
      else  { ar_other(); }
      wdt_disable();
    }
  },
  "ct2",
  2500,
  NULL,
  2,
  NULL);
  xTaskCreate([](void *prm) {
    while(1)
    {
      if (!client.connected())              { MQTTreconnect();    }
      client.loop();
    }
  },
  "ct1",
  1024,
  NULL,
  1,
  NULL);
} // end setup

//-----------------------------------------loop---------------------------------
void loop()
{
  
} // end loop

void milirst()
{
  noInterrupts();
  timer0_millis = 0;
  interrupts();
}

void convertcharf(float fl1)
{
  String fls=(String)fl1;
  fls.toCharArray(cvt_c,fls.length()+1);
}

void convertcharfp(float fl1, float fl2)
{
  String fls1=(String)fl1;
  String fls2=(String)fl2;
  char flc1[15],flc2[15];
  fls1.toCharArray(flc1,fls1.length()+1);
  fls2.toCharArray(flc2,fls2.length()+1);
  sprintf(cvt_c,"%s  ,%s",flc1,flc2);
}
