
//--------------- send ds18b20 nhiet do--------------
void ds18b20() 
  {
    float temp = 20.2;               
    sensords.requestTemperatures();
    Serial.print("\n nhiet do : ");
    Serial.println(sensords.getTempCByIndex(0));           
    temp = (float)sensords.getTempCByIndex(0);
    if(20.0<=temp<=40.0)
      {
        Serial.println("nhiet do nam trong khoang cho phep");
        String a = String(temp);
        client.publish("nhiet do",(char*) a.c_str());
      }
    if(temp<20.0)
      {
        Serial.println("nhiet do duoi nguong cho phep");
        client.publish("warn1","nhiet do qua thap");
      }  
    if(temp>40.0)
      {
        Serial.println("nhiet do qua muc cho phep");
        client.publish("warn1","nhiet do qua cao");
      }
    else 
      {
        Serial.println("thiet bi co van de");
        client.publish("warm1","thiet bi co van de");
      }
  }

//----------------------------do am DHT11-----------------------------

void DHT11sensor()
  {
    byte h = dht.readHumidity();
    byte t = dht.readTemperature();
    char cth[10];                                                           // bien chuyen doi cho h
    char ctt[10];                                                           // bien chuyen doi cho h
    if (t<=0 || h<=0) 
      {
        Serial.println("Failed to read from DHT");
        client.publish("monitor/warning","sensor misstake");
        lcd.clear();
        lcd.print("    miss");
        lcd.setCursor(3,1);
        lcd.print(" sent H");
      } 
    else
      {
        Serial.println("Humidity: " + String(h) + String(" %\t") + String(" &&    ") + String("Temperature: ") + String(t) + String(" degree"));
        sprintf(cth,"%d percent",h);
        sprintf(ctt,"%d degree",t);
        client.publish("monitor/humidity",cth);
        client.publish("monitor/warning"," system sensor t*C");
        Serial.println("sent humidity :"+(String)cth);
        lcd.clear();
        lcd.print("   send data");
        lcd.setCursor(3,1);
        lcd.print(" sent T");
        delay(1000);
        client.publish("monitor/temperature",ctt);
        client.publish("monitor/warning"," system sensor h%");
        Serial.println("send temperature :"+(String)ctt);
        lcd.clear();
        lcd.print("   send data");
        lcd.setCursor(3,1);
        lcd.print(" sent H");
        delay(500);
      }
  }// end DHTTsensor


  /*  Serial.print("gia tri char a : " + String(a));
      Serial.print("\n sau khi chuyen doi :" );
   *  Serial.println((char*) a.c_str());
   * 
   */
