#include <Servo.h> //используем библиотеку для работы с сервоприводом

Servo servo0; //объявляем servo0
Servo servo1; //объявляем servo1
Servo servo2; //объявляем servo2
Servo servo3; //объявляем servo3
Servo servo4; //объявляем servo4
Servo servo5; //объявляем servo5
Servo servo6; //объявляем servo6
Servo servo7; //объявляем servo7
Servo servo8; //объявляем servo8

void setup(){
  servo0.attach(0); //привязываем привод к порту 8
  servo1.attach(1); //привязываем привод к порту 8
  servo2.attach(2); //привязываем привод к порту 8
  servo3.attach(3); //привязываем привод к порту 8
  servo4.attach(4); //привязываем привод к порту 8
  servo5.attach(5); //привязываем привод к порту 8
  servo6.attach(6); //привязываем привод к порту 8
  servo7.attach(7); //привязываем привод к порту 8
  servo8.attach(8); //привязываем привод к порту 8
  delay(7000);
}

void servos(int i) {
      servo0.write(i);
      servo1.write(i);
      servo2.write(i);
      servo3.write(i);
      servo4.write(i);
      servo5.write(i);
      servo6.write(i);
      servo7.write(i);
      servo8.write(i);
}

void loop(void) {
  for (int i=0; i<180; i++) {
  servos(i);
  delay(10);  
  }
  for (int i=180; i>1; i--) {
  servos(i);
  delay(10);    
  }
}


