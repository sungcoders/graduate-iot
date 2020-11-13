#ifndef LCDSPI_SCDT_H
#define LCDSPI_SCDT_H
// set reset - 3,3v
#define CS   43   //Chon chip - cs lcd 
#define SDA  45   //Data SPI  - sdi lcd
#define CD   47   //Chot du lieu Command va Data (C/D)  - a0 lcd
#define SCK  49   //Clock SPI  - sclk lcd

void lcd_init(void);
void lcd_puts(char *s);
void lcd_putchar(char dat);
void lcd_clear(void);
void lcd_gotoxy(uint8_t x, uint8_t y);
void lcd_write(uint8_t cd, uint8_t Byte);
void lcd_number(uint8_t x,uint8_t y, float wr);
void start_lcd(void);
void lcd_print(uint8_t x,uint8_t y, String str);

#endif
