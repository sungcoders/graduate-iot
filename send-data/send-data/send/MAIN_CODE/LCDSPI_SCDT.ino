#include "LCDSPI_SCDT.h"
#include "font5x7.h"

void lcd_write(uint8_t cd, uint8_t Byte)
{ //SPI 4Line
  uint8_t h;
  digitalWrite(CS, LOW); //CS
  if (cd)digitalWrite(CD, HIGH); //CD
  else digitalWrite(CD, LOW); //CD
  for (h = 0; h < 8; h++)
  {
    if (!(Byte & 0x80))digitalWrite(SDA, LOW); //SDA
    else digitalWrite(SDA, HIGH); //SDA
    digitalWrite(SCK, LOW); //SCK
    digitalWrite(SCK, HIGH); //SCK
    Byte <<= 1;
  }
  if (!cd)digitalWrite(CD, HIGH); //CD
  else digitalWrite(CD, LOW); //CD
  digitalWrite(CS, HIGH); //CS
}
void lcd_gotoxy(uint8_t x, uint8_t y)
{
  x = x * 6; //Graphic Mode thi xoa dong nay di, Text thi de lai
  lcd_write(0, (x & 0x0f));
  lcd_write(0, ((x >> 4) & 0x07) | 0x10);
  lcd_write(0, (y & 0x0f) | 0xb0);
}
void lcd_clear(void)
{
  uint8_t u, w;
  for (u = 0; u < 8; u++)
  {
    lcd_gotoxy(0, u);
    for (w = 0; w < 130; w++)
      lcd_write(1, 0);
  }
  lcd_gotoxy(0, 0);
}
void lcd_putchar(char dat)
{
  uint8_t v;
  if ((dat >= 0x20) && (dat <= 0x7f))
  {
    dat = dat - 32;
    for (v = 0; v < 6; v++)
      lcd_write(1, font[dat][v]);
  }
}
void lcd_puts(char *s)
{
  while (*s)
    lcd_putchar(*s++);
}
void lcd_init(void)
{ //Khoi tao GLCD
  lcd_write(0, 0xE2); //Reset
  delay(100);       //Delay 100ms
  lcd_write(0, 0xAF); //Display ON, 0xAE - OFF
  lcd_write(0, 0x2F); //Power control
  lcd_clear(); //Xoa man hinh
}

void lcd_number(uint8_t x, uint8_t y, float wr)
{
  String st = (String)wr;
  char ct[20];
  st.toCharArray(ct, st.length() + 1);
  lcd_gotoxy(x, y);
  lcd_puts(ct);
}

void start_lcd(void)
{
  pinMode(CS, OUTPUT);  //Chon chip
  pinMode(SDA, OUTPUT);  //Data SPI
  pinMode(CD, OUTPUT);  //Chot du lieu Command va Data (C/D)
  pinMode(SCK, OUTPUT);  //Clock SPI
  lcd_init(); //Khoi tao lcd
}

void lcd_print(uint8_t x, uint8_t y, String str)
{
  char ctstr[25];
  str.toCharArray(ctstr, str.length() + 1);
  lcd_gotoxy(x, y);
  lcd_puts(ctstr);
}

void lcd_int(uint8_t x, uint8_t y, int in)
{
  char ct[10];
  String stri=(String)in;
  stri.toCharArray(ct,stri.length()+1);
  lcd_gotoxy(x,y);
  lcd_puts(ct);
}
