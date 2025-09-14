#define LED 9
#define sensor A1

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(sensor, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = analogRead(sensor);
  val = map(sensor, 0, 1023, 10, 50);
  analogWrite(LED, val);
}
