
void init_startup()
{
  Serial.begin(9600);
  // khai báo chân
  pinMode(TdsSensorPin,INPUT);
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
  // dùng cho ds18b20
  sensors_ds.begin();
  delay(10);
  deviceCount = sensors_ds.getDeviceCount();
  char extra[50];
  sprintf(extra,"\nĐang tìm ds18b20...tìm thấy: %d device",deviceCount);
  Serial.println(extra);
  // dùng cho rtc
  if(!rtc.begin() || !rtc.isrunning())  {Serial.print("RTC Không Hoạt Động\n");}  else  {Serial.print("RTC Đã bắt đầu...");}
// ------------- Set thoi gian ------------------------------
//    rtc.adjust(DateTime(2020, 10, 10, 8, 28, 0));
//    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // lệnh cho TDS
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization
}

// ************************* khoi tao ket noi ban dau ************************
void init_send_once()
{
  while((Ethernet.linkStatus()!=LinkON) || (!client.connected()) )
  {
    if(Ethernet.linkStatus()!=LinkON)    {   ethernet();       }
    if(!client.connected())              {   MQTTreconnect();  }
    delayMicroseconds(500);
    multi_ds18b20();
    tds();
    YHDC100();
  }
}

void send_ar()
{
  Serial.println("reset board after for 12h");
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
  if((millis()-tar)>=5001)
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
