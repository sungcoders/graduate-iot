#include <UIPEthernet.h>
#include <PubSubClient.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <avr/wdt.h>
#include <Wire.h>
#include "DHT.h"
#include "LCDSPI_SCDT.h"
#define   TdsPin A8
          OneWire onewire(37);
          DallasTemperature sensors_ds(&onewire);
          EthernetClient Ethclient;
          PubSubClient client(Ethclient);
DHT dht(39, DHT22);
// thông tin kết nối
const byte      mac[] = {0x54,0x34,0x41,0x30,0x30,0x35};
const uint8_t   IP[] = {192,168,1,198};
const uint8_t   dns1[] = {8,8,8,8};
const uint8_t   gateway[] = {192,168,1,1};
const uint8_t   subnetmsk[] = {255,255,255,0};
const char* mqtt_server = "mohinhrauthuycanh.ddns.net";
// ********************************** khai báo biến ***************************************//
int   deviceCount;                    // biến nhận số lượng ds18b20
byte  count_ar=0 ;                    // biến thời gian
char cvt_c[50];                       // biến lưu trữ kiểu char convert
unsigned long ts, tar, t_noti,t1,t2,t3,t4,t5;
// ********************************** khai báo hàm con ***************************************//
extern volatile unsigned long timer0_millis;
void(* resetFunc) (void) = 0;               //cài đặt hàm reset
void convertcharf(float fl1);
void convertcharfp(float fl1, float fl2);
void milirst();

// ------------------------------ setup ----------------------------------------
void setup()
{
  init_startup();
  delayMicroseconds(100);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  init_send_once();
}

void loop()
{
  wdt_enable(WDTO_8S);
  if (!client.connected())  { MQTTreconnect(); }
  client.loop();
  if(millis()-t1>=1000)
  {
    if(check_rtc()==1)
    {
      read_rtc();
      rtc_display();
    }
    else
    {
      lcd_print(0,0,"      RTC not run    ");
    }
    BH1750_display();
    DHT_read();
    ds18b20(1);
    t1=millis();
  }
  if(millis()-t2>=1700)
  {
    readTds();
    t2=millis();
  }
  if(millis()-t3>=2500)
  {
    YHDC30();
    t3=millis();
  }
  if(millis() >= 2592000000) //30 day
  {
    delayMicroseconds(500);
    milirst();
  }
  else  { ar_other(); }
  if(millis()-t5>60000) // 1phut
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
