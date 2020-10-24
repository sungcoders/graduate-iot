
//------------------------enthernet-----------------------------------
void ethernet()
{
  byte i=0;
  Serial.print(F("connecting ethernet :\n"));
  Ethernet.begin(mac,IP);
  while(Ethernet.linkStatus()!=LinkON)  //https://www.arduino.cc/en/Reference/EthernetLinkStatus
  {
    i++;
    Serial.print(F("Ethernet lỗi - chờ 3s, Lần: "));
    Serial.print(i);
    delay(2000);
    Ethernet.begin(mac,IP);
    delay(200);
    switch(Ethernet.linkStatus())
    {
      case Unknown:
        {Serial.println(F("\tKiểm tra dây bus?")); }
        break;
      case LinkON:
        {Serial.println(F("\tEthernet thành công.")); }
        break;
      default:
        {Serial.println(F("\tEthernet thất bại!")); }
        break;
    }
    if(i>=20)
    {
      Serial.println(F("Reset lỗi Ethernet !"));
      delay(100);
      resetFunc();                                // ham reset
    }
    delayMicroseconds(200);
  }
  Serial.print(F("Ethernet Thành công..."));
  Serial.print(F("IP Address: "));
  Serial.println(Ethernet.localIP());
} //end ethetnet

// -------------------------- MQTT ------------------------------
void MQTTreconnect()
{
  int j=0;
  if (Ethernet.linkStatus() != LinkON) { ethernet(); }
  while(!client.connected())
  {
    j++;
    Serial.print(F("MQTT kết nối - Lần: "));
    Serial.print(String(j));
    Serial.print(F("\tKết nối lại..."));
    if(client.connect("arduinoClient"))
    {
      Serial.println(F("Đã kết nối MQTT"));
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
      Serial.println(client.state());
      delay(1000);
    }
    if(j>=15) 
    {
      Serial.println(F("reset lỗi MQTT !"));
      delayMicroseconds(500);
      resetFunc();
    }
  }
}

//---------------------- callback from MQTT -------------------------------
void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print(F("tin nhắn từ ["));
  Serial.print(topic);
  Serial.print(F("] - "));
  for (byte z = 0; z < length; z++)
  {
    Serial.print((char)payload[z]);
  }
  switch((char)payload[0])
  {
    case 'p':
      {digitalWrite(40, HIGH);  Serial.println("\tbat van 1");}
      break;
    case 'l':
      {digitalWrite(40, LOW);  Serial.println("\ttat van 1");}
      break;
    case 'o':
      {digitalWrite(38, HIGH);  Serial.println("\tbat van 2"); }
      break;
    case 'k':
      {digitalWrite(38, LOW);  Serial.println("\ttat van 2"); }
      break;
    case 'i':
      {digitalWrite(46, HIGH);   Serial.println("\tbat DC 1"); }
      break;
    case 'j':
      {digitalWrite(46, LOW);   Serial.println("\ttat DC 1"); }
      break;
    case 'u':
      {digitalWrite(48, HIGH);   Serial.println("\tbat DC 2"); }
      break;
    case 'h':
      {digitalWrite(48, LOW);   Serial.println("\ttat DC 2"); }
      break;
    case 'y':
      {digitalWrite(42, HIGH);   Serial.println("\tbat AC 1"); }
      break;
    case 'g':
      {digitalWrite(42, LOW);   Serial.println("\ttat AC 1"); }
      break;
    case 't':
      {digitalWrite(44, HIGH);   Serial.println("\tbat AC 2"); }
      break;
    case 'f':
      {digitalWrite(44, LOW);   Serial.println("\ttat AC 2"); }
      break;
    case 'b':
      {multi_ds18b20(); Serial.print(F("Đã gửi Nhiệt Độ.")); client.publish("status","Đã gửi nhiệt độ lên !"); }
      break;
    default:
      Serial.println("end");
      break;
  }
  Serial.println();
}
