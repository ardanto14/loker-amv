#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

uint8_t HELLO_MESSAGE_ROW_1[] = "Hello, Tap your"; // hello message for row 1
uint8_t HELLO_MESSAGE_ROW_2[] = "key to open"; // hello message for row 2

// fail message if the card is not valid
uint8_t FAIL_MESSAGE_ROW_1[] = "Key Error!"; // fail message for row 1
uint8_t FAIL_MESSAGE_ROW_2[] = "Card is not valid"; // fail message for row 2

// success message if the locker is successfully open
uint8_t SUCCESS_MESSAGE_ROW_1[] = "Hi: "; // success message for row 1
uint8_t SUCCESS_MESSAGE_ROW_2[] = "Close after use"; // success message for row 2

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void printHelloMessage();
void printFailMessage();
void printSuccessMessage(String name);

void setup()
{
  // initialize the LCD
  lcd.begin();
  
  // turn on back light
  lcd.backlight();
}

void loop()
{
  // define this later
}

/* method to print hello message to the lcd */
void printHelloMessage() {
  lcd.clear(); // clear the lcd
  lcd.setCursor(0,0); // set cursor to row 0 column 0
  lcd.write(HELLO_MESSAGE_ROW_1); // write message
  lcd.setCursor(0, 1); // seet cursor to row 1 column 0
  lcd.write(HELLO_MESSAGE_ROW_2); // write message
}


/* method to print fail message to the lcd */
void printFailMessage() {
  lcd.clear(); // clear the lcd
  lcd.setCursor(0,0); // set cursor to row 0 column 0
  lcd.write(FAIL_MESSAGE_ROW_1); // write message
  lcd.setCursor(0, 1); // seet cursor to row 1 column 0
  lcd.write(FAIL_MESSAGE_ROW_2); // write message
}


/* method to print success message to the lcd
 * receive param name in String to print it to lcd
*/
void printSuccessMessage(uint8_t name[]) {
  uint8_t final[16]; // variable to be printed
  strcpy(final, SUCCESS_MESSAGE_ROW_1); // copy message to row
  strcat(final, name); // concatenate message and name
  lcd.clear(); // clear the lcd
  lcd.setCursor(0,0); // set cursor to row 0 column 0
  lcd.write(final); // write message
  lcd.setCursor(0, 1); // set cursor to row 1 column 0
  lcd.write(SUCCESS_MESSAGE_ROW_2); // write message
}



