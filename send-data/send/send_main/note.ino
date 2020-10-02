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

void rtcds1307() 
{
  DateTime now = rtc.now();
  Serial.print((String)now.hour() + ":" + (String)now.minute() + ":" + (String)now.second() + String("\n"));
  Serial.print((String)daysOfTheWeek[now.dayOfTheWeek()] + String(",") + (String)now.day() + "/" + (String)now.month() + "/" + (String)now.year() + String("\n"));
}
  
