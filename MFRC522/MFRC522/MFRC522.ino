#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN= 9;    
constexpr uint8_t SS_PIN = 10;    

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() 
{
  Serial.begin(9600);
  SPI.begin(); 
  mfrc522.PCD_Init();
  Serial.println(F("TAP YOUR CARD!"));
  Serial.println(F(" "));
}

void loop() 
{
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
  byte block;
  byte len;
  MFRC522::StatusCode status;
  
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;

  Serial.println(F("CARD DETECTED"));
  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid));     //dump some details about the card
  Serial.print(F("NAME: "));

  byte buffer1[18];

  block = 4;
  len = 18;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
  if (status != MFRC522::STATUS_OK) 
  {
    Serial.print(F("AUTHENTICATION FAILED: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  status = mfrc522.MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK) 
  {
    Serial.print(F("READING FAILED: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  for (uint8_t i = 0; i < 16; i++)
  {
    if (buffer1[i] != 32)
    {
      Serial.write(buffer1[i]);
    }
  }
  Serial.print(" ");
  
  byte buffer2[18];
  block = 1;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) 
  {
    Serial.print(F("AUTHENTICATION FAILED: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer2, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("READING FAILED: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  
  for (uint8_t i = 0; i < 16; i++) {
    Serial.write(buffer2[i] );
  }
  
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if ((content.substring(1) == "F8 69 0F 89")||(content.substring(1) == "FA 92 E7 B6")||    //verify NUID AMV Member
  (content.substring(1) == "CA 97 DF B6")||(content.substring(1) == "FA 96 DF B6")||
  (content.substring(1) == "DA D1 D3 B6")||(content.substring(1) == "6A BE D1 B6")||
  (content.substring(1) == "7A B7 B4 B3")||(content.substring(1) == "EA D8 D9 B6")||
  (content.substring(1) == "0A D9 D9 B6")||(content.substring(1) == "EA 04 DB B6")||
  (content.substring(1) == "CA 4D CF B6")||(content.substring(1) == "9A 51 D7 B6")||
  (content.substring(1) == "7A 50 D7 B6")||(content.substring(1) == "DA E9 CD B6")||
  (content.substring(1) == "9A F4 E6 B6")||(content.substring(1) == "5A D1 DD B6")||
  (content.substring(1) == "D0 26 12 A3"))
  {
    Serial.println("IDENTIFICATION SUCCESS");           
    Serial.println();
    delay(3000);
  }
 
 else   {
    Serial.println("IDENTIFICATION FAILED, TRY AGAIN");
    delay(3000);
  }
  
  delay(1000);
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
