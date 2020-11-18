// tds ton 25us
// if 5us



//****************************cam bien dong lan 1*****************************
//  float congsuat;
//  byte var = 0;
//  while (var <5)
//  {
//    double Irms = SCT013.calcIrms (1480);  //so lan lay mau 5-1480
//    var ++; 
//  }
//  SCT013.current(pinSCT,60.6);             // dongdien: input pin, hieuchinh.
//  double Irms = SCT013.calcIrms(100);// Calculate Irms only
//  //congsuat = Irms * dienap*0.7;
//  if( Irms < 0.1)
//  {
//    Irms = 0; congsuat = 0;
//    Serial.print((String)congsuat + String("W\t") + (String)Irms + String("A"));           // Apparent power
//  }
//  else
//  {
//    Irms = Irms-0.1;
//    congsuat = Irms * dienap*0.7;
//    Serial.print((String)congsuat + String("W\t") + (String)Irms + String("A"));            // Apparent power
//  }

//void YHDC100()
//{
//  float Dongdien,sensor,Congsuat,ondac=0;
//  long retardo=millis();
//  int s=0;
//  while(millis()-retardo<500)
//  {
//    sensor = analogRead(pinSCT) * (1.115 / 1023.0);
//    Dongdien=sensor*0.4;
//    ondac=ondac+sq(Dongdien);
//    s=s+1;
//  }
//  ondac=ondac*2.0;
//  Dongdien=sqrt((ondac)/s);
//  Congsuat=Dongdien*230.0;
//  Serial.print(F("Dong Dien: "));
//  Serial.print(Dongdien);
//  Serial.print(F("A   Cong suat: "));
//  Serial.print(Congsuat);
//  Serial.println(F("W"));
//  convertcharfp(Congsuat,Dongdien);
//  client.publish("YHDC100",cvt_c);
//}

//  float temperature = 25, tdsValue = 0, sum_tds = 0;
//  for(byte k=1; k<=5; k++)
//  {
//    gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
//    gravityTds.update();  //sample and calculate 
//    tdsValue = gravityTds.getTdsValue();  // then get the value
//    sum_tds += (int)tdsValue;
//    delayMicroseconds(200);
//  }
//  sum_tds = (float)(sum_tds/5.0);
//  if(sum_tds<=0.0)
//  {
//    client.publish("TDS_sensor1","TDS quá Thấp");
//    Serial.println(F("TDS quá thấp!"));
//  }
//  else if(sum_tds>=1000.0)
//  {
//    client.publish("TDS_sensor1","TDS quá cao");
//    Serial.println(F("TDS quá cao!"));
//  }
//  else
//  {
//    convertcharf(sum_tds);
//    client.publish("TDS_sensor1",cvt_c);
//  }
