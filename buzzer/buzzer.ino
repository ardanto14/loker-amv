int Buzzer = 12;//the pin of the active buzzer
void setup()
{
  pinMode(Buzzer,OUTPUT);//initialize the buzzer pin as an output
}
void loop()
{
  // for success
  for (int y = 0; y < 3; y++){
    digitalWrite (Buzzer, HIGH) ;// Buzzer On
    delay (50) ;// Delay 1ms 
    digitalWrite (Buzzer, LOW) ;// Buzzer Off
    delay (50) ;// delay 1ms
    }
  delay(1000);

  // for failed
  for (int y = 0; y < 1; y++){
    digitalWrite (Buzzer, HIGH) ;// Buzzer On
    delay (1000) ;// Delay 1ms 
    digitalWrite (Buzzer, LOW) ;// Buzzer Off
    delay (1000) ;// delay 1ms
    }
  delay(1000);

  // for opening locker
  for (int y = 0; y < 2; y++){
    digitalWrite (Buzzer, HIGH) ;// Buzzer On
    delay (100) ;// Delay 1ms 
    digitalWrite (Buzzer, LOW) ;// Buzzer Off
    delay (100) ;// delay 1ms
    }
  delay(1000);

  // for remove card
  for (int y = 0; y < 4; y++){
    digitalWrite (Buzzer, HIGH) ;// Buzzer On
    delay (150) ;// Delay 1ms 
    digitalWrite (Buzzer, LOW) ;// Buzzer Off
    delay (300) ;// delay 1ms
    }
  delay(1000);
}
