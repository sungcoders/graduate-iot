
void multi_ds18b20()
{
  // gioi han nhiet do
  int limit_over = 50;
  int limit_under = 25;
  // khai bao
  float tempC,tempF;                   // gia tri *C va *F
  char tempc[10],tempf[10];            // cac bien tam thoi *C va *F
  char dsc[20],dsf[10];                // cac bien luu ket qua cuoi cung
  const char ds18b20[20];              // cac MQTT topic
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
      sprintf(dsc,"Sensor %d : %s *C\t|\t%s *F",i+1,tempc,dsf);
      sprintf(ds18b20,"ds18b20_%d",i+1);
      client.publish(ds18b20,dsc);
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
  DateTime now = rtc.now();
  Serial.print((String)now.hour() + ":" + (String)now.minute() + ":" + (String)now.second() + String("\n"));
  Serial.print((String)daysOfTheWeek[now.dayOfTheWeek()] + String(",") + (String)now.day() + "/" + (String)now.month() + "/" + (String)now.year() + String("\n"));
}

void sd_card()
{
  f = SD.open("test.txt", FILE_WRITE);
  if (f)
  {
    Serial.print("Writing to test.txt...");
    f.println("testing 1, 2, 3.");
    f.close();
    Serial.println("done.");
  }
  else
  {
    Serial.println("error opening test.txt");
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
    Serial.println("error opening test.txt");
  }
}
