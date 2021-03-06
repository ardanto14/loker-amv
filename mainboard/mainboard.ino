#include <MFRC522.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "DS3231.h"

bool cardIsValid(String message);
void printHelloMessage();
void printFailMessage();
void printSuccessMessage(String username);
void printUnlocked(String username);
void printLocked();
void relayBoxOpen();
void relayBoxClose();
void printToLog(String username);
void soundBuzzer(byte type);


// define constant for soundBuzzer method
#define SUCCESS 100
#define FAILED 101
#define OPEN_LOCKER 102
#define REMOVE_CARD 103
#define OPEN 104
#define CLOSE 105

// pin for relay
#define RELAY_PIN 23

// the pin of the active buzzer
#define BUZZER_PIN 12

// pin for mfrc522
#define RST_RFID 9   
#define SS_RFID 10

// this should be included before anything except import
File myFile;
RTClib RTC;
DS3231 clocker;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 mfrc522(SS_RFID, RST_RFID);

void setup() {
  Serial.begin(9600);
  Serial1.begin(19200);
  
  Wire.begin();
  SPI.begin();
  
  pinMode(SS_RFID, OUTPUT);

  /*------- for relay -------*/
  pinMode(RELAY_PIN, OUTPUT);


  /*------- for lcd -------*/
  // initialize the LCD
  lcd.begin();
  // turn on back light
  lcd.backlight();

  /*------- for rtc -------*/
  // uncomment this if you want to set rtc clock
  //clocker.setSecond(0);
  //clocker.setMinute(53);
  //clocker.setHour(18);
  //clocker.setDate(11);
  //clocker.setMonth(7);
  //clocker.setYear(18);
  //clocker.setDoW(3);
  //clocker.setClockMode(false); // set to 24 h

  /*------- for buzzer -------*/
  pinMode(BUZZER_PIN ,OUTPUT);

  mfrc522.PCD_Init();
  Serial.println("TAP YOUR CARD!");
  printHelloMessage();
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
    
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  String content= "";
  
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  content.toUpperCase();

  DateTime now = RTC.now();
  unsigned long startTime = now.unixtime();
  unsigned long waitTime = startTime + 180;
  if (cardIsValid(content.substring(1))) {
    Serial.println("IDENTIFICATION SUCCESS");
    printToLog(getName(content.substring(1)), OPEN);
    printSuccessMessage(getName(content.substring(1)));
    relayBoxOpen();
    soundBuzzer(SUCCESS);
    delay(3000);
    printUnlocked(getName(content.substring(1)));
    while (true) {
      if(!mfrc522.PICC_IsNewCardPresent()) {
        now = RTC.now();
        if (now.unixtime() >= waitTime) {
          startTime = now.unixtime();
          waitTime = startTime + 180;
          soundBuzzer(OPEN_LOCKER);
        }
        continue;
      }

      if (!mfrc522.PICC_ReadCardSerial()) {
        now = RTC.now();
        if (now.unixtime() >= waitTime) {
          startTime = now.unixtime();
          waitTime = startTime + 180;
          soundBuzzer(OPEN_LOCKER);
        }
        continue;
      }

      String newcontent = "";
      
      for (byte i = 0; i < mfrc522.uid.size; i++) 
      {
        newcontent.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        newcontent.concat(String(mfrc522.uid.uidByte[i], HEX));
      }

      newcontent.toUpperCase();
      
      if (newcontent.substring(1) == content.substring(1)) {
        Serial.println("Locker successfully locked");
        printToLog(getName(content.substring(1)), CLOSE);
        printLocked();
        relayBoxClose();
        soundBuzzer(REMOVE_CARD);
        delay(1000);
        printHelloMessage();
        return;
      } else {
        Serial.println("Card is different");
        printFailMessage();
        soundBuzzer(FAILED);
        delay(1000);
        printUnlocked(getName(content.substring(1)));
      }

    }
  } else {
    Serial.println("IDENTIFICATION FAILED, TRY AGAIN");
    soundBuzzer(FAILED);
    printFailMessage();
    delay(1000);
    printHelloMessage();
  }
}

// --- helper method start here

/* method for return the username based on uid number */
String getName(String id) {
  String uidList[] = {"F8 69 0F 89", "FA 92 E7 B6", "CA 97 DF B6",
        "FA 96 DF B6", "DA D1 D3 B6", "6A BE D1 B6", "7A B7 B4 B3",
        "EA D8 D9 B6", "0A D9 D9 B6", "EA 04 DB B6", "CA 4D CF B6",
        "9A 51 D7 B6", "7A 50 D7 B6", "DA E9 CD B6", "9A F4 E6 B6",
        "5A D1 DD B6", "D0 26 12 A3"};
  String nameList[] = {"Alif", "Goldy", "Grace",
        "Fikih", "Agoy", "Farol", "Muffin",
        "Josu", "Auli", "Doni", "Fahmi",
        "Irfan", "Suryadi", "Fajar", "Gita",
        "Dhafin", "Danto"};
  for (int i = 0; i < sizeof(uidList); i++) {
    if (id == uidList[i]) {
      return nameList[i];
    }
  }
  return "unknown";
}

