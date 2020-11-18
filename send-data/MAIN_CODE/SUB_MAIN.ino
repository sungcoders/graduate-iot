
int check_ds18b20()
{
  int deviceCount;
  sensors_ds.begin();
  delay(10);
  deviceCount = sensors_ds.getDeviceCount();
  //Serial.print(F("\nĐang tìm ds18b20...tìm thấy: "));
  //Serial.print(deviceCount);
  //Serial.println(F("device"));
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
  SetPinOut();
  start_lcd();
  check_ds18b20();
  (check_rtc()==1)?lcd_print(0,0,"      RTC run ok     "):lcd_print(0,0,"      RTC not run    ");
//  set_time(10,28,50,15,11,0,4);
  read_rtc();
  rtc_display();
  lcd_print(19,4,"*C:");
}

void init_send_once()
{
  while((Ethernet.linkStatus()!=LinkON) || (!client.connected()) )
  {
    (!client.connected())?MQTTreconnect():client.loop();
    delayMicroseconds(500);
    readTdsQuick();
    DHT_read();
    ds18b20(1);
    ds18b20(2);
    YHDC30();
  }
}

void take_A_rest()
{
  //Serial.println(F("reset board after for 12h"));
  for (int i=6; i>=1; i--)
  {
    client.publish("arduino",num_to_char(i,"arduino taking a rest"));
    delay(TIME01S*10);
  }
  resetFunc();
}

void ar_other()
{
  if((millis()-tar)>=TIME01S*5)
  {
    dem+=1;
    client.publish("arduino",num_to_char(dem," Active"));
    tar = millis();
  }
  if(dem>=250) {dem=0;}
}
