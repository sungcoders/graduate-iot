
void ethernet()
{
  byte i=0;
  Serial.print(F("connecting ethernet :\n"));
  lcd_print(0,5,"Internet: ");
  Ethernet.begin(mac,IP,dns1,gateway,subnetmsk);
  while(Ethernet.linkStatus()!=LinkON)  //https://www.arduino.cc/en/Reference/EthernetLinkStatus
  {
    i++;
    Serial.print(F("Ethernet lỗi - chờ 3s, Lần: "));
    Serial.print(i);
    lcd_print(10,5,"again_");
    lcd_int(16,5,i);
    delay(2000);
    Ethernet.begin(mac,IP,dns1,gateway,subnetmsk);
    delay(200);
    switch(Ethernet.linkStatus())
    {
      case Unknown:
        {Serial.println(F("\tKiểm tra dây bus?")); lcd_print(10,5,"bus?       ");}
        break;
      case LinkON:
        {Serial.println(F("\tEthernet thành công.")); lcd_print(10,5,"success     ");}
        break;
      default:
        {Serial.println(F("\tEthernet thất bại!")); lcd_print(10,5,"failed      ");}
        break;
    }
    if(i>=20)
    {
      Serial.println(F("Reset lỗi Ethernet !"));
      lcd_print(10,5,"reset       ");
      delay(100);
      resetFunc();                                // ham reset
    }
    delayMicroseconds(200);
  }
  Serial.print(F("Ethernet Thành công..."));
  lcd_print(52,5,"success");
  Serial.print(F("IP Address: "));
  Serial.println(Ethernet.localIP());
} //end ethetnet

boolean MQTTreconnect()
{
  int j=0;
  while(!client.connected())
  {
    if (Ethernet.linkStatus() != LinkON) { ethernet(); }
    j++;
    Serial.print(F("MQTT kết nối - Lần: "));
    Serial.print(String(j));
    lcd_print(0,6,"Server:");
    lcd_int(7,6,j);
    Serial.print(F("\tKết nối lại..."));
    lcd_print(10,6,"again!");
    if(client.connect("arduinoClient"))
    {
      Serial.println(F("Đã kết nối MQTT"));
      lcd_print(7,6,"success        ");
      client.subscribe("mega1/sub");
      client.subscribe("van1");
      client.subscribe("van2");
      client.subscribe("dc1");
      client.subscribe("dc2");
      client.subscribe("ac1");
      client.subscribe("ac2");
      client.subscribe("tds_sub");
      client.subscribe("yhdc_sub");
      client.subscribe("ds18b20_sub");
      client.publish("caytrong","cay_trong");
      client.publish("status","không có thông báo mới");
    }
    else 
    {
      Serial.print(F("\nMQTT thất bại, Thử lại lần nữa - lỗi:"));
      lcd_print(7,6,"failed         ");
      Serial.println(client.state());
      delay(2000);
    }
    if(j>=15) 
    {
      Serial.println(F("reset lỗi MQTT !"));
      lcd_print(7,6,"reset         ");
      delayMicroseconds(500);
      resetFunc();
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print(F("tin nhắn từ ["));
  Serial.print(topic);
  Serial.print(F("] - "));
  for (byte i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  switch((char)payload[0])
  {
    case 'p':
      {digitalWrite(8, LOW);  Serial.println(F("\tbat van 1")); lcd_print(6,4,"on van1");} // relay 1 8
      break;
    case 'l':
      {digitalWrite(8, HIGH);  Serial.println(F("\ttat van 1"));  lcd_print(6,4,"off van1");}
      break;
    case 'o':
      {digitalWrite(9, LOW);  Serial.println(F("\tbat van 2")); lcd_print(6,4,"on van2");}  // relay 2 9
      break;
    case 'k':
      {digitalWrite(9, HIGH);  Serial.println(F("\ttat van 2")); lcd_print(6,4,"off van2");}  // relay 3 10
      break;
    case 'i':
      {digitalWrite(10, LOW);   Serial.println(F("\tbat DC 1")); lcd_print(6,4,"on dc1  ");} // relay 4 11
      break;
    case 'j':
      {digitalWrite(10, HIGH);   Serial.println(F("\ttat DC 1"));  lcd_print(6,4,"off dc1  ");}  // relay 5 12
      break;
    case 'u':
      {digitalWrite(11, LOW);   Serial.println(F("\tbat DC 2")); lcd_print(6,4,"on dc2  ");}   // relay 6 7
      break;
    case 'h':
      {digitalWrite(11, HIGH);   Serial.println(F("\ttat DC 2")); lcd_print(6,4,"off dc2  ");}   // relay 7 6
      break;
    case 'y':
      {digitalWrite(12, LOW);   Serial.println(F("\tbat AC 1")); lcd_print(6,4,"on ac1  ");}
      break;
    case 'g':
      {digitalWrite(12, HIGH);   Serial.println(F("\ttat AC 1")); lcd_print(6,4,"off ac1  ");}
      break;
    case 't':
      {digitalWrite(6, LOW);   Serial.println(F("\tbat AC 2")); lcd_print(6,4,"on ac2  ");}
      break;
    case 'f':
      {digitalWrite(6, HIGH);   Serial.println(F("\ttat AC 2")); lcd_print(6,4,"off ac2  ");}
      break;
//    case 'b':
//      {multi_ds18b20(); Serial.print(F("Đã gửi Nhiệt Độ.")); client.publish("status","Đã gửi nhiệt độ lên !"); }
//      break;
  }
}
