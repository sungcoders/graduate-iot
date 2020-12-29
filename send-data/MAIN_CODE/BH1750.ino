
byte buff[2];

int BH1750_display()
{
  int val=0;
  BH1750_Init(0x23);                                          //setting i2c address with ADD LOW
  if(2==BH1750_Read(0x23))
  {
    val=((buff[0]<<8)|buff[1])/1.2;
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
  Wire.beginTransmission(address);       // khởi tạo kết nối
  Wire.requestFrom(address, 2);          // yêu cầu dữ liệu từ master cho slave
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
  Wire.write(0x10);                   //1_lux độ phân giải 120ms
  Wire.endTransmission();
}
