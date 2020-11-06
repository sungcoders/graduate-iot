
int check_ds18b20()
{
  char extra[50];
  sensors_ds.begin();
  delay(10);
  deviceCount = sensors_ds.getDeviceCount();
  sprintf(extra,"\nĐang tìm ds18b20...tìm thấy: %d device",deviceCount);
  Serial.println(extra);
  lcd_hello(deviceCount);
  return (deviceCount);
}

void init_startup()
{
  Serial.begin(9600);
  Serial2.begin(9600);
  Wire.begin();
  dht.begin();
  // khai báo chân
  pinMode(TdsPin,INPUT);
  pinMode(38, OUTPUT);
  pinMode(40, OUTPUT);
  pinMode(42, OUTPUT);
  pinMode(44, OUTPUT);
  pinMode(46, OUTPUT);
  pinMode(48, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(38,LOW);
  digitalWrite(40,LOW);
  digitalWrite(42,LOW);
  digitalWrite(44,LOW);
  digitalWrite(46,LOW);
  digitalWrite(48,LOW);
  start_lcd();
  check_ds18b20();
  if(check_rtc()==1) {Serial.println(F("RTC Đang hoạt động.")); lcd_print(0,0,"      RTC not run    ");} else {Serial.println(F("RTC failed!"));}
//  set_time(18,54,50,11,05,0,4);
  read_rtc();
  rtc_display();
  lcd_print(19,4,"*C:");
}

// ************************* khoi tao ket noi ban dau ************************
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
  client.publish("arduino","arduino taking a rest");
  delay(20000);
  client.publish("arduino","arduino taking a rest 1");
  delay(10000);
  client.publish("arduino","arduino taking a rest 2");
  delay(15000);
  client.publish("arduino","arduino taking a rest 3");
  delay(1000);
  resetFunc();
}

void ar_other()
{
  if((millis()-tar)>=5001ul)
  {
    char ar[50];
    sprintf(ar,"Active [ %d ]",count_ar++);
    client.publish("arduino",ar);
    tar = millis();
  }
  if(count_ar>=250)
  {
    count_ar=0;
  }
}