
float ds18b20(int ds)
{
  // gioi han nhiet do
  float limit_over = 50;
  float limit_under = 25;
  // khai bao
  float tempC;
  const char ds18b20[10];          // cac MQTT topic
  int count = check_ds18b20();
  if(count>0)
  {
    if((ds-1)<count)
    {
      sprintf(ds18b20,"ds18b20_%d",ds);
      sensors_ds.requestTemperatures();
      tempC = sensors_ds.getTempCByIndex(ds-1);
      if(limit_under<=tempC<=limit_over)
      {
        client.publish(ds18b20,float_to_char(tempC," *C"));
        Serial.println(ds18b20);
      }
      else if(tempC<limit_under)
      {
        client.publish(ds18b20,"nhiet do qua thap");
        Serial.println(F("nhiet do qua thap"));
      }
      else
      {
        client.publish(ds18b20,"nhiet do qua cao");
        Serial.println(F("nhiet do qua cao"));
      }
      (ds==1)?lcd_number(0,4,tempC):((ds==2)?lcd_number(6,4,tempC):lcd_number(12,4,tempC));
    }
    return (tempC);
  }
  return false;
}

float DHT_read()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(t) || isnan(h))
  {
    Serial.println(F("Failed to read from DHT"));
    lcd_print(0,1,"Fail DHT invironment");
  }
  else
  {
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print(F("Temperature: "));
    Serial.print(t);
    Serial.println(" *C");
    lcd_print(0,1,"DHT:      *C        %");
    lcd_number(5,1,t);
    lcd_number(15,1,h);
    client.publish("DHT_T",float_to_char(t," *C"));
    client.publish("DHT_H",float_to_char(h," *C"));
  }
  return h,t;
}

void readTdsQuick() {
  analogReference(DEFAULT);
  float aref = 5.0;
  float ec = 0.0;
  float tds = 0.0;
  float waterTemp = 0.0;
  float ecCalibration = 1; // hieu chuan
  waterTemp = 25.0;//dallasTemperature.getTempCByIndex(0);
  float rawEc = analogRead(A8) * aref / 1024.0; // doc tinh hieu tuong tu chuyen sang dien ap
  float temperatureCoefficient = 1.0 + 0.02 * (waterTemp - 25.0); // cong thuc bu nhiet do : nhiet do chuan 25C = t(hientai)/(1+0.02)*(t(hientai)-25C) 
  ec = (rawEc / temperatureCoefficient) * ecCalibration; // hieu chuan ec bu tru nhiet do
  tds = (133.42 * pow(ec, 3) - 255.86 * ec * ec + 857.39 * ec) * 0.5; // chuyen gia tri dien ap thanh tds
  client.publish("TDS_sensor1",float_to_char(tds," ppm"));
  Serial.print(F("TDS:")); Serial.println(tds);
  Serial.print(F("EC:")); Serial.println(ec);
  Serial.print(F("Temperature:")); Serial.println(waterTemp);
  lcd_print(0,3,"TDS:");
  lcd_number(4,3,tds);
  lcd_number(14,3,waterTemp);
  lcd_print(19,3,"*C");
}

float get_corriente()
{
  analogReference(INTERNAL1V1);
  float voltajeSensor;
  float corriente=0;
  float Sumatoria=0;
  long tiempo=millis();
  int N=0;
  while(millis()-tiempo<500)// delay 0.5s duoc 30 chu ky voi tan so 60hz
  {
    voltajeSensor = analogRead(A0) * (1.1 / 1023.0);////dien ap do cam bien
    corriente=voltajeSensor*30.0; //corriente=VoltajeSensor*(30A/1V)
    Sumatoria=Sumatoria+sq(corriente);//tong binh phuong
    N=N+1;
    delay(1);
  }
  Sumatoria=Sumatoria*2;//bu cho binh phuong ở chu ky am nhan voi 2
  corriente=sqrt((Sumatoria)/N); //dong dien hieu dung
  return(corriente); 
}

void YHDC30()
{
  float Irms=get_corriente(); // Dong dien (A)
  Irms=(Irms <0.388)?0:(Irms-0.388);
  float P=Irms*220.0; // tinh cong suat P=IV (Watts)
  client.publish("YHDC100",float_to_char(Irms," A"));
  Serial.print(F("Dòng Điện: "));
  Serial.print(Irms,2);
  Serial.print(F("A, Công suất: "));
  Serial.print(P,2);  
  Serial.println("W");
  lcd_print(0,7,"YHDC:     A        W ");
  lcd_number(6,7,Irms);
  lcd_number(14,7,P);
  (Irms>0.0)?client.publish("on_off_YHDC","RUN"):client.publish("on_off_YHDC","STOP");
}
