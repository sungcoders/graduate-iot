boolean wifi()
{
  WiFi.begin(ssid,pass);
  while(WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    digitalWrite(2,LOW);
    delay(500);
    Serial.print(".");
    return false;
  }
  digitalWrite(2,HIGH);
  Serial.println("WIFI CONNECTED");
  return true;
}

boolean reconnect()
{
  if(WiFi.waitForConnectResult() != WL_CONNECTED) {wifi();}
  while (!client.connected())
  {
    Serial.print("MQTT again...");
    if(client.connect("ESP01"))
    {
      Serial.println(F("MQTT Connected"));
      client.publish("outTopic", "hello world");
      client.subscribe("inTopic");
      return false;
    }
    else
    {
      Serial.print(F("failed, rc="));
      Serial.print(client.state());
      Serial.println(F(" try again..."));
      delay(5000);
      return true;
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print(F("Message arrived ["));
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  switch((char)payload[1])
  {
    case '1': {digitalWrite(BUILTIN_LED, LOW); Serial.println(F("Đã bật led")); break;}
    case '2': {digitalWrite(BUILTIN_LED, HIGH); Serial.println(F("Đã tắt led")); break;}
  }
}