/* method to check if the given uid is valid */
bool cardIsValid(String message) {
  String uidList[] = {"F8 69 0F 89", "FA 92 E7 B6", "CA 97 DF B6",
        "FA 96 DF B6", "DA D1 D3 B6", "6A BE D1 B6", "7A B7 B4 B3",
        "EA D8 D9 B6", "0A D9 D9 B6", "EA 04 DB B6", "CA 4D CF B6",
        "9A 51 D7 B6", "7A 50 D7 B6", "DA E9 CD B6", "9A F4 E6 B6",
        "5A D1 DD B6", "D0 26 12 A3"};
  for (int i = 0; i < 17; i++) {
    if (message == uidList[i]) {
      return true;
    }
  }
  return false;
}

/* method to print hello message to the lcd */
void printHelloMessage() {
  lcd.clear(); // clear the lcd
  lcd.setCursor(0,0); // set cursor to row 0 column 0
  lcd.print("Hello, Tap your"); // write message
  lcd.setCursor(0, 1); // seet cursor to row 1 column 0
  lcd.print("key to open"); // write message
}


/* method to print fail message to the lcd */
void printFailMessage() {
  lcd.clear(); // clear the lcd
  lcd.setCursor(0,0); // set cursor to row 0 column 0
  lcd.print("Key Error!"); // write message
  lcd.setCursor(0, 1); // seet cursor to row 1 column 0
  lcd.print("Card isn't valid"); // write message
}


/* method to print success message to the lcd
 * receive param name in String to print it to lcd
*/
void printSuccessMessage(String username) {
  String final = "Hi, " + username + "!"; // variable to be printed
  //strcpy(final, "Hi, "); // copy message to row
  //strcat(final, name); // concatenate message and name
  //strcat(final, "!");
  lcd.clear(); // clear the lcd
  lcd.setCursor(0,0); // set cursor to row 0 column 0
  lcd.print(final); // write message
  lcd.setCursor(0, 1); // set cursor to row 1 column 0
  lcd.print("Close after use"); // write message
}


/* method to print that locker is unlocked to the lcd */
void printUnlocked(String username) {
  lcd.clear(); // clear the lcd
  lcd.setCursor(0,0); // set cursor to row 0 column 0
  lcd.print("Unlocked by:"); // write message
  lcd.setCursor(0, 1); // seet cursor to row 1 column 0
  lcd.print(username); // write message
}


/* method to print that locker is unlocked to the lcd */
void printLocked() {
  lcd.clear(); // clear the lcd
  lcd.setCursor(0,0); // set cursor to row 0 column 0
  lcd.print("Locker is locked"); // write message
  lcd.setCursor(0, 1); // seet cursor to row 1 column 0
  lcd.print("Thanks for using"); // write message
}

/* method for activate relays */
void relayBoxOpen() {
  digitalWrite(RELAY_PIN, HIGH);
}

/* method for deactivate relays */
void relayBoxClose() {
  digitalWrite(RELAY_PIN, LOW);
}

/* method to save the user log data */
void printToLog(String username, byte type) {
  DateTime now = RTC.now();

  // if the file opened okay, write to it:
  if (type == OPEN) {
    Serial1.print("OPEN ");
  } else if (type == CLOSE) {
    Serial1.print("CLOSE ");
  }
  Serial1.print(now.year());
  Serial1.print("-");
  Serial1.print(now.month());
  Serial1.print("-");
  Serial1.print(now.day());
  Serial1.print(" ");
  if (now.second() < 10) {
    Serial1.print(0, DEC);
  }
  Serial1.print(now.hour());
  Serial1.print(":");
  if (now.second() < 10) {
    Serial1.print(0, DEC);
  }
  Serial1.print(now.minute());
  Serial1.print(":");
  if (now.second() < 10) {
    Serial1.print(0, DEC);
  }
  Serial1.print(now.second());
  Serial1.print(" ");
  Serial1.println(username);
  if (type == CLOSE) {
    Serial1.println("-----");
  }
}

/* method for setting the buzzer sound */
void soundBuzzer(byte type) {
  if (type == SUCCESS) {
    for (int y = 0; y < 3; y++){
      digitalWrite(BUZZER_PIN, HIGH) ;// Buzzer On
      delay(50) ;// Delay 1ms 
      digitalWrite(BUZZER_PIN, LOW) ;// Buzzer Off
      delay(50) ;// delay 1ms
      }
  } else if (type == FAILED) {
    for (int y = 0; y < 1; y++){
      digitalWrite(BUZZER_PIN, HIGH) ;// Buzzer On
      delay(1000) ;// Delay 1ms 
      digitalWrite(BUZZER_PIN, LOW) ;// Buzzer Off
      delay(1000) ;// delay 1ms
      }
  } else if (type == OPEN_LOCKER) {
    for (int y = 0; y < 2; y++){
      digitalWrite(BUZZER_PIN, HIGH) ;// Buzzer On
      delay(100) ;// Delay 1ms 
      digitalWrite(BUZZER_PIN, LOW) ;// Buzzer Off
      delay(100) ;// delay 1ms
      }
  } else if (type == REMOVE_CARD) {
    for (int y = 0; y < 4; y++){
      digitalWrite(BUZZER_PIN, HIGH) ;// Buzzer On
      delay(150) ;// Delay 1ms 
      digitalWrite(BUZZER_PIN, LOW) ;// Buzzer Off
      delay(300) ;// delay 1ms
      }
  }
}
