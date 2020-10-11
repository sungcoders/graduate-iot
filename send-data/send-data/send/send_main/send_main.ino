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
EnergyMonitor emon;
#include "GravityTDS.h"
#define   TdsSensorPin A1
GravityTDS gravityTds;
#define   VREF 5.0      // analog reference voltage(Volt) of the ADC
#define   SCOUNT  30           // sum of sample point
// thông tin kết nối
byte        mac[] = { 0x54, 0x34, 0x41, 0x30, 0x30, 0x35 };
uint8_t     IP[] = {192, 168, 1, 199};
const char* mqtt_server = "192.168.1.111";
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
unsigned long tsensor = 0, ts, t_tds;       // ts
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
  if (Ethernet.linkStatus() != LinkON)    {
    ethernet();
  }
  if (!client.connected())              {
    MQTTreconnect();
  }
  if (!client.loop())                   {
    client.connect("arduinoClient");
  }
  if ((millis() - tsensor) >= 5000)
  {
    multi_ds18b20();
    sd_card();
    client.publish("caytrong", "cay_trong");
    tsensor = millis();
  }
  if (millis() - t_tds > 600000)
  {
    YHDC100();
    tds();
    t_tds = millis();
  }
  if ((millis() - ts) >= 1000)
  {
    ts = millis();
    rtcds1307();
  }
  DateTime now = rtc.now();
  if ((now.hour() == 14 || now.hour() == 2) && now.minute() == 00)
  {
    send_ar();
  }
  else
  {
    ar_other();
  }
  if (millis() >= 2592000000) //30 day
  {
    tsensor = 0;
    milirst();
  }
} //end loop

void milirst()
{
  noInterrupts();
  timer0_millis = 0;
  interrupts();
}
