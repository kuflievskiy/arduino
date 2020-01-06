
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);

  // connect the A0 to the 5V 
  // connect the A0 to the GND via resistor 
  //pinMode(A0, INPUT);
  
  // connect the A0 to the GND 
  pinMode(A0, INPUT_PULLUP);
}

// the loop function runs over and over again forever
void loop() {

  if (Serial.available() > 0) {
    //Serial.println('Hello, World ');
    
    boolean btn1 = digitalRead(A0);
    //Serial.print('Button: ');
    Serial.println(btn1);
  }



  delay(3);
}
