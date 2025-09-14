#define Led_1 6
const int PIN_BUTTON = 5;

void setup() {
 // put your setup code here, to run once:
pinMode(Led_1, OUTPUT);
pinMode(PIN_BUTTON, INPUT_PULLUP);
Serial.begin(9600);
}
void loop() {
 // put your main code here, to run repeatedly:
int buttonState = digitalRead(PIN_BUTTON);
if (!buttonState)
{
 delay(250);
 return;
}

if(buttonState)
{
  for(int i = 0; i <= 255; i+=51)
  {
    analogWrite(Led_1, i);
    delay(200);
  }
}
 
}
