int potpin = A2;

int digits_array[10][7] = 
{
{ 0,0,0,0,0,0,1 },    // 0
{ 1,0,0,1,1,1,1 },    // 1
{ 0,0,1,0,0,1,0 },    // 2
{ 0,0,0,0,1,1,0 },    // 3
{ 1,0,0,1,1,0,0,},    // 4
{ 0,1,0,0,1,0,0 },    // 5
{ 0,1,0,0,0,0,0 },    // 6
{ 0,0,0,1,1,1,1 },    // 7
{ 0,0,0,0,0,0,0 },    // 8
{ 0,0,0,0,1,0,0 }     // 9
};
void setup() {
  for (int i = 22; i <= 29; ++i)
  {
  pinMode(i, OUTPUT); 
  }

  for (int i = 30; i <= 36; ++i)
  {
  pinMode(i, OUTPUT);
  }
  pinMode(potpin, INPUT);

  Serial.begin(115200);
}
void loop() {
float val;

val = analogRead(potpin);
val *= 4.0 / 1023.0;

for (int pin = 22, i = 0; i < 7; i++, pin++)
{
  digitalWrite(pin, digits_array[int(val)][i]);
}
int num2 = (val - floor(val)) * 10;

for (int pin = 30, i = 0; i < 7; i++, pin++)
{
  digitalWrite(pin, digits_array[num2][i]);
}
}
