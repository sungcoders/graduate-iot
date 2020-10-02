
void multi_ds18b20()
{
  float tempC,tempF;
  char tempc[10],tempf[10];
  char dsc[20],dsf[10];
  sensors_ds.requestTemperatures();
  for (int i = 0;  i < deviceCount;  i++)
  {
    tempC = sensors_ds.getTempCByIndex(i);
    String tempx=(String)tempC;
    tempx.toCharArray(tempc,tempx.length()+1);
    tempF=DallasTemperature::toFahrenheit(tempC);
    String tempf=(String)tempF;
    tempf.toCharArray(dsf,tempf.length()+1);
    sprintf(dsc,"Sensor %d : %s *C\t|\t%s *F",i+1,tempc,dsf);
  }
}
