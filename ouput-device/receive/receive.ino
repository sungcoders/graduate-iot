#include <UIPEthernet.h>
#include <PubSubClient.h> 

byte        mac[] = { 0x54, 0x34, 0x41, 0x30, 0x30, 0x36 };
uint8_t     IP[]={192,168,1,250};
const char* mqtt_server = "192.168.1.111";
            EthernetClient Ethclient;
            PubSubClient client(Ethclient);
unsigned long long tdevice=0;
const int          light = 10;
const int          fan = 8;

extern volatile unsigned long timer0_millis;

void(* resetFunc) (void) = 0;//cài đặt hàm reset
void MQTTreconnect();
void callback(char* topic, byte* message, unsigned int length);
          
void setup() 
{
    Serial.begin(9600);
    pinMode(8,OUTPUT);
    pinMode(13,OUTPUT);
    delay(300);
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    Serial.println("\nserial started in setup");
}

void loop() 
{
   if(Ethernet.linkStatus()!=LinkON)    {   ethernet();       }
   if(!client.connected())              {   MQTTreconnect();  }
   if(!client.loop())                   {   client.connect("arduinoClient"); }
   
    if((millis()-tdevice) >=10000)
    {
      client.publish("mega1/pub","day la mega1");
      tdevice = millis();
      Serial.println("sent data");
    }
    if(millis() >= 2592000000)  //30 day
    {
      tdevice =0;
      milirst();
    }
  }

void milirst() 
{
  noInterrupts();
  timer0_millis = 0;
  interrupts();
}






 
