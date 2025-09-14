#define Led_1 11
#define Led_2 7

int n1 = 5;
int n2 = 4;


void setup() {
  // put your setup code here, to run once:
pinMode(Led_1, OUTPUT);
pinMode(Led_2, OUTPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

for(int i = 0; i < n1; i++)
{
 digitalWrite(Led_1, HIGH);
 delay(200);
 digitalWrite(Led_1, LOW);
 delay(200);
}

for(int i = 0; i < n2; i++)
{
 digitalWrite(Led_2, HIGH);
 delay(500);
 digitalWrite(Led_2, LOW);
 delay(500);
}
}
