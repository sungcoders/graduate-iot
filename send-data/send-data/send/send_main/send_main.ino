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
const byte        mac[] = { 0x54, 0x34, 0x41, 0x30, 0x30, 0x35 };
const uint8_t     IP[] = {192, 168, 1, 199};
const char* mqtt_server = "mohinhrauthuycanh.ddns.net";
const char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tueday", "Wed", "Thuday", "Friday", "Satuday"};
// ********************************** khai báo biến ***************************************//
int   deviceCount;                    // biến nhận số lượng ds18b20
byte  countMQTT=0, count_ar=0 ;       // biến đếm MQTT
char  extra[100];                     // biến lưu tạm thời toàn cục
// ********************************** khai báo hàm con ***************************************//
extern volatile unsigned long timer0_millis;
unsigned long ts, tar, t_noti;       // ts

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
//  init_send_once();
}

//-----------------------------------------loop---------------------------------
void loop()
{
  wdt_enable(WDTO_4S);
//  while((Ethernet.linkStatus()!=LinkON) || (!client.connected()) )
//  {
//    Serial.println(F("check EQ in loop"));
//    if (Ethernet.linkStatus() != LinkON)  { ethernet();         }
//    if (!client.connected())              { MQTTreconnect();    }
//  }
//  client.loop();
  if(millis()-ts>=3000)
  {
    YHDC100();
    tds();
    Serial.println("ds18b20");
    multi_ds18b20();
    ts=millis();
  }
  if(millis() >= 2592000000) //30 day
  {
    delayMicroseconds(500);
    milirst();
  }
  wdt_disable();
  delayMicroseconds(100);
}

void milirst()
{
  noInterrupts();
  timer0_millis = 0;
  interrupts();
}
