/*
 ** MOSI - pin 11
 ** MOSO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
 */

#include <SPI.h>
#include <SD.h>

// this should be included before anything except import
File myFile;

void printToLog(char username[]);

void setup() {
  Serial.begin(9600);

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
  
  SD.remove("log.txt"); // uncomment for the real code
  
  myFile = SD.open("log.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    // writing file begin here
    myFile.println("aku");
    myFile.close(); // dont forget to close
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

