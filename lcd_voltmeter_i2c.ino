#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

int potpin = A2;

void setup() {
  // put your setup code here, to run once:
lcd.init();
lcd.backlight();
lcd.setCursor(1, 0);
pinMode(potpin, INPUT);
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
float val = analogRead(potpin);
val = val / 1023.0 * 4.0;
lcd.setCursor(1, 1);
lcd.print(val);
lcd.print("V");
delay(200);
}
