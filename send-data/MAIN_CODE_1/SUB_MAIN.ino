
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
  for(int i=0; i<7; i++)
  {
    pinMode(pinout[i],OUTPUT);
    digitalWrite(pinout[i],HIGH);
  }
}

void init_send_once()
{
  (!client.connected())?MQTTreconnect():client.loop();
  delayMicroseconds(500);
  BH1750_display();
  readTdsQuick();
  DHT_read();
  ds18b20(1);
  ds18b20(2);
  YHDC30();
}

void take_A_rest()
{
  for (int i=6; i>=1; i--)
  {
    client.publish("arduino",num_to_char(i,"arduino taking a rest"));
    delay(TIME01S*10);
  }
  resetFunc();
}

void send_arduino_loop()
{
  if((millis()-tar)>=TIME01S*5)
  {
    dem+=1;
    client.publish("arduino",num_to_char(dem," Active"));
    tar = millis();
  }
  if(dem>=250) {dem=0;}
}
