/*
 * cac chan khai bao
 * dht11 = 49
 * ds = 45
 * reset = 46
 * sda = 20
 * scl = 21
 * 
 */
// ---------------------- RTC-----------------------------

void rtcds1307() {
    DateTime now = rtc.now();
    lcd.clear();
    lcd.setCursor(4, 1);
    if(now.hour()<=9)
    {
      lcd.print("0");
      lcd.print(now.hour());
      Serial.print("0");
      Serial.print(now.hour());
    }
    else {
        Serial.print(now.hour());
        lcd.print(now.hour()); 
      }
        lcd.print(':');
        Serial.print(':');
    if(now.minute()<=9)
      {
        lcd.print("0");
        lcd.print(now.minute());
        Serial.print("0");
        Serial.print(now.minute());
      }
    else {
        lcd.print(now.minute());
        Serial.print(now.minute());
      }
        lcd.print(':');
        Serial.print(':');
   if(now.second()<=9)
   {
        lcd.print("0");
        lcd.print(now.second());
        Serial.print("0");
        Serial.print(now.second());
    }
   else {
        lcd.print(now.second()); 
        Serial.print(now.second());
    }
        Serial.println();
        lcd.setCursor(1, 0);
        lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
        Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
        lcd.print(",");
        Serial.print(",");
   if(now.day()<=9)
    {
        lcd.print("0");
        lcd.print(now.day());
        Serial.print("0");
        Serial.print(now.day());
      }
   else {
        lcd.print(now.day());
        Serial.print(now.day());
      }
        lcd.print('/');
        Serial.print('/');
   if(now.month()<=9)
    {
        lcd.print("0");
        lcd.print(now.month());
        Serial.print("0");
        Serial.print(now.month());
      }
   else {
        lcd.print(now.month()); 
        Serial.print(now.month());
      }
        lcd.print('/');
        Serial.print('/');
   if(now.year()<=9)
    {
        lcd.print("0");
        lcd.print(now.year());
        Serial.print("0");
        Serial.print(now.year());
      }
   else {
        lcd.print(now.year()); 
        Serial.print(now.year());
    } 
        Serial.println();
}
  
