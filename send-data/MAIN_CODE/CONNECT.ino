
void ethernet()
{
  byte i=0;
  lcd_print(0,5,"Internet: ");
  Ethernet.begin(mac,IP,dns1,gateway,subnetmsk);
  while(Ethernet.linkStatus()!=LinkON)  //https://www.arduino.cc/en/Reference/EthernetLinkStatus
  {
    i++;
    lcd_print(10,5,"again_");
    lcd_int(16,5,i);
    delay(2000);
    Ethernet.begin(mac,IP,dns1,gateway,subnetmsk);
    delay(200);
    switch(Ethernet.linkStatus())
    {
      case Unknown:
        {lcd_print(10,5,"bus?       ");}
        break;
      case LinkON:
        {lcd_print(10,5,"success     ");}
        break;
      default:
        {lcd_print(10,5,"failed      ");}
        break;
    }
    if(i>=20)
    {
      lcd_print(10,5,"reset       ");
      delay(100);
      resetFunc();                                // ham reset
    }
    lcd_print(10,5,"success     ");
    delayMicroseconds(200);
  }
} //end ethetnet

boolean MQTTreconnect()
{
  int j=0;
  while(!client.connected())
  {
    if (Ethernet.linkStatus() != LinkON) { ethernet(); }
    j++;
    lcd_print(0,6,"Server:");
    lcd_int(7,6,j);
    lcd_print(10,6,"again!");
    if(client.connect("arduinoClient"))
    {
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
      client.subscribe("updates");
      client.subscribe("get_data");
      client.publish("caytrong","cay_trong");
      client.publish("status","không có thông báo mới");
      client.publish("send_update","no request");
    }
    else 
    {
      lcd_print(7,6,"failed         ");
      delay(2000);
    }
    if(j>=15) 
    {
      lcd_print(7,6,"reset         ");
      delayMicroseconds(500);
      resetFunc();
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length)
{
  switch((char)payload[0])
  {
    case 'p':
      {digitalWrite(8, HIGH); lcd_print(6,4,"OFF");  check_motor = 1; stt_dc=0;} // relay 1 8
      break;
    case 'l':
      {digitalWrite(8, LOW);  lcd_print(6,4,"ON");   check_motor = 1; stt_dc=1;}
      break;
    case 'o':
      {digitalWrite(9, LOW); lcd_print(6,4,"on van2");}  // relay 2 9
      break;
    case 'k':
      {digitalWrite(9, HIGH); lcd_print(6,4,"off van2");}  // relay 3 10
      break;
    case 'i':
      {digitalWrite(10, LOW); lcd_print(6,4,"on dc1  ");} // relay 4 11
      break;
    case 'j':
      {digitalWrite(10, HIGH); lcd_print(6,4,"off dc1  ");}  // relay 5 12
      break;
    case 'u':
      {digitalWrite(11, LOW); lcd_print(6,4,"on dc2  ");}   // relay 6 7
      break;
    case 'h':
      {digitalWrite(11, HIGH); lcd_print(6,4,"off dc2  ");}   // relay 7 6
      break;
    case 'y':
      {digitalWrite(12, LOW); lcd_print(6,4,"on ac1  ");}
      break;
    case 'g':
      {digitalWrite(12, HIGH); lcd_print(6,4,"off ac1  ");}
      break;
    case 't':
      {digitalWrite(6, LOW); lcd_print(6,4,"on ac2  ");}
      break;
    case 'f':
      {digitalWrite(6, HIGH); lcd_print(6,4,"off ac2  ");}
      break;
    case 'z':
      {lcd_print(6,4,"off ac2  "); client.publish("send_update","wait for..."); BH1750_display(); ds18b20(1); ds18b20(2); YHDC30(); DHT_read(); readTdsQuick(); client.publish("send_update","done");}
      break;
    case 'x':
      {client.publish("stt_server","geting data... - no control"); lcd_print(0,0," request from server "); BH1750_display(); ds18b20(1); ds18b20(2); YHDC30(); readTdsQuick(); DHT_read(); lcd_print(0,0,"     done request    "); client.publish("stt_server","done, Let's control");}
      break;
  }
}
