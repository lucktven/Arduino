#define Led_1 6

void setup() {
 // put your setup code here, to run once:
pinMode(Led_1, OUTPUT);
Serial.begin(9600);
}
void loop() {
 // put your main code here, to run repeatedly:


  for(int i = 0; i <= 255; i+=21)
  {
    analogWrite(Led_l, i);
    delay(200);
  }

  for(int i = 255; i >= 0; i-=21)
  {
    analogWrite(Led_l, i);
    delay(200);
  }
 
}
