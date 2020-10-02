
//------------------------enthernet-----------------------------------
void ethernet() 
{
  Serial.print("connecting ethernet :");
  lcd.clear();
  lcd.print("conneting E ...");
  Ethernet.begin(mac,IP);
  while(Ethernet.linkStatus()!=LinkON)
  {                                       //https://www.arduino.cc/en/Reference/EthernetLinkStatus
    for(int i=1; i<=20; i++)
      {
        Serial.print("Failed ethernet-waiting 3s, times: " + String(i));
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
            Serial.println("Kiểm tra dây bus của bạn ???");
            lcd.clear();
            lcd.print(" E & M connect !");
            lcd.setCursor(0,1);
            lcd.print("check divice's ?");
          }
        else if (Ethernet.linkStatus() == LinkON) 
          {
            Serial.println("Kết nối Ethernet thành công !\n");
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("status: link ON");
            break;
          } 
        else if (Ethernet.linkStatus() == LinkOFF)
          {
            Serial.println("Kết nối Ethernet thất bại !\n");
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("status: link ON");
          }
        if(i>=20)
          {
            Serial.println("Reset lỗi Ethernet !");
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("reset by E miss");
            delay(100);
            resetFunc();                                // ham reset
          } 
      }// end for
  } // end while
    
  Serial.println("Ethernet Thành công");
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
        Serial.println("Số lần kết nối MQTT: " + String(j));
        lcd.clear();
        lcd.print(" E & M connect !");
        lcd.setCursor(0,1);
        lcd.print("MQTT ... L");
        lcd.setCursor(11,1);
        lcd.print(j);
        Serial.print("Kết nối lại MQTT...");
        if(client.connect("arduinoClient")) 
          {
            Serial.println("Đã kết nối MQTT");
            lcd.clear();
            lcd.print(" E & M connect !");
            lcd.setCursor(0,1);
            lcd.print("MQTT Thành Công");
            client.subscribe("mega1/#");
          }
        else 
          {
            Serial.print("Thất bại, lỗi :" + String(client.state()) + String("\nthử lại lần nữa \n"));
            lcd.clear();
            lcd.print(" E & M connect !");
            lcd.setCursor(0,1);
            lcd.print("fail MQTT...4s");
            delay(4000);
          }
        if(j>=15) 
          {
            Serial.println("reset lỗi MQTT !");
            lcd.clear();
            lcd.print(" E & M connect !");
            lcd.setCursor(0,1);
            lcd.print("reset by M miss");
            delay(500);
            resetFunc();
          }
      } //end while
    Serial.println("MQTT thành công");
  } //end MQTTconnect

//---------------------- callback from MQTT -------------------------------
void callback(char* topic, byte* message, unsigned int length)
  {
    Serial.print("tin nhắn đến (topic): ");
    Serial.print(topic);
    Serial.print(".\tnội dung: ");
    String messageTemp;
  
    for (int i = 0; i < length; i++)
      {
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
      }
    if((String)topic=="room/lamp")
      {                    // room/lamp == relay1      room/relay2 == relay2
        Serial.print("Changing Room lamp to ");
        if(messageTemp == "on")
          {
            digitalWrite(13, HIGH);
            Serial.print(" led8 On");
          }
        else if(messageTemp == "off")
          {
            digitalWrite(13, LOW);
            Serial.print("led8 Off");
          }
  }// end if
    Serial.println();
  }// end callback
