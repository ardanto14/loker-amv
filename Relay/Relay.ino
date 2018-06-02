/*
  RELAY SELECTOR

  @author : Razaqa Dhafin Haffiyan
  @date  : May 26th, 2018

*/

// total pins used by relay
const int totalPins = 1;

// array of relays' pin numbers
const int boxRelayPins[totalPins] = {23};

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital relays' pin numbers as outputs.
  for (int i = 0; i < totalPins; i++) {
    pinMode(boxRelayPins[i], OUTPUT);
  }
}

// the loop function runs over and over again forever
void loop() {
  //call openBox
  //openBox();
  //closeBox();
}

void openBox(int boxNumber) {
  for (int i = 0; i < totalPins; i++) {
    digitalWrite(boxRelayPins[boxNumber], HIGH);
    delay(100);
  }
}

void closeBox(int boxNumber) {
  for (int i = 0; i < totalPins; i++) {
    digitalWrite(boxRelayPins[boxNumber], LOW);
    delay(100);
  }
}

