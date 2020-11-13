
int check_ds18b20()
{
  int deviceCount;
  sensors_ds.begin();
  delay(10);
  deviceCount = sensors_ds.getDeviceCount();
  Serial.print(F("\nĐang tìm ds18b20...tìm thấy: "));
  Serial.print(deviceCount);
  Serial.println(F("device"));
  return (deviceCount);
}

void SetPinOut()
{
  for(int i=0; i<7; i++)
  {
    pinMode(pinout[i],OUTPUT);
    digitalWrite(pinout[i],HIGH);
  }
}

void init_startup()
{
  Serial.begin(9600);
  Serial2.begin(9600);
  Wire.begin();
  dht.begin();
  pinMode(TdsPin,INPUT);
  SetPinOut();
  start_lcd();
  check_ds18b20();
  (check_rtc()==1)?Serial.println(F("RTC Đang hoạt động.")):Serial.println(F("RTC failed!")),lcd_print(0,0,"      RTC not run    ");
//  set_time(18,54,50,11,05,0,4);
  read_rtc();
  rtc_display();
  lcd_print(19,4,"*C:");
}

void init_send_once()
{
  while((Ethernet.linkStatus()!=LinkON) || (!client.connected()) )
  {
    if(Ethernet.linkStatus()!=LinkON)    {   ethernet();       }
    if(!client.connected())              {   MQTTreconnect();  }
    delayMicroseconds(500);
    readTds();
    YHDC30();
  }
}

void send_ar()
{
  Serial.println(F("reset board after for 12h"));
  for (int i=6; i>=1; i--)
  {
    client.publish("arduino",num_to_char(i,"arduino taking a rest"));
    delay(TIME1*10);
  }
  resetFunc();
}

void ar_other()
{
  if((millis()-tar)>=TIME1*5)
  {
    dem+=1;
    client.publish("arduino",num_to_char(dem," Active"));
    tar = millis();
  }
  if(dem>=250) {dem=0;}
}
