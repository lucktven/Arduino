#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>
#include "SafeState.h"
#include "icons.h"

/* Визначення запірного механізму */
#define SERVO_PIN 6
#define SERVO_LOCK_POS   20
#define SERVO_UNLOCK_POS 90
Servo lockServo;

/* Дисплей*/
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

/* Keypad setup */
const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLS = 4;
byte rowPins[KEYPAD_ROWS] = {5, 4, 3, 2};
byte colPins[KEYPAD_COLS] = {A3, A2, A1, A0};
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

/* SafeState зберігає секретний код в EEPROM */
SafeState safeState;

void lock() {
  lockServo.write(SERVO_LOCK_POS);
  safeState.lock();
}

void unlock() {
  lockServo.write(SERVO_UNLOCK_POS);
}

void showStartupMessage() {
  lcd.setCursor(4, 0);
  lcd.print("Welcome!");
  delay(1000);

  lcd.setCursor(0, 2);
  String message = "ArduinoSafe v1.0";
  for (byte i = 0; i < message.length(); i++) {
    lcd.print(message[i]);
    delay(100);
  }
  delay(500);
}

String inputSecretCode() {
  lcd.setCursor(5, 1);
  lcd.print("[____]");
  lcd.setCursor(6, 1);
  String result = "";
  while (result.length() < 4) {
    char key = keypad.getKey();
    if (key >= '0' && key <= '9') {
      lcd.print('*');
      result += key;
    }
  }
  return result;
}

void showWaitScreen(int delayMillis) {
  lcd.setCursor(2, 1);
  lcd.print("[..........]");
  lcd.setCursor(3, 1);
  for (byte i = 0; i < 10; i++) {
    delay(delayMillis);
    lcd.print("=");
  }
}

bool setNewCode() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter new code:");
  String newCode = inputSecretCode();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Confirm new code");
  String confirmCode = inputSecretCode();

  if (newCode.equals(confirmCode)) {
    safeState.setCode(newCode);
    return true;
  } else {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Code mismatch");
    lcd.setCursor(0, 1);
    lcd.print("Safe not locked!");
    delay(2000);
    return false;
  }
}

void showUnlockMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(ICON_UNLOCKED_CHAR);
  lcd.setCursor(4, 0);
  lcd.print("Unlocked!");
  lcd.setCursor(15, 0);
  lcd.write(ICON_UNLOCKED_CHAR);
  delay(1000);
}

void safeUnlockedLogic() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.write(ICON_UNLOCKED_CHAR);
  lcd.setCursor(2, 0);
  lcd.print(" # to lock");
  lcd.setCursor(15, 0);
  lcd.write(ICON_UNLOCKED_CHAR);

  bool newCodeNeeded = true;

  if (safeState.hasCode()) {
    lcd.setCursor(0, 1);
    lcd.print("  A = new code");
    newCodeNeeded = false;
  }

  auto key = keypad.getKey();
  while (key != 'A' && key != '#') {
    key = keypad.getKey();
  }

  bool readyToLock = true;
  if (key == 'A' || newCodeNeeded) {
    readyToLock = setNewCode();
  }

  if (readyToLock) {
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.write(ICON_UNLOCKED_CHAR);
    lcd.print(" ");
    lcd.write(ICON_RIGHT_ARROW);
    lcd.print(" ");
    lcd.write(ICON_LOCKED_CHAR);

    safeState.lock();
    lock();
    showWaitScreen(100);
  }
}

void safeLockedLogic() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(ICON_LOCKED_CHAR);
  lcd.print(" Safe Locked! ");
  lcd.write(ICON_LOCKED_CHAR);

  String userCode = inputSecretCode();
  bool unlockedSuccessfully = safeState.unlock(userCode);
  showWaitScreen(200);

  if (unlockedSuccessfully) {
    showUnlockMessage();
    unlock();
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Access Denied!");
    showWaitScreen(1000);
  }
}

void setup() {
  lcd.begin(16, 2);
  init_icons(lcd);

  lockServo.attach(SERVO_PIN);

  /* Make sure the physical lock is sync with the EEPROM state */
  Serial.begin(115200);
  if (safeState.locked()) {
    lock();
  } else {
    unlock();
  }

  showStartupMessage();
}

