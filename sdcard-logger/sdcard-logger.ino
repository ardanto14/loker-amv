/*
 ** MOSI - pin 11
 ** MOSO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
 */

#include <SPI.h>
#include <SD.h>
#include "DS3231.h"

// this should be included before anything except import
File myFile;
RTClib rtc;

void printToLog(char username[]);

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // include this in setup
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
  }
}

void loop() {
  printToLog("ngetest");
  delay(1000);
}

void printToLog(char username[]) {
  DateTime now = rtc.now();
  
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

