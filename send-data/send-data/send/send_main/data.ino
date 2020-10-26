
void multi_ds18b20()
{
  // gioi han nhiet do
  float limit_over = 50;
  float limit_under = 25;
  // khai bao
  float tempC,tempF;                   // gia tri *C va *F
  const char ds18b20[10];              // cac MQTT topic
  sensors_ds.requestTemperatures();    // deviceCount
  for (int i = 0;  i < 8;  i++)
  {
    tempC = sensors_ds.getTempCByIndex(i);
    if(limit_under<=tempC<=limit_over)
    {
      convertcharf(tempC);
      client.publish(ds18b20,cvt_c);
      Serial.println(cvt_c);
    }
    else if(tempC<limit_under)
    {
      sprintf(ds18b20,"ds18b20_%d",i+1);
      client.publish(ds18b20,"nhiet do qua thap");
      Serial.println(F("nhiet do qua thap"));
    }
    else
    {
      sprintf(ds18b20,"ds18b20_%d",i+1);
      client.publish(ds18b20,"nhiet do qua cao");
      Serial.println(F("nhiet do qua cao"));
    }
    delayMicroseconds(500);
  }
}

// ---------------------- RTC-----------------------------
void rtcds1307()
{
  DateTime now = rtc.now();
  if(now.hour()>24 || now.minute()>=60 || now.second()>=60)
  {
    Serial.println("Sai Giờ Phút Giây");
  }
  else if(now.day()>31 || now.month()>12)
  {
    Serial.println("Sai Ngày Tháng");
  }
  else
  {
    Serial.println(String("\n") + (String)now.hour() + ":" + (String)now.minute() + ":" + (String)now.second());
    Serial.println((String)daysOfTheWeek[now.dayOfTheWeek()] + String(",") + (String)now.day() + "/" + (String)now.month() + "/" + (String)now.year() + String("\n"));
  }
}

void tds()
{
  float temperature = 25, tdsValue = 0, sum_tds = 0;
  for(byte k=1; k<=5; k++)
  {
    gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate 
    tdsValue = gravityTds.getTdsValue();  // then get the value
    sum_tds += (int)tdsValue;
    delayMicroseconds(200);
  }
  sum_tds = (float)(sum_tds/5.0);
  if(sum_tds<=0.0)
  {
    client.publish("TDS_sensor1","TDS quá Thấp");
    Serial.println(F("TDS quá thấp!"));
  }
  else if(sum_tds>=1000.0)
  {
    client.publish("TDS_sensor1","TDS quá cao");
    Serial.println(F("TDS quá cao!"));
  }
  else
  {
    convertcharf(sum_tds);
    client.publish("TDS_sensor1",cvt_c);
  }
}

void YHDC100()
{
  float Dongdien,sensor,Congsuat,ondac=0;
  long retardo=millis();
  int s=0;
  while(millis()-retardo<500)
  {
    sensor = analogRead(pinSCT) * (1.115 / 1023.0);
    Dongdien=sensor*0.4;
    ondac=ondac+sq(Dongdien);
    s=s+1;
    delay(1);
  }
  delay(1);
  ondac=ondac*2.0;
  Dongdien=sqrt((ondac)/s);
  Congsuat=Dongdien*230.0;
  Serial.print("Dong Dien: ");
  Serial.print(Dongdien);
  Serial.print("A   Cong suat: ");
  Serial.print(Congsuat);
  Serial.println("W");
  convertcharfp(Congsuat,Dongdien);
  client.publish("YHDC100",cvt_c);
}
