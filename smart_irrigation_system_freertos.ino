
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <FreeRTOS_SAMD21.h>

#define SOIL_MOISTURE_PIN A0
#define TEMPERATURE_PIN A1
#define PUMP_PIN 2

#define SOIL_MOISTURE_THRESHOLD 400
#define TEMPERATURE_THRESHOLD 25.0

#define DHT_PIN 7
#define DHT_TYPE DHT11

LiquidCrystal_I2C lcd(0x27, 16, 2); // Адреса модуля та кількість символів та рядків

DHT dht(DHT_PIN, DHT_TYPE);

void readSoilMoisture(void *pvParameters) {
  (void)pvParameters;

  while (1) {
    int soilMoisture = analogRead(SOIL_MOISTURE_PIN);

    Serial.print("Вологість ґрунту: ");
    Serial.println(soilMoisture);

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void readTemperature(void *pvParameters) {
  (void)pvParameters;

  while (1) {
    float temperature = dht.readTemperature();
    int temperatureNTC = analogRead(TEMPERATURE_PIN);
    Serial.print("Температура DHT: ");
    Serial.println(temperature);
    Serial.print("Температура NTC: ");
    Serial.println(temperatureNTC);

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void controlPump(void *pvParameters) {
  (void)pvParameters;

  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);

  while (1) {
    int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
    float temperature = dht.readTemperature();

    if (soilMoisture < SOIL_MOISTURE_THRESHOLD && temperature > TEMPERATURE_THRESHOLD) {
      digitalWrite(PUMP_PIN, HIGH);
      Serial.println("Полив увімкнено");
    } else {
      digitalWrite(PUMP_PIN, LOW);
      Serial.println("Полив вимкнено");
    }

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2); // Ініціалізація LCD: 16 символів у 2 рядках
  lcd.backlight(); // Увімкнення підсвічування

  lcd.print("Система поливу");
  lcd.setCursor(0, 1);
  lcd.print("запущена");
  delay(2000);
  lcd.clear();

  dht.begin();

  xTaskCreatePinnedToCore(readSoilMoisture, "ReadSoilMoisture", 2048, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(readTemperature, "ReadTemperature", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(controlPump, "ControlPump", 2048, NULL, 1, NULL, 1);
}

void loop() {
  // Виконання задач FreeRTOS, виконується в основному циклі

  vTaskDelay(pdMS_TO_TICKS(1000));

Код файлу параметрів компонентів системи:
{
  “version”: 1,
  “author”: “Максим Головенець”,
  “editor”: “wokwi”,
  “parts”: [
    { “type”: “wokwi-arduino-mega”, “id”: “mega”, “top”: -310.31, “left”: 12.01, “attrs”: {} },
    { “type”: “chip-soil_sensor”, “id”: “chip1”, “top”: -50.51, “left”: 261.84, “attrs”: {} },
    {
      “type”: “wokwi-lcd1602”,
      “id”: “lcd1”,
      “top”: -138.65,
      “left”: 510.53,
      “attrs”: { “pins”: “i2c” }
    },
    { “type”: “wokwi-servo”, “id”: “servo1”, “top”: -433.83, “left”: 588.57, “attrs”: {} },
    {
      “type”: “wokwi-ntc-temperature-sensor”,
      “id”: “ntc2”,
      “top”: -518.66,
      “left”: -339.12,
      “attrs”: {}
    }
  ],
  “connections”: [
    [ “chip1:GND”, “uno:GND.3”, “black”, [ “h0” ] ],
    [ “chip1:VCC”, “uno:5V”, “red”, [ “h0” ] ],
    [ “chip1:A0”, “uno:A0”, “cyan”, [ “h0” ] ],
    [ “lcd1:VCC”, “mega:5V”, “violet”, [ “v-60.76”, “h-338.02” ] ],
    [ “lcd1:GND”, “mega:GND.2”, “black”, [ “v-58.74”, “h-328.27” ] ],
    [ “lcd1:SDA”, “mega:20”, “white”, [ “h-53.54”, “v-239.39”, “h-74.5” ] ],
    [ “lcd1:SCL”, “mega:21”, “gray”, [ “h-42.69”, “v-259.16”, “h-75.85” ] ],
    [ “chip1:VCC”, “mega:5V”, “red”, [ “h0” ] ],
    [ “chip1:GND”, “mega:GND.3”, “black”, [ “h0” ] ],
    [ “chip1:A0”, “mega:A0”, “#8f4814”, [ “h0” ] ],
    [ “servo1:GND”, “mega:GND.2”, “black”, [ “h-592.58”, “v318.4”, “h190.27” ] ],
    [ “servo1:V+”, “mega:5V”, “cyan”, [ “h-647.88”, “v332.74”, “h235.82” ] ],
    [ “servo1:PWM”, “mega:2”, “limegreen”, [ “h0” ] ],
    [ “ntc2:GND”, “mega:GND.1”, “black”, [ “h0” ] ],
    [ “ntc2:VCC”, “mega:5V”, “purple”, [ “h132.89”, “v405.05”, “h247.74” ] ],
    [ “ntc2:OUT”, “mega:1”, “magenta”, [ “h0” ] ]
  ],
  «dependencies»: {}
}

Код файлу датчика вологості грунту:


#include “wokwi-api.h”
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  pin_t pin;
  float moisture;
} chip_data_t;

void chip_timer_callback(void *data) 
{
  chip_data_t *chip_data = (chip_data_t*)data;
  float moisture = attr_read(chip_data->moisture);
  float volts = 5.0 * (moisture / 1024.0);
  pin_dac_write(chip_data->pin, volts);
}

void chip_init() 
{
  chip_data_t *chip_data = (chip_data_t*)malloc(sizeof(chip_data_t));
  chip_data->moisture = attr_init(“moisture”, 500.0);
  chip_data->pin = pin_init(“A0”, ANALOG);

  const timer_config_t config = 
  {
    .callback = chip_timer_callback,
    .user_data = chip_data,
  };

  timer_t timer_id = timer_init(&config);
  timer_start(timer_id, 1000, true);
}

Код файлу відображення датчика на схемі:

{
  “name”: “Soil Sensor”,
  “author”: “Максим Головенець”,
  “pins”: [
    “GND”,
    “VCC”,
    “A0”,
    “”,
    “”,
    “”,
    “”,
    “”
  ],
  “controls”: [
    {
      “id”: “moisture”,
      “label”: “Soil Moisture”,
      “type”: “range”,
      “min”: 239,
      “max”: 595,
      “step”: 1
    }
  ]
}
