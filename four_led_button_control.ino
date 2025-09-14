#define Led_1 6
#define Led_2 7
#define Led_3 10
#define Led_4 11

const int PIN_BUTTON = 5;

void setup() {
  // put your setup code here, to run once:
pinMode(Led_1, OUTPUT);
pinMode(Led_2, OUTPUT);
pinMode(Led_3, OUTPUT);
pinMode(Led_4, OUTPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
int buttonState = digitalRead(PIN_BUTTON);

if (!buttonState)
{
  delay(50);
  return;
}

 digitalWrite(Led_1, HIGH);
 digitalWrite(Led_2, HIGH);
 digitalWrite(Led_3, HIGH);
 digitalWrite(Led_4, HIGH);
 delay(250);
 digitalWrite(Led_1, LOW);
 digitalWrite(Led_2, LOW);
 digitalWrite(Led_3, LOW);
 digitalWrite(Led_4, LOW);
 delay(250);

}
