
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
      sprintf(dsc," %d : %s*C\t|\t%s*F",i+1,tempc,dsf);
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
  DateTime now = rtc.now();
  Serial.print((String)now.hour() + ":" + (String)now.minute() + ":" + (String)now.second() + String("\n"));
  Serial.print((String)daysOfTheWeek[now.dayOfTheWeek()] + String(",") + (String)now.day() + "/" + (String)now.month() + "/" + (String)now.year() + String("\n"));
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

void init_startup()
{
    Serial.begin(9600);
    // SD card
    Serial.print(F("\nInitializing SD card..."));
    if (!SD.begin(4))
    {
      Serial.println(F("initialization failed!"));
    }
    else{ Serial.println(F("initialization done.")); }
    // dùng cho ds18b20
    sensors_ds.begin();
    delay(10);
    deviceCount = sensors_ds.getDeviceCount();
    sprintf(extra,"\nĐang định vị ds18b20...tìm thấy: %d device",deviceCount);
    Serial.println(extra);
    // lệnh cho TDS
    gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
    gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
    gravityTds.begin();  //initialization
    // lệnh cho lcd
    lcd.begin();
    lcd.backlight();
    lcd.print("   start lcd");
    // khai báo chân
    pinMode(TdsSensorPin,INPUT);
    pinMode(38, OUTPUT);
    pinMode(40, OUTPUT);
    pinMode(42, OUTPUT);
    pinMode(44, OUTPUT);
    pinMode(46, OUTPUT);
    pinMode(48, OUTPUT);
    pinMode(13,OUTPUT);
    // dùng cho rtc
    Wire.begin();
//    if (! rtc.begin()) {  Serial.print("Không tìm thấy RTC"); lcd.clear();  lcd.print("Không tìm thấy RTC");   while (1); }
//    if (! rtc.isrunning())  {Serial.print("RTC không hoạt động\n");  lcd.clear();  lcd.print("RTC not run");}
// --------------???----------???-----------------------------------------------------
//    rtc.adjust(DateTime(2020, 8, 3, 7, 39, 0));
//    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
// --------------!!!------------!!!---------------------------------------------------
}
