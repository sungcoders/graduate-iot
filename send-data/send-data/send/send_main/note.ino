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
