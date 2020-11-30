
#define RXD2 16
#define TXD2 17
#define TIME1   1000
#define TIME5P  300000

unsigned long t4,t5;
uint8_t espcount=0;
extern volatile unsigned long timer0_millis;

void milirst()
{
  noInterrupts();
  timer0_millis = 0;
  interrupts();
}

void setup()
{
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  pinMode(2,OUTPUT);
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);
}

void loop()
{
  uart_receive();
  uart_check();
}

void uart_receive()
{
  String a;
  while (Serial2.available()>0)
  {
    a += char(Serial2.read());
    delay(100);
    if(Serial2.available()==0)
    {
      delay(10);
      Serial.println(a);
      if(a=="MEGA2560")
      {
        Serial.println("MEGA đang hoạt động");
        a="";
        digitalWrite(4,HIGH);
        digitalWrite(2,HIGH);
        delay(500);
        digitalWrite(2,LOW);
        delay(500);
        digitalWrite(2,HIGH);
        delay(500);
        digitalWrite(2,LOW);
        t4=millis();

      }
      else
      {
        Serial.println("MEGA đang chờ tín hiệu");
        espcount +=1;
        if(espcount>=4)
        {
          digitalWrite(4,LOW);
          Serial.println("da bat relay");
          Serial.println(a);
          delay(30000);
          digitalWrite(4,HIGH);
          Serial.println("da tat relay");
          espcount=0;
        }
        a="";
      }
      a="";
    }
    delay(5);
    Serial.print(".");
  }
}

void uart_send()
{
  if(millis()-t5>TIME1*60) // 1phut
  {
    Serial2.write("ESP32");
    Serial.println("i am here");
    t5=millis();
  }  
}

void uart_check()
{
  if(millis()-t4>TIME1*300) // 5phut
  {
    while (Serial2.available()==0)
    {
      digitalWrite(4,LOW);
      Serial.println("da bat relay 1");
      delay(30000); // 30s
      digitalWrite(4,HIGH);
      Serial.println("da off relay1");
    t4=millis();
    break;
    }
    t4=millis();
  }
}
