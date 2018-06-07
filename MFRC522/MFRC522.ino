#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN= 9;    
constexpr uint8_t SS_PIN = 10;    

MFRC522 mfrc522(SS_PIN, RST_PIN);

boolean cardIsValid(String message);

void setup() 
{
  Serial.begin(9600);
  SPI.begin(); 
  mfrc522.PCD_Init();
}

void loop() 
{
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
    
  if (!mfrc522.PICC_ReadCardSerial()) {
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
  
  if (cardIsValid(content.substring(1)))
  {
    Serial.println("IDENTIFICATION SUCCESS");           
    delay(3000);
  } 
  else {
    Serial.println("IDENTIFICATION FAILED, TRY AGAIN");
    delay(3000);
  }
}

boolean cardIsValid(String message) {
  String uidList[] = {"F8 69 0F 89", "FA 92 E7 B6", "CA 97 DF B6", "FA 96 DF B6", "DA D1 D3 B6", "6A BE D1 B6", "7A B7 B4 B3", "EA D8 D9 B6", "0A D9 D9 B6", "EA 04 DB B6", "CA 4D CF B6", 
  "9A 51 D7 B6", "7A 50 D7 B6", "DA E9 CD B6", "9A F4 E6 B6", "5A D1 DD B6", "D0 26 12 A3"};
  for (int i = 0; i < 17; i++) {
    if (message == uidList[i]) {
      return true;
    }
  }
  return false;
}

