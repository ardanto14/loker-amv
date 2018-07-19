#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "DS3231.h"

RTClib RTC;
DS3231 clocker;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup () {
    Serial.begin(9600);
    Wire.begin();
    lcd.begin();
    lcd.backlight();
    // uncomment this if you want to set rtc clock
    clocker.setSecond(0);
    clocker.setMinute(13);
    clocker.setHour(22);
    clocker.setDate(28);
    clocker.setMonth(5);
    clocker.setYear(18);
    clocker.setDoW(1);
    clocker.setClockMode(false); // set to 24 h
}

void loop () {
    DateTime now = RTC.now();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(now.year(), DEC);
    lcd.print('/');
    lcd.print(now.month(), DEC);
    lcd.print('/');
    lcd.print(now.day(), DEC);
    lcd.setCursor(0, 1);
    if (now.hour() < 10) {
      lcd.print(0, DEC);
    }
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    if (now.minute() < 10) {
      lcd.print(0, DEC);
    }
    lcd.print(now.minute(), DEC);
    lcd.print(':');
    if (now.second() < 10) {
      lcd.print(0, DEC);
    }
    lcd.print(now.second(), DEC);
    delay(1000);
}