void loop() {
  if (safeState.locked()) {
    safeLockedLogic();
  } else {
    safeUnlockedLogic();
  }
}
Код файлу параметрів компонентів системи:
{
  "version": 1,
  "author": "Максим Головенець",
  "editor": "wokwi",
  "parts": [
    { "type": "wokwi-arduino-uno", "id": "uno", "top": 200, "left": 20, "attrs": {} },
    { "type": "wokwi-membrane-keypad", "id": "keypad", "top": 140, "left": 360, "attrs": {} },
    {
      "type": "wokwi-servo",
      "id": "servo",
      "top": 20,
      "left": 400,
      "attrs": { "hornColor": "black" }
    },
    { "type": "wokwi-lcd1602", "id": "lcd", "top": 8, "left": 20, "attrs": {} },
    { "type": "wokwi-resistor", "id": "r1", "top": 140, "left": 220, "attrs": { "value": "220" } }
  ],
  "connections": [
    [ "uno:GND.1", "lcd:VSS", "black", [ "v-51", "*", "h0", "v18" ] ],
    [ "uno:GND.1", "lcd:K", "black", [ "v-51", "*", "h0", "v18" ] ],
    [ "uno:GND.1", "lcd:RW", "black", [ "v-51", "*", "h0", "v18" ] ],
    [ "uno:5V", "lcd:VDD", "red", [ "v21.3", "h-170.4", "v-273.83" ] ],
    [ "uno:5V", "r1:2", "red", [ "v30.9", "h-218.4", "v-258.9", "h317.2" ] ],
    [ "r1:1", "lcd:A", "pink", [] ],
    [ "uno:12", "lcd:RS", "blue", [ "v-16", "*", "h0", "v20" ] ],
    [ "uno:11", "lcd:E", "purple", [ "v-20", "*", "h0", "v20" ] ],
    [ "uno:10", "lcd:D4", "green", [ "v-24", "*", "h0", "v20" ] ],
    [ "uno:9", "lcd:D5", "brown", [ "v-28", "*", "h0", "v20" ] ],
    [ "uno:8", "lcd:D6", "gold", [ "v-32", "*", "h0", "v20" ] ],
    [ "uno:7", "lcd:D7", "gray", [ "v-36", "*", "h0", "v20" ] ],
    [ "uno:6", "servo:PWM", "orange", [ "v-40", "*", "h0", "h-52" ] ],
    [ "uno:5V", "servo:V+", "red", [ "v40.5", "h-189.6", "v-268.5", "h353.6", "v-84" ] ],
    [ "uno:GND.1", "servo:GND", "black", [ "v-46", "*", "v88", "h-60" ] ],
    [ "uno:A3", "keypad:C1", "brown", [ "v116", "*", "h0", "v0" ] ],
    [ "uno:A2", "keypad:C2", "gray", [ "v120", "*", "h0", "v0" ] ],
    [ "uno:A1", "keypad:C3", "orange", [ "v124", "*", "h0", "v0" ] ],
    [ "uno:A0", "keypad:C4", "pink", [ "v128", "*", "h0", "v0" ] ],
    [ "uno:5", "keypad:R1", "blue", [ "v-34", "h96", "*", "v12" ] ],
    [ "uno:4", "keypad:R2", "green", [ "v-30", "h80", "*", "v16" ] ],
    [ "uno:3", "keypad:R3", "purple", [ "v-26", "h64", "*", "v20" ] ],
    [ "uno:2", "keypad:R4", "gold", [ "v-22", "h48", "*", "v24" ] ]
  ],
  "dependencies": {}
}


Код заголовного файлу SafeState.h :

#ifndef SAFESTATE_H
#define SAFESTATE_H

class SafeState {
  public:
    SafeState();
    void lock();
    bool unlock(String code);
    bool locked();
    bool hasCode();
    void setCode(String newCode);

  private:
    void setLock(bool locked);
    bool _locked;
};

#endif /* SAFESTATE_H */

Код заголовкового файлу відображення спец-символів icons.h :

#ifndef ICONS_H
#define ICONS_H

#include <LiquidCrystal.h>

// Наші власні номери піктограм
#define ICON_LOCKED_CHAR   (byte)0
#define ICON_UNLOCKED_CHAR (byte)1

// Це стандартна піктограма на наборі символів LCD1602
#define ICON_RIGHT_ARROW   (byte)126

void init_icons(LiquidCrystal &lcd);

#endif /* ICONS_H */

Код виконавчого файлу SafeState.cpp :

#include <Arduino.h>
#include <EEPROM.h>
#include "SafeState.h"

/* Safe state */
#define EEPROM_ADDR_LOCKED   0
#define EEPROM_ADDR_CODE_LEN 1
#define EEPROM_ADDR_CODE     2
#define EEPROM_EMPTY         0xff

#define SAFE_STATE_OPEN (char)0
#define SAFE_STATE_LOCKED (char)1

SafeState::SafeState() {
  this->_locked = EEPROM.read(EEPROM_ADDR_LOCKED) == SAFE_STATE_LOCKED;
}

void SafeState::lock() {
  this->setLock(true);
}

bool SafeState::locked() {
  return this->_locked;
}

bool SafeState::hasCode() {
  auto codeLength = EEPROM.read(EEPROM_ADDR_CODE_LEN);
  return codeLength != EEPROM_EMPTY;
}

void SafeState::setCode(String newCode) {
  EEPROM.write(EEPROM_ADDR_CODE_LEN, newCode.length());
  for (byte i = 0; i < newCode.length(); i++) {
    EEPROM.write(EEPROM_ADDR_CODE + i, newCode[i]);
  }
}

bool SafeState::unlock(String code) {
  auto codeLength = EEPROM.read(EEPROM_ADDR_CODE_LEN);
  if (codeLength == EEPROM_EMPTY) {
    // There was no code, so unlock always succeeds
    this->setLock(false);
    return true;
  }
  if (code.length() != codeLength) {
    return false;
  }
  for (byte i = 0; i < code.length(); i++) {
    auto digit = EEPROM.read(EEPROM_ADDR_CODE + i);
    if (digit != code[i]) {
      return false;
    }
  }
  this->setLock(false);
  return true;
}

void SafeState::setLock(bool locked) {
  this->_locked = locked;
  EEPROM.write(EEPROM_ADDR_LOCKED, locked ? SAFE_STATE_LOCKED : SAFE_STATE_OPEN);
}

Код виконавчого файлу відображення спец-символів icons.cpp :

#include <Arduino.h>
#include "icons.h"

const byte iconLocked[8] PROGMEM = {
  0b01110,
  0b10001,
  0b10001,
  0b11111,
  0b11011,
  0b11011,
  0b11111,
};

const byte iconUnlocked[8] PROGMEM = {
  0b01110,
  0b10000,
  0b10000,
  0b11111,
  0b11011,
  0b11011,
  0b11111,
};

void init_icons(LiquidCrystal &lcd) {
  byte icon[8];
  memcpy_P(icon, iconLocked, sizeof(icon));
  lcd.createChar(ICON_LOCKED_CHAR, icon);
  memcpy_P(icon, iconUnlocked, sizeof(icon));
  lcd.createChar(ICON_UNLOCKED_CHAR, icon);
}
