#include <Wire.h>
#include <SPI.h>
#include <UIPEthernet.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>
          LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <SD.h>
          File f;
#include "RTClib.h"
          RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tueday", "Wed", "Thuday", "Friday", "Satuday"};
#include <DallasTemperature.h>
#include <OneWire.h>
#define   ONE_WIRE_BUS 3
          OneWire onewire(ONE_WIRE_BUS);
          DallasTemperature sensors_ds(&onewire);
#include "EmonLib.h"
          EnergyMonitor SCT013;
#define   pinSCT  A3
#include "GravityTDS.h"
#define   TdsSensorPin A1
          GravityTDS gravityTds;
#define   VREF 5.0      // analog reference voltage(Volt) of the ADC
#define   SCOUNT  30           // sum of sample point
// thông tin kết nối
byte        mac[] = { 0x54, 0x34, 0x41, 0x30, 0x30, 0x35 };
uint8_t     IP[] = {192, 168, 1, 199};
const char* mqtt_server = "mohinhrauthuycanh.ddns.net";
            EthernetClient Ethclient;
            PubSubClient client(Ethclient);
// ********************************** khai báo biến ***************************************//
int         deviceCount;                    // biến nhận số lượng ds18b20
byte        countMQTT=0, count_ar=0 ;                  // biến đếm MQTT
char        extra[100];                     // biến lưu tạm thời toàn cục
// ********************************** khai báo TDS ***************************************//
float       temperature = 25, tdsValue = 0;
int         sum_tds = 0;
// ********************************** khai báo hàm con ***************************************//
extern volatile unsigned long timer0_millis;
unsigned long tsensor = 0, ts, t_tds, tar, t_noti;       // ts
void(* resetFunc) (void) = 0;               //cài đặt hàm reset
void ethernet();
void MQTTreconnect();
void init_startup();
void send_ar();
void ar_other();
void multi_ds18b20();
void sd_card();
void YHDC100();
void init_send_once();
void tds();
void rtcds1307();
void milirst();

// ------------------------------setup------------------------------------------
void setup()
{
  init_startup();
  delay(100);
  // thiết lập kết nối server
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  init_send_once();
}

//-----------------------------------------loop---------------------------------
void loop()
{
  if (Ethernet.linkStatus() != LinkON)  { ethernet();                       }
  if (!client.connected())              { MQTTreconnect();                  }
  if (!client.loop())                   { client.connect("arduinoClient");  }
  if ((millis() - tsensor) >= 1500)
  {
    delayMicroseconds(5);
    multi_ds18b20();
    delayMicroseconds(5);
    sd_card();
    delayMicroseconds(5);
    client.publish("caytrong", "cay_trong");
    tsensor = millis();
  }
  if((millis()-t_noti)>=10000)
  {
    client.publish("status","không có thông báo mới");
    t_noti = millis();
  }
  if (millis() - t_tds > 2980)
  {
    delayMicroseconds(5);
    YHDC100();
    delayMicroseconds(5);
    tds();
    t_tds = millis();
  }
  DateTime now = rtc.now();
  if ((now.hour() == 14 || now.hour() == 2) && now.minute() == 00)
  {
    delayMicroseconds(5);
    send_ar();
  }
  else
  {
    delayMicroseconds(5);
    ar_other();
  }
  if (millis() >= 2592000000) //30 day
  {
    delayMicroseconds(500);
    milirst();
    tsensor = 0;
  }
} //end loop

void milirst()
{
  noInterrupts();
  timer0_millis = 0;
  interrupts();
}
