#include <Wire.h>
#include <DS3231.h>

DS3231 Clock;

int Year;
int Month;
int Date;
int DoW;
int Hour;
int Minute;
int Second;

bool Century = false;
bool h12 = false;
bool PM = false;

void setup () {
    Clock.setClockMode(false);  // set to 24h
    //setClockMode(true); // set to 12h

    Clock.setYear(Year);
    Clock.setMonth(Month);
    Clock.setDate(Date);
    Clock.setDoW(DoW);
    Clock.setHour(Hour);
    Clock.setMinute(Minute);
    Clock.setSecond(Second);
    Wire.begin();
}

void loop () {
  
    delay(1000);
  
    Serial.print(Clock.getYear(), DEC);
    Serial.print("-");
    Serial.print(Clock.getMonth(Century), DEC);
    Serial.print("-");
    Serial.print(Clock.getDate(), DEC);
    Serial.print(" ");
    Serial.print(Clock.getHour(h12, PM), DEC); //24-hr
    Serial.print(":");
    Serial.print(Clock.getMinute(), DEC);
    Serial.print(":");
    Serial.println(Clock.getSecond(), DEC);
    Clock.getDoW();
}
