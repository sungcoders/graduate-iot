
void multi_ds18b20()
{
  // gioi han nhiet do
  int limit_over = 50;
  int limit_under = 25;
  // khai bao
  float tempC,tempF;                   // gia tri *C va *F
  char tempc[10],tempf[10];            // cac bien tam thoi *C va *F
  char dsc[20],dsf[10];                // cac bien luu ket qua cuoi cung
  const char ds18b20[10];              // cac MQTT topic
  sensors_ds.requestTemperatures();
  for (int i = 0;  i < deviceCount;  i++)
  {
    tempC = sensors_ds.getTempCByIndex(i);
    if(limit_under<=tempC<=limit_over)
    {
      String tempx=(String)tempC;
      tempx.toCharArray(tempc,tempx.length()+1);
      tempF=DallasTemperature::toFahrenheit(tempC);
      String tempf=(String)tempF;
      tempf.toCharArray(dsf,tempf.length()+1);
      sprintf(dsc,"%s*C\t|\t%s*F",tempc,dsf);
      sprintf(ds18b20,"ds18b20_%d",i+1);
      client.publish(ds18b20,dsc);
      Serial.println(dsc);
    }
    else if(tempC<limit_under)
    {
      sprintf(ds18b20,"ds18b20_%d",i+1);
      client.publish(ds18b20,"nhiet do qua thap");
    }
    else
    {
      sprintf(ds18b20,"ds18b20_%d",i+1);
      client.publish(ds18b20,"nhiet do qua cao");
    }
  }
}

// ---------------------- RTC-----------------------------
void rtcds1307() 
{
  char hhmmss_c[20],ddmmyy_c[20], rtc_time1[40],rtc_time2[40];
  DateTime now = rtc.now();
  String hhmmss = String( (String)now.hour() + "h:" + (String)now.minute() + "m:" + (String)now.second() + String("s"));
  hhmmss.toCharArray(hhmmss_c,hhmmss.length()+1);
  String ddmmyy = String( (String)daysOfTheWeek[now.dayOfTheWeek()] + String(", ") + (String)now.day() + "/" + (String)now.month() + "/" + (String)now.year() );
  ddmmyy.toCharArray(ddmmyy_c,ddmmyy.length()+1);
  Serial.print((String)now.hour() + ":" + (String)now.minute() + ":" + (String)now.second() + String("\n"));
  Serial.print((String)daysOfTheWeek[now.dayOfTheWeek()] + String(",") + (String)now.day() + "/" + (String)now.month() + "/" + (String)now.year() + String("\n"));
  sprintf(rtc_time1,"%s",ddmmyy_c);
  sprintf(rtc_time2,"%s",hhmmss_c);
  Serial.print("ddmmyy: ");
  Serial.println(rtc_time1);
  Serial.println(hhmmss);
  client.publish("ddmmyy_mqtt",rtc_time1);
  client.publish("hhmmss_mqtt",rtc_time2);
}

void tds()
{
  byte k=1;
  while(k<=10)
  {
    //temperature = readTemperature();  //add your temperature sensor and read it
    gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate 
    tdsValue = gravityTds.getTdsValue();  // then get the value
    sum_tds += (int)tdsValue;
    if(k>=10)
    {
      sum_tds = (int)(sum_tds/10);
      if(sum_tds<=0)
      {
        client.publish("TDS_sensor1","TDS quá Thấp");
      }
      else if(sum_tds>=1000)
      {
        client.publish("TDS_sensor1","TDS quá cao");
      }
      char tds_value[10];
      sprintf(tds_value,"%d ppm",sum_tds);
      client.publish("TDS_sensor1",tds_value);
      Serial.print(tds_value);
      Serial.println();
    }
    k++;
    delay(10);
  }
}


void sd_card()
{
  f = SD.open("test.txt", FILE_WRITE);
  if (f)
  {
    Serial.print(F("Writing to test.txt..."));
    f.println("testing 1, 2, 3.");
    f.close();
    Serial.println("done.");
  }
  else
  {
    Serial.println(F("error opening test.txt"));
  }

  f = SD.open("test.txt");
  if (f)
  {
    Serial.println("test.txt:");
    while (f.available())
    {
      Serial.write(f.read());
    }
    f.close();
  }
  else
  {
    Serial.println(F("error opening test.txt"));
  }
}

void YHDC100()
{
  char irms_c[15];
  float Irmsf = emon.calcIrms(1480);
  Irmsf = Irmsf*230.0;
  String irmsf = (String)Irmsf;
  irmsf.toCharArray(irms_c,irmsf.length()+1);
  char YH[15];
  sprintf(YH,"%s mA",irms_c);
  client.publish("YHDC100",YH);
  Serial.print("Dòng điện: ");
  Serial.println(YH);
}
