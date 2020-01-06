byte valueFromKeyBoard;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);

  pinMode(13, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {

  if (Serial.available() > 0) {
    valueFromKeyBoard = Serial.read();


    if ('1' == valueFromKeyBoard) {
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)

      //Serial.write('1');

      digitalWrite(13, HIGH);
    }

    if ('0' == valueFromKeyBoard) {
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW

      //Serial.write('0');
      digitalWrite(13, LOW);
    }
    //Serial.print('Hello, World ');
  }



  delay(3);
}
