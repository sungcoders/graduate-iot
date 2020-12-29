
int check_ds18b20()
{
  int deviceCount;
  sensors_ds.begin();
  delay(10);
  deviceCount = sensors_ds.getDeviceCount();
  return (deviceCount);
}

void SetPinOut()
{
  for (int i = 0; i < 7; i++)
  {
    pinMode(pinout[i], OUTPUT);
    digitalWrite(pinout[i], HIGH);
  }
}

void init_startup()
{
  Serial2.begin(9600);
  Wire.begin();
  dht.begin();
  SetPinOut();
  start_lcd();
  check_ds18b20();
  (check_rtc() == 1) ? lcd_print(0, 0, "      RTC run ok     ") : lcd_print(0, 0, "      RTC not run    ");
  //  set_time(10,28,50,15,11,0,4);
  read_rtc();
  rtc_display();
  lcd_print(19, 4, "*C:");
}

void init_send_once()
{
  while ((Ethernet.linkStatus() != LinkON) || (!client.connected()) )
  {
    (!client.connected()) ? MQTTreconnect() : client.loop();
    delayMicroseconds(500);
    BH1750_display();
    readTdsQuick();
    DHT_read();
    ds18b20(1);
    ds18b20(2);
    YHDC30();
  }
}
