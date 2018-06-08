/*
 ** MOSI - pin 11
 ** MOSO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
 */
 
#include <MFRC522.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "DS3231.h"

// this should be included before anything except import
File myFile;
RTClib RTC;
DS3231 clocker;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// total pins used by relay
const int totalPins = 1;

// array of relays' pin numbers
const int boxRelayPins[totalPins] = {23};

// the pin of the active buzzer
const int Buzzer = 12;

// pin for mfrc522
constexpr uint8_t RST_PIN= 9;    
constexpr uint8_t SS_PIN = 10; 
MFRC522 mfrc522(SS_PIN, RST_PIN);

// a map where the keys are hex strings and the values are strings
std::map<std::string, std::string> userName;
 
// insert users
userName["F8 69 0F 89"] = std::string("Alif");
userName["FA 96 DF B6"] = std::string("Fikih");
userName["FA 92 E7 B6"] = std::string("Goldy");

userName["DA D1 D3 B6"] = std::string("Agoy");
userName["6A BE D1 B6"] = std::string("Farol");
userName["0A D9 D9 B6"] = std::string("Auli");
userName["CA 97 DF B6"] = std::string("Grace");

userName["7A B7 B4 B3"] = std::string("Muffin");
userName["EA D8 D9 B6"] = std::string("Josu");
userName["EA 04 DB B6"] = std::string("Doni");

userName["CA 4D CF B6"] = std::string("Fahmi");
userName["9A 51 D7 B6"] = std::string("Irfan");
userName["7A 50 D7 B6"] = std::string("Suryadi");
userName["DA E9 CD B6"] = std::string("Fajar");

userName["9A F4 E6 B6"] = std::string("Gita");
userName["5A D1 DD B6"] = std::string("Dafin");
userName["D0 26 12 A3"] = std::string("Danto");

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Wire.begin();
  SPI.begin(); 

  /*------- for relay -------*/
  // initialize digital relays' pin numbers as outputs.
  for (int i = 0; i < totalPins; i++) {
    pinMode(boxRelayPins[i], OUTPUT);
  }

  /*------- for lcd -------*/
  // initialize the LCD
  lcd.begin();
  // turn on back light
  lcd.backlight();

  /*------- for rtc -------*/
  // uncomment this if you want to set rtc clock
  //clocker.setSecond(0);
  //clocker.setMinute(13);
  //clocker.setHour(22);
  //clocker.setDate(28);
  //clocker.setMonth(5);
  //clocker.setYear(18);
  //clocker.setDoW(1);
  //clocker.setClockMode(false); // set to 24 h

  /*------- for sdcard logger -------*/
  // include this in setup
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
  }

  /*------- for buzzer -------*/
  //initialize the buzzer pin as an output
  pinMode(Buzzer,OUTPUT);

  mfrc522.PCD_Init();
  Serial.println(F("TAP YOUR CARD!"));
  Serial.println(F(" "));
  printHelloMessage();
}

void loop() {
  // put your main code here, to run repeatedly:

  /* ------------------run mfrc522------------------------ */
  if (! (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())) {
    printHelloMessage();
    return;
  }
  
  String content= "";
  byte letter;
  
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  content.toUpperCase();

  bool untapped = false;
  DateTime now = RTC.now();
  int startTime = now.unixtime();
  int waitTime = startTime + 180;
  if (cardIsValid(content.substring(1))) {
    Serial.println("IDENTIFICATION SUCCESS");
    buzzerSound(3);
    printToLog(userName[content.substring(1)]);
    relayBoxOpen();
    printSuccessMessage(userName[content.substring(1)]);
    while (true) {
      while (! (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())) {
        now = RTC.now();
        untapped = true;
        if (now.unixTime() == waitTime) {
          soundBuzzer(2);
          startTime = now.unixtime();
          waitTime = startTime + 180;
          printUnlocked();
        }
      }
      if (untapped) {
        String newcontent= "";
        byte newletter;
        
        for (byte i = 0; i < mfrc522.uid.size; i++) 
        {
          newcontent.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
          newcontent.concat(String(mfrc522.uid.uidByte[i], HEX));
        }
        if (newcontent.substring(1) == content.substring(1)) {
          soundBuzzer(4);
          printLocked();
          relayBoxClose();
          delay(5000);
          return;
        } else {
          printFailMessage(0);
        }
      }
    }
  } else {
    Serial.println("IDENTIFICATION FAILED, TRY AGAIN");
    buzzerSound(1);
    printFailMessage();
    delay(3000);
  }
  /* ------------------------------------------ */
}

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
void printSuccessMessage(char name[]) {
  uint8_t final[16]; // variable to be printed
  strcpy(final, "Hi, "); // copy message to row
  strcat(final, name); // concatenate message and name
  strcat(final, "!");
  lcd.clear(); // clear the lcd
  lcd.setCursor(0,0); // set cursor to row 0 column 0
  lcd.print((char*) final); // write message
  lcd.setCursor(0, 1); // set cursor to row 1 column 0
  lcd.print("Close after use"); // write message
}


