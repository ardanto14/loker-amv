#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void printHelloMessage();
void printFailMessage();
void printSuccessMessage(uint8_t name[]);

void setup()
{
  // initialize the LCD
  lcd.begin();
  
  // turn on back light
  lcd.backlight();
}

void loop()
{
  printHelloMessage();
  delay(1000);
  printFailMessage();
  delay(1000);
  printSuccessMessage("akoh");
  delay(1000);
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
void printSuccessMessage(uint8_t name[]) {
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



