#include <WiFiManager.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define RXD2 16
#define TXD2 17

char const* ssidname ="ESP32_WFCNF";
char const* ssidpass ="12345678";
const char* mqtt_server = "mohinhrauthuycanh.ddns.net";
IPAddress staticIP(192,168,1,146);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
IPAddress dns1(8,8,8,8);
IPAddress dns2(8,8,4,4);

WiFiClient espClient;
PubSubClient client(espClient);

String a,b;
unsigned long t2,t3,t4,t5;
int espcount=0;
void setup()
{
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  pinMode(2,OUTPUT);
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);
  WiFi.config(staticIP,gateway,subnet,dns1,dns2);
  while(WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    digitalWrite(2,LOW);
    WiFiManager wifiManager;
    wifiManager.autoConnect(ssidname,ssidpass);
  }
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop()
{
  while(WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    digitalWrite(2,LOW);
    WiFiManager wifiManager;
    wifiManager.autoConnect(ssidname,ssidpass);
    digitalWrite(2,HIGH);
  }
  if (!client.connected())  { reconnect(); }
  client.loop();
  while (Serial2.available()>0)
  {
    a += char(Serial2.read());
    delay(100);
    if(Serial2.available()==0)
    {
      delay(10);
      Serial.println(a);
      if(a=="MEGA2560")
      {
        Serial.println("MEGA đang hoạt động");
        t4=millis();
        a="";
        digitalWrite(4,HIGH);
        digitalWrite(2,HIGH);
        delay(500);
        digitalWrite(2,LOW);
        delay(500);
        digitalWrite(2,HIGH);
        delay(500);
        digitalWrite(2,LOW);
      }
      else
      {
        Serial.println("MEGA đang chờ tín hiệu");
        espcount +=1;
        if(espcount>=4)
        {
          digitalWrite(4,LOW);
          delay(30000);
          Serial.println("da bat relay");
          espcount=0;
        }
        a="";
      }
      a="";
    }
    delay(5);
    Serial.print(".");
  }
  if(millis()-t4>300000) // 5phut
  {
    while (Serial2.available()==0)
    {
      digitalWrite(12,LOW);
      delay(30000); // 30s
      Serial.println("da bat relay");
    }
    t4=millis();
  }
  if(millis()-t5>30000) // 1phut
  {
    Serial2.write("ESP32");
    Serial.println("da gui esp32");
    t5=millis();
  }
  
}
