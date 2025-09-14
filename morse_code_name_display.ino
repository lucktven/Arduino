#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

#define Led_1 11

void setup()
{
lcd.init();
lcd.backlight();
lcd.setCursor(1, 0);
lcd.print("Maksim");
lcd.setCursor(1, 1);
lcd.print("Holovenets");
pinMode(Led_1, OUTPUT);
Serial.begin(9600);
}

//Функція, яка відтворює крапку у Морзе за допомогою світлодіода
void dot()
{
digitalWrite(Led_1, HIGH);
Serial.print(" • ");
delay(350);
digitalWrite(Led_1, LOW);
}

//Функція, яка відтворює паузу у Морзе за допомогою світлодіода
void pause()
{
Serial.print("   ");
delay(700);
}

//Функція, яка відтворює тире у Морзе за допомогою світлодіода
void dash()
{
digitalWrite(Led_1, HIGH);
Serial.print(" –– ");
delay(1050);
digitalWrite(Led_1, LOW);
}

void loop() 
{
 //М
 dash();
 dash();
 pause();
 //А
 dot();
 dash();
 pause();
 //К
 dash();
 dot();
 dash();
 pause();
 //С
 dot();
 dot();
 dot();
 pause();
 //И
 dash();
 dot();
 dash();
 dash();
 pause();
 //М
 dash();
 dash();
 pause();
 //Г
 dot();
 dot();
 dot();
 dot();
 pause();
 //О
 dash();
 dash();
 dash();
 pause();
 //Л
 dot();
 dash();
 dot();
 dot();
 pause();
 //О
 dash();
 dash();
 dash();
 pause();
 //В
 dot();
 dash();
 dash();
 pause();
 //Е
 dot();
 pause();
 //Н
 dash();
 dot();
 pause();
 //Е
 dot();
 pause();
 //Ц
 dash();
 dot();
 dash();
 dot();
 pause();
 //Ь
 dash();
 dot();
 dot();
 dash();
 pause();
 delay(5000);                                                     
}
