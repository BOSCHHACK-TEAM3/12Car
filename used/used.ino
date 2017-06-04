#define THRESHOLD 75


#define TRIG 7
#define ECHO 6
#define LED 12


float TestDistance(void);

int afterjudge=0;
int startdistance=0;
byte rx=0;
void setup() {
  // put your setup code here, to run once:
  init();
  Serial.begin(9600);
  pinMode(TRIG,OUTPUT);
  pinMode(ECHO,INPUT);
  pinMode(LED,OUTPUT);
}
float distance=0;
void loop() {
  if(startdistance==0)
  {
    readrx();
    if(rx=='a')
    {
    digitalWrite(LED,HIGH);
    delay(1000);
    digitalWrite(LED,LOW);
    startdistance=1;
    Serial.write('f');
    }
    delay(100); 
  }
  else
  {
    distance = TestDistance();
    if (distance < 1000) Serial.write((distance > 75 ? 'n' : 'c'));
    delay(1000);
    readrx();
    if(rx=='b')
    {
    startdistance=0;
    Serial.write('f');
    digitalWrite(LED,HIGH);
    delay(1000);
    digitalWrite(LED,LOW);
    }
    else if(rx=='a')
    {
      Serial.write('f');
      digitalWrite(LED,HIGH);
      delay(1000);
      digitalWrite(LED,LOW);
      rx=0;
    }
  }
}
  /*
  olddistance =distance;
  distance=TestDistance();
  float difference= olddistance-distance;
  if (difference>500||difference<-500)
  {
    difference=
  }
  */

void readrx()
{
  if(Serial.available())
  {
     rx=Serial.read();
  }
}
/*瓒呭０娉㈡祴璺濆嚱鏁帮紝杩斿洖鍊煎崟浣嶄负cm*/
float TestDistance(void)
{
  digitalWrite(TRIG, LOW);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  float dis = pulseIn(ECHO, HIGH) * 0.34 / 2 / 10;//璺濈璁＄畻锛岄珮鐢靛钩鎸佺画鏃堕棿鍗充负澹版尝浼犳挱鏃堕棿
  return dis;
}

