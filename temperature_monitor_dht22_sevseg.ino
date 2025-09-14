//Блок підключення бібліотеки та змінних для роботи з датчиком DHT22
#include "DHT.h"
#define DHTPIN A3
#define DHTTYPE DHT22

#define LED A0

//Створення об’єкта типу DHT для контролювання датчика 
DHT dht(DHTPIN, DHTTYPE);

#include <SevSeg.h>
SevSeg sevseg;

void setup() {
  Serial.begin(115200);

pinMode(DHTPIN, INPUT);
dht.begin();
//Налаштування чотирьох цифрового семисигментного дисплея
  pinMode(A0, OUTPUT);

byte numDigits = 4;
byte digitPins[] ={2, 3, 4, 5};
byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13};
bool resistorsOnSegments = false;
byte hardwareConfig = COMMON_ANODE;
bool updateWithDelays = false;
bool leadingZeros = false;
bool disableDecPoint = false;

sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
updateWithDelays, leadingZeros, disableDecPoint);
sevseg.setBrightness(90);
}

void loop() {

static unsigned long timer = millis();
//Зчитування значення температури з датчика
float t = dht.readTemperature();
Serial.println(t);

int temp = t * 10;
/*Виведення температури на семисигментний дисплей. Крапка стоїть після першого символу*/
sevseg.setNumber(temp, 1);

//Очищення дисплею
sevseg.refreshDisplay();
delay(2);
/*при умові, за якої температура відмінна від оптимальної на 30 градусів, світлодіод буде мерехтіти*/ 
if(t>30.0 || t<0.0)
{
if(millis() - timer > 500) digitalWrite(LED, HIGH);
if(millis() - timer > 500) digitalWrite(LED, LOW);
}
else
{
digitalWrite(LED, LOW);
}
