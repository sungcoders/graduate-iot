
int giay,phut,gio,ngay,thang,nam,wd;
char tg[50];

void read_rtc()
{
  Wire.beginTransmission(0x68);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(0x68,7);
  giay = b2d(Wire.read() & 0x7f);
  phut = b2d(Wire.read());
  gio = b2d(Wire.read() & 0x3f);  // 24h
  wd = b2d(Wire.read());
  ngay = b2d(Wire.read());
  thang = b2d(Wire.read());
  nam = b2d(Wire.read());
  nam +=2020;
}
int b2d(byte num) { return ((num/16*10) + (num%16)); }
int d2b(byte num) { return ((num/10*16) + (num%10)); }

void rtc_display()
{
  sprintf(tg,"%dh:%dp _ %d/%d/%d",gio,phut,ngay,thang,nam);
  lcd_print(0,0,tg);
  Serial.println(tg);
}

void set_time( byte hr, byte min, byte sec, byte d, byte mth, byte yr, byte wd)
{
  Wire.beginTransmission(0x68);
  Wire.write(0x00);
  Wire.write(d2b(sec));
  Wire.write(d2b(min));
  Wire.write(d2b(hr));
  Wire.write(d2b(wd));
  Wire.write(d2b(d));
  Wire.write(d2b(mth));
  Wire.write(d2b(yr));
  Wire.endTransmission();
}

int check_rtc()
{
  read_rtc();
  if(gio>24 || phut>60 || giay>60 || ngay>31 || thang>12) { return 0; }
  else { return 1; }
}
