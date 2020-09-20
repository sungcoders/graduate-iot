
//------------------------enthernet-----------------------------------

void ethernet() 
  {
    Serial.print("connecting ethernet :");
    Ethernet.begin(mac,IP);
    while(Ethernet.linkStatus()!=LinkON)
      {                                       //https://www.arduino.cc/en/Reference/EthernetLinkStatus
        for(int i=1; i<=20; i++)
          {
            Serial.print("Failed ethernet-waiting for 3s, times: " + String(i));
            delay(3000);
            Ethernet.begin(mac,IP);
            delay(200);
            if (Ethernet.linkStatus() == Unknown)
              {
                Serial.println("   check for your devices ??? ");
              }
            else if (Ethernet.linkStatus() == LinkON)
              {
                Serial.println("   enthernet connected Link status: On \n");
                break;
              }
            else if (Ethernet.linkStatus() == LinkOFF)
              {
                Serial.println("   Failed ethernet: link status = off \n");
              }
            if(i>=20)
              {
                Serial.println("reset because Ethernet misstake ");
                delay(500);
                resetFunc();                                // ham reset
              } 
          }// end for
      } // end while
    
    Serial.println("ethernet connected");
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
        Serial.print("connecting again MQTT...");
        if(client.connect("arduinoClient"))
          {
            Serial.println("client connected MQTT");
            client.subscribe("mega1/#");
            client.subscribe("room/relay2");
            delay(200);
            break;
          }
        else 
          {
            Serial.print("failed, src=" + String(client.state()) + String("\ntry again for 4s \n"));
            delay(4000);
          }
        if(j>=15)
          {
            Serial.println("reset board because MQTT misstake !!");
            delay(500);
            resetFunc();
          }
        
      } //end while
  } //end MQTTconnect



//---------------------- callback from MQTT -------------------------------
void callback(char* topic, byte* message, unsigned int length)
  {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(".   Message: ");
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

    if((String)topic=="room/relay2")
      {
        Serial.print("Changing Room lamp to ");
        if(messageTemp == "on1")
          {
            digitalWrite(8, HIGH);
            Serial.print(" led13 On");
          }
        else if(messageTemp == "off1")
          {
           digitalWrite(8, LOW);
           Serial.print("led13 Off");
          }
      }
    Serial.println();
  }// end callback
