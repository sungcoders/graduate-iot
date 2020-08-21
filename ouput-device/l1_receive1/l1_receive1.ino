#include <UIPEthernet.h>
#include <PubSubClient.h> 

byte mac[] = { 0x54, 0x34, 0x41, 0x30, 0x30, 0x36 };
uint8_t IP[]={192,168,1,250};
const char* mqtt_server = "192.168.1.121";
          EthernetClient Ethclient;
          PubSubClient client(Ethclient);
unsigned long long tdevice=0, tdevice1=0;
const int light = 10;
const int fan = 8;

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
    if(Ethernet.linkStatus() != LinkON)
        ethernet();
    if (!client.connected())  
        MQTTreconnect();
    client.loop();
  }








 
