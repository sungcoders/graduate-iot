
byte buff[2];

int BH1750_display()
{
  int val=0;
  BH1750_Init(0x23);
  if(2==BH1750_Read(0x23))
  {
    val=((buff[0]<<8)|buff[1])/1.2;
    //Serial.print(val,DEC);     
    //Serial.println("lux");
    lcd_print(0,2,"Anh sang:       lux");
    lcd_number(10,2,val);
    client.publish("BH1750_1",num_to_char(val," lux"));
  }
  else
  {
    lcd_print(0,2,"Anh sang: fail  lux");
    client.publish("BH1750_1","fail  lux");
  }
  return (val);
}

int BH1750_Read(int address)
{
  int i=0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while(Wire.available())
  {
    buff[i] = Wire.read();
    i++;
  }
  Wire.endTransmission();  
  return i;
}
 
void BH1750_Init(int address)
{
  Wire.beginTransmission(address);
  Wire.write(0x10);                   //1_lux reolution 120ms
  Wire.endTransmission();
}
