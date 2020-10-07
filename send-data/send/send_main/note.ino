/*
 * cac chan khai bao
 * dht11 = 49
 * ds = 45
 * reset = 46
 * sda = 20
 * scl = 21
 * 
 */


//  int getMedianNum(int bArray[], int iFilterLen) 
//{
//      int bTab[iFilterLen];
//      for (byte i = 0; i<iFilterLen; i++)
//    bTab[i] = bArray[i];
//      int i, j, bTemp;
//      for (j = 0; j < iFilterLen - 1; j++) 
//      {
//    for (i = 0; i < iFilterLen - j - 1; i++) 
//          {
//      if (bTab[i] > bTab[i + 1]) 
//            {
//    bTemp = bTab[i];
//          bTab[i] = bTab[i + 1];
//    bTab[i + 1] = bTemp;
//       }
//    }
//      }
//      if ((iFilterLen & 1) > 0)
//  bTemp = bTab[(iFilterLen - 1) / 2];
//      else
//  bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
//      return bTemp;
//}

 
 
//  static unsigned long analogSampleTimepoint = millis();
//   if(millis()-analogSampleTimepoint > 40U)     //every 40 milliseconds,read the analog value from the ADC
//   {
//     analogSampleTimepoint = millis();
//     analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);    //read the analog value and store into the buffer
//     analogBufferIndex++;
//     if(analogBufferIndex == SCOUNT) 
//         analogBufferIndex = 0;
//   }   
//   static unsigned long printTimepoint = millis();
//   if(millis()-printTimepoint > 800U)
//   {
//      printTimepoint = millis();
//      for(copyIndex=0; copyIndex<SCOUNT; copyIndex++)
//      analogBufferTemp[copyIndex]= analogBuffer[copyIndex];
//      averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
//      float compensationCoefficient=1.0+0.02*(temperature-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
//      float compensationVolatge=averageVoltage/compensationCoefficient;  //temperature compensation
//      tdsValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5; //convert voltage value to tds value
//      //Serial.print("voltage:");
//      //Serial.print(averageVoltage,2);
//      //Serial.print("V   ");
////      Serial.print("TDS Value:");
////      Serial.println(tdsValue,0);
//            y+=tdsValue;
////      Serial.println(y);
////      y+=a[x];
//      x++;
//      if(x>=10)
//      {
//        y=y/x;
//        x=0;
//        String tds_temp = (String)y;
//        char tds[5];
//        sprintf(tds,"%d ppm",tds_temp);
//        client.publish("TDS_sensor1",tds);
//        Serial.print("\ntds: ");
//        Serial.println(tds);
//        y=0;
//      }
//   }

  
