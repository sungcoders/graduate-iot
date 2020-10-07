
//------------------------enthernet-----------------------------------
void ethernet() 
{
  Serial.print("connecting ethernet :\n");
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
            Serial.println("  Kiểm tra dây bus của bạn ???");
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
            client.subscribe("mega1/sub");
            client.subscribe("van1");
            client.subscribe("van2");
            client.subscribe("dc1");
            client.subscribe("dc2");
            client.subscribe("ac1");
            client.subscribe("ac2");
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
    for (byte i = 0; i < length; i++)
      {
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
      }
    if((String)topic=="van1")
      {
        Serial.print("\nĐiều Khiển van 1 :");
        if(messageTemp == "on_van1")
          {
            digitalWrite(40, HIGH);
            Serial.print(" led gpio 40 On");
          }
        else if(messageTemp == "off_van1")
          {
            digitalWrite(40, LOW);
            Serial.print("led gpio 40 Off");
          }
        else {Serial.println("check your on payload topic MQTT");}
      }// end if
    if((String)topic=="van2")
      {
        Serial.print("\nĐiều Khiển Van2: ");
        if(messageTemp == "on_van2")
          {
            digitalWrite(38, HIGH);
            Serial.print(" led gpio 38 On");
          }
        else if(messageTemp == "off_van2")
          {
            digitalWrite(38, LOW);
            Serial.print("led gpio 38 Off");
          }
        else {Serial.println("check your on payload topic MQTT");}
      }// end if
    if((String)topic=="dc1")
      {
        Serial.print("\nĐiều Khiển Van2: ");
        if(messageTemp == "on_dc1")
          {
            digitalWrite(46, HIGH);
            Serial.print(" led gpio 46 On");
          }
        else if(messageTemp == "off_dc1")
          {
            digitalWrite(46, LOW);
            Serial.print("led gpio 46 Off");
          }
        else {Serial.println("check your on payload topic MQTT");}
      }// end if
    if((String)topic=="dc2")
      {
        Serial.print("\nĐiều Khiển dc2: ");
        if(messageTemp == "on_dc2")
          {
            digitalWrite(48, HIGH);
            Serial.print(" led gpio 48 On");
          }
        else if(messageTemp == "off_dc2")
          {
            digitalWrite(48, LOW);
            Serial.print("led gpio 48 Off");
          }
        else {Serial.println("check your on payload topic MQTT");}
      }// end if
    if((String)topic=="ac1")
      {
        Serial.print("\nĐiều Khiển động cơ ac1: ");
        if(messageTemp == "on_ac1")
          {
            digitalWrite(42, HIGH);
            Serial.print(" led gpio 42 On");
          }
        else if(messageTemp == "off_ac1")
          {
            digitalWrite(42, LOW);
            Serial.print("led gpio 42 Off");
          }
        else {Serial.println("check your on payload topic MQTT");}
      }// end if
    if((String)topic=="ac2")
      {
        Serial.print("\nĐiều Khiển động cơ ac2: ");
        if(messageTemp == "on_ac2")
          {
            digitalWrite(44, HIGH);
            Serial.print(" led gpio 44 On");
          }
        else if(messageTemp == "off_ac2")
          {
            digitalWrite(44, LOW);
            Serial.print("led gpio 44 Off");
          }
        else {Serial.println("check your on payload topic MQTT");}
      }// end if
    Serial.println();
  }// end callback
