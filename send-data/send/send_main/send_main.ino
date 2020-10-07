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
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

#include <DallasTemperature.h>
#include <OneWire.h>
#define   ONE_WIRE_BUS 3
          OneWire onewire(ONE_WIRE_BUS);
          DallasTemperature sensors_ds(&onewire);
#include "GravityTDS.h"
#define   TdsSensorPin A1
          GravityTDS gravityTds;
#define   VREF 5.0      // analog reference voltage(Volt) of the ADC
#define   SCOUNT  30           // sum of sample point
            // thông tin kết nối
byte        mac[] = { 0x54, 0x34, 0x41, 0x30, 0x30, 0x35 };
uint8_t     IP[]={192,168,1,67};
const char* mqtt_server = "192.168.1.11";                             
            EthernetClient Ethclient;
            PubSubClient client(Ethclient);
            // ********************************** khai báo biến ***************************************//
int         deviceCount;                    // biến nhận số lượng ds18b20
byte        countMQTT=0;                    // biến đếm MQTT
char        extra[100];                     // biến lưu tạm thời toàn cục
            // ********************************** khai báo TDS ***************************************//
//int         analogBuffer[SCOUNT];    // store the analog value in the array, read from ADC
//int         analogBufferTemp[SCOUNT];
//int         analogBufferIndex = 0,copyIndex = 0;
//float       averageVoltage = 0,tdsValue = 0,temperature = 25;
int         a=0,y=0;
//byte        x=0;
float temperature = 25,tdsValue = 0;
            // ********************************** khai báo hàm con ***************************************//
extern volatile unsigned long timer0_millis;
unsigned long tsensor=0, ts;                    
void(* resetFunc) (void) = 0;               //cài đặt hàm reset

// ------------------------------setup------------------------------------------

void setup() 
{
    Serial.begin(9600);
    // SD
    Serial.print("\nInitializing SD card...");
    if (!SD.begin(4))
    {
      Serial.println("initialization failed!");
    }
    else{ Serial.println("initialization done."); }
    // dùng cho ds18b20
    sensors_ds.begin();
    delay(10);
    deviceCount = sensors_ds.getDeviceCount();
    sprintf(extra,"\nĐang định vị ds18b20...tìm thấy: %d device",deviceCount);
    Serial.println(extra);
    // lệnh cho TDS
    gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
    gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
    gravityTds.begin();  //initialization
    // lệnh cho lcd
    lcd.begin();
    lcd.backlight();
    lcd.print("   start lcd");
    // khai báo chân
    pinMode(TdsSensorPin,INPUT);
    pinMode(38, OUTPUT);
    pinMode(40, OUTPUT);
    pinMode(42, OUTPUT);
    pinMode(44, OUTPUT);
    pinMode(46, OUTPUT);
    pinMode(48, OUTPUT);
    pinMode(13,OUTPUT);
    // dùng cho rtc
           multi_ds18b20();
    Wire.begin();
//    if (! rtc.begin()) {  Serial.print("Không tìm thấy RTC"); lcd.clear();  lcd.print("Không tìm thấy RTC");   while (1); }
//    if (! rtc.isrunning())  {Serial.print("RTC không hoạt động\n");  lcd.clear();  lcd.print("RTC not run");}
// --------------???----------???-----------------------------------------------------
//    rtc.adjust(DateTime(2020, 8, 3, 7, 39, 0));
//    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
// --------------!!!------------!!!---------------------------------------------------
    delay(100);
    // thiết lập kết nối server
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

//-----------------------------------------loop---------------------------------

void loop()
{
   if(Ethernet.linkStatus()!=LinkON)    {   ethernet();       }
   if(!client.connected())              {   MQTTreconnect();  }
   if(!client.loop())                   {   client.connect("arduinoClient"); }
   if((millis()-tsensor)>=5000)
    {
      multi_ds18b20();
      sd_card();
      client.publish("mega1/pub","day la mega");
      client.subscribe("mega1/sub");
      tsensor=millis();
    }
    if(millis()-a>3000)
    {
      tds();
    }
//   if((millis()-ts)>=1000)
//    {
//    ts=millis();
//    rtcds1307();
//    }
//   DateTime now = rtc.now();
//   if((now.hour()==14 || now.hour()==2) && now.minute()==00)
//    {
//      Serial.println("reset board after for 12h");
//      client.publish("arduino","arduino taking a rest");
//      lcd.clear();
//      lcd.print("reset board 12h");
//      lcd.setCursor(0,1);
//      lcd.print("reset board 12h");
//      delay(20000);
//      client.publish("arduino","arduino taking a rest");
//      delay(10000);
//      client.publish("arduino","arduino taking a rest");
//      delay(15000);
//      client.publish("arduino","arduino taking a rest");
//      delay(1000);
//      resetFunc();
//    }
    if(millis() >= 2592000000)  //30 day
    {
      tsensor =0;
      milirst();
    }
} // end loop


void milirst() 
{
  noInterrupts();
  timer0_millis = 0;
  interrupts();
}
