
//------------------------enthernet-----------------------------------
void ethernet() 
{
  Serial.print("connecting ethernet :");
  lcd.clear();
  lcd.print("conneting E ...");
  Ethernet.begin(mac,IP);
  while(Ethernet.linkStatus()!=LinkON)
  {                                       //https://www.arduino.cc/en/Reference/EthernetLinkStatus
    Serial.println(Ethernet.linkStatus());
    for(int i=1; i<=20; i++)
      {
        Serial.print("Failed ethernet-waiting for 3s, times: " + String(i));
        lcd.clear();
        lcd.print(" E & M connect !");
        lcd.setCursor(0,1);
        lcd.print("fail..3s..l");
        lcd.print(i);
        delay(3000);
        Ethernet.begin(mac,IP);
        delay(200);
        if (Ethernet.linkStatus() == Unknown) 
          {
            Serial.println("check for your devices ??? ");
            lcd.clear();
            lcd.print(" E & M connect !");
            lcd.setCursor(0,1);
            lcd.print("check divice's ?");
          }
        else if (Ethernet.linkStatus() == LinkON) 
          {
            Serial.println("enthernet connected Link status: On \n");
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("status: link ON");
            break;
          } 
        else if (Ethernet.linkStatus() == LinkOFF)
          {
            Serial.println("Failed ethernet: link status = off \n");
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("status: link ON");
          }
        if(i>=20)
          {
            Serial.println("reset because Ethernet misstake ");
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("reset by E miss");
            delay(500);
            resetFunc();                                // ham reset
          } 
      }// end for
  } // end while
    
  Serial.println("ethernet connected");
  lcd.clear();
  lcd.print(" E & M connect !");
  lcd.setCursor(0,1);
  lcd.print("connected E");
  Serial.print("IP Address        : ");
  Serial.println(Ethernet.localIP());
  Serial.print("Subnet Mask       : ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("Default Gateway IP: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("DNS Server IP     : ");
  Serial.println(Ethernet.dnsServerIP()); 
} //end ethetnet

// --------------------------MQTT------------------------------
  
void MQTTreconnect() 
  {
    int j=0;
    while(!client.connected()) 
      {
        j++;
        Serial.println("\nProgram of MQTT");
        Serial.println("count for connect MQTT: "+ String(j));
        lcd.clear();
        lcd.print(" E & M connect !");
        lcd.setCursor(0,1);
        lcd.print("MQTT ... L");
        lcd.setCursor(11,1);
        lcd.print(j);
        Serial.print("connecting again MQTT...");
        if(client.connect("arduinoClient")) 
          {
            Serial.println("client connected MQTT");
            lcd.clear();
            lcd.print(" E & M connect !");
            lcd.setCursor(0,1);
            lcd.print("MQTT connected");
          }
        else 
          {
            Serial.print("failed, src=" + String(client.state()) + String("\ntry again for 4s \n"));
            lcd.clear();
            lcd.print(" E & M connect !");
            lcd.setCursor(0,1);
            lcd.print("fail MQTT...4s");
            delay(4000);
          }
        if(j>=15) 
          {
            Serial.println("reset board because MQTT misstake !!");
            lcd.clear();
            lcd.print(" E & M connect !");
            lcd.setCursor(0,1);
            lcd.print("reset by M miss");
            delay(500);
            resetFunc();
          }
      } //end while
    Serial.println("mqtt connected");
  } //end MQTTconnect