/* method to print that locker is unlocked to the lcd */
void printUnlocked() {
  lcd.clear(); // clear the lcd
  lcd.setCursor(0,0); // set cursor to row 0 column 0
  lcd.print("Attention!"); // write message
  lcd.setCursor(0, 1); // seet cursor to row 1 column 0
  lcd.print("Locker is unlocked!"); // write message
}


/* method to print that locker is unlocked to the lcd */
void printLocked() {
  lcd.clear(); // clear the lcd
  lcd.setCursor(0,0); // set cursor to row 0 column 0
  lcd.print("Attention!"); // write message
  lcd.setCursor(0, 1); // seet cursor to row 1 column 0
  lcd.print("Locker is locked"); // write message
}

/* method for activate relays */
void relayBoxOpen(int boxNumber) {
  for (int i = 0; i < totalPins; i++) {
    digitalWrite(boxRelayPins[boxNumber], HIGH);
    delay(100);
  }
}

/* method for deactivate relays */
void relayBoxClose(int boxNumber) {
  for (int i = 0; i < totalPins; i++) {
    digitalWrite(boxRelayPins[boxNumber], LOW);
    delay(100);
  }
}

/* method to save the user log data */
void printToLog(char username[]) {
  DateTime now = RTC.now();
  
  SD.remove("log.txt"); // uncomment for the real code
  
  myFile = SD.open("log.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    // writing file begin here
    myFile.print(now.year());
    myFile.print("-");
    myFile.print(now.month());
    myFile.print("-");
    myFile.print(now.day());
    myFile.print(" ");
    if (now.second() < 10) {
      myFile.print(0, DEC);
    }
    myFile.print(now.hour());
    myFile.print(":");
    if (now.second() < 10) {
      myFile.print(0, DEC);
    }
    myFile.print(now.minute());
    myFile.print(":");
    if (now.second() < 10) {
      myFile.print(0, DEC);
    }
    myFile.print(now.second());
    // close the file:
    myFile.print(" ");
    myFile.println((char*) username);
    myFile.close(); // dont forget to cloaw
  } else {
    Serial.println("Error writing");
  }
  
  myFile = SD.open("log.txt");
  if (myFile) {
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    Serial.println("Error open");
  }
}

/* method for print clock on lcd */
void runRTC() {
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
  //delay(1000);
}

/* method for setting the buzzer sound */
void buzzerSound(int type) {
  // 3 success, 1 failed, 2 opening locker, 4 remove cards
  if (type == 3) {
    // for success
    for (int y = 0; y < 3; y++){
      digitalWrite (Buzzer, HIGH) ;// Buzzer On
      delay (50) ;// Delay 1ms 
      digitalWrite (Buzzer, LOW) ;// Buzzer Off
      delay (50) ;// delay 1ms
      }
    delay(1000);
  } else if (type == 1) {
    // for failed
    for (int y = 0; y < 1; y++){
      digitalWrite (Buzzer, HIGH) ;// Buzzer On
      delay (1000) ;// Delay 1ms 
      digitalWrite (Buzzer, LOW) ;// Buzzer Off
      delay (1000) ;// delay 1ms
      }
    delay(1000);
  } else if (type == 2) {
    // for opening locker
    for (int y = 0; y < 2; y++){
      digitalWrite (Buzzer, HIGH) ;// Buzzer On
      delay (100) ;// Delay 1ms 
      digitalWrite (Buzzer, LOW) ;// Buzzer Off
      delay (100) ;// delay 1ms
      }
    delay(1000);
  } else if (type == 4) {
    // for remove card
    for (int y = 0; y < 4; y++){
      digitalWrite (Buzzer, HIGH) ;// Buzzer On
      delay (150) ;// Delay 1ms 
      digitalWrite (Buzzer, LOW) ;// Buzzer Off
      delay (300) ;// delay 1ms
      }
    delay(1000);
  }
}


