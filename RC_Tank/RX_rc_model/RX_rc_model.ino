/**
  Custom RC Model Robot.
  Receiver part.

  List of modules used: 
    - Arduino UNO (microcontroler)
    - nRF24L01 (radio module)
    - L298N (H-bridge motor controller module)
  
  Author: Kuflievskiy Aleksey kuflievskiy@gmail.com
*/

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9, 10); // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; // All available pipes numbers

const byte THRESHOLD_X_START_CENTERZONE = 120;
const byte THRESHOLD_X_END_CENTERZONE = 130;
const byte THRESHOLD_Y_START_CENTERZONE = 120;
const byte THRESHOLD_Y_END_CENTERZONE = 130;
const byte SENSOR_MAX_VALUE = 255;

const byte DEBUG_MODE = 1;

void setup() {

  // left motor setup
  pinMode(7, OUTPUT); // IN1
  pinMode(6, OUTPUT); // IN2
  pinMode(5, OUTPUT); // EN1
  // right motor setup
  pinMode(4, OUTPUT); // IN3
  pinMode(3, OUTPUT); // EN2
  pinMode(2, OUTPUT);// IN4

  Serial.begin(9600); //открываем порт для связи с ПК
  radio.begin(); //активировать модуль
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах

  radio.openReadingPipe(1, address[0]);     //хотим слушать трубу 0
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp(); //начать работу
  radio.startListening();  //начинаем слушать эфир, мы приёмный модуль

  if (DEBUG_MODE) {
    Serial.println("setup done");
  }

}

void loop(void) {


  //Serial.println("loop");
  byte pipeNo;
  byte controlData[3] = {};
  while ( radio.available(&pipeNo)) {  // слушаем эфир со всех труб

    radio.read( &controlData, sizeof(controlData) );         // чиатем входящий сигнал

    Serial.print("Recieved: ");
    Serial.println("X: ");
    Serial.println(controlData[0]);
    Serial.println("Y: ");
    Serial.println(controlData[1]);
    Serial.println("Button: ");
    Serial.println(controlData[2]);

    byte isButtonClicked = !controlData[2];

    int xCoordinate = controlData[0];
    int yCoordinate = controlData[1];

    if (
      ( xCoordinate < THRESHOLD_X_END_CENTERZONE and xCoordinate > THRESHOLD_X_START_CENTERZONE ) and
      ( yCoordinate < THRESHOLD_Y_END_CENTERZONE and yCoordinate > THRESHOLD_Y_START_CENTERZONE )
    ) {

      if (DEBUG_MODE) {
        Serial.println("Center position. Do nothing.");
      }
      stopMotor('left');
      stopMotor('right');
    }


    // full-forward
    if (
      ( xCoordinate < THRESHOLD_X_END_CENTERZONE and xCoordinate > THRESHOLD_X_START_CENTERZONE ) and
      ( yCoordinate < THRESHOLD_Y_START_CENTERZONE )
    ) {
      if (DEBUG_MODE) {
        Serial.println("full-forward");
      }
      runMotor('right', 'forward', 255);
      runMotor('left', 'forward', 255);
    }

    // full-backward
    if (
      ( xCoordinate < THRESHOLD_X_END_CENTERZONE and xCoordinate > THRESHOLD_X_START_CENTERZONE ) and
      ( yCoordinate > THRESHOLD_Y_END_CENTERZONE and yCoordinate <= SENSOR_MAX_VALUE )
    ) {
      if (DEBUG_MODE) {
        Serial.println("full-backward");
      }
      runMotor('right', 'backward', 255);
      runMotor('left', 'backward', 255);
    }

    // full-left
    if (
      ( xCoordinate < THRESHOLD_X_START_CENTERZONE ) and
      ( yCoordinate < THRESHOLD_Y_END_CENTERZONE and yCoordinate > THRESHOLD_Y_START_CENTERZONE )
    ) {
      Serial.println("full-left");
      runMotor('right', 'forward', 255);
      runMotor('left', 'backward', 255);
    }

    // full-right
    if (
      ( xCoordinate > THRESHOLD_X_END_CENTERZONE ) and
      ( yCoordinate < THRESHOLD_Y_END_CENTERZONE and yCoordinate > THRESHOLD_Y_START_CENTERZONE )
    ) {
      if (DEBUG_MODE) {
        Serial.println("full-right");
      }
      // right motor backward
      runMotor('right', 'backward', 255);

      // left motor forward
      runMotor('left', 'forward', 255);

    }

    // forward-left
    if (
      ( xCoordinate < THRESHOLD_X_START_CENTERZONE ) and
      ( yCoordinate < THRESHOLD_Y_START_CENTERZONE )
    ) {
      if (DEBUG_MODE) {
        Serial.println("forward-left");
      }
      runMotor('right', 'forward', 255);
      runMotor('left', 'forward', 150);
    }

    // forward-right
    if (
      ( xCoordinate > THRESHOLD_X_END_CENTERZONE ) and
      ( yCoordinate < THRESHOLD_Y_START_CENTERZONE )
    ) {
      if (DEBUG_MODE) {
        Serial.println("forward-right");
      }
      runMotor('right', 'forward', 150);
      runMotor('left', 'forward', 255);
    }

    // backward-left
    if (
      ( xCoordinate < THRESHOLD_X_START_CENTERZONE ) and
      ( yCoordinate > THRESHOLD_Y_END_CENTERZONE )
    ) {
      if (DEBUG_MODE) {
        Serial.println("backward-left");
      }
      runMotor('right', 'backward', 255);
      runMotor('left', 'backward', 150);
    }
    // backward-right
    if (
      ( xCoordinate > THRESHOLD_X_END_CENTERZONE ) and
      ( yCoordinate > THRESHOLD_Y_END_CENTERZONE )
    ) {
      if (DEBUG_MODE) {
        Serial.println("backward-right");
      }
      runMotor('right', 'backward', 150);
      runMotor('left', 'backward', 255);
    }
  }
}

/**
   Function runMotor
   This function is used to run the DC motor with some speed and direction values defined using the function parameters.

   @param char motorName - left|right
   @param char motorDirection - forward|backward
   @param byte motorSpeed - [0;255]

   @return void
*/
void runMotor(char motorName, char motorDirection, byte motorSpeed) {

  if ('right' == motorName) {
    analogWrite(5, motorSpeed); // EN1
    if ('forward' == motorDirection) {
      digitalWrite(7, 1); // IN1
      digitalWrite(6, 0); // IN2
    } else if ('backward' == motorDirection) {
      digitalWrite(7, 0); // IN1
      digitalWrite(6, 1); // IN2
    }
  } else if ('left' == motorName) {
    analogWrite(3, motorSpeed); // EN2
    if ('forward' == motorDirection) {
      digitalWrite(4, 0); // IN3
      digitalWrite(2, 1 ); // IN4
    } else if ('backward' == motorDirection) {
      digitalWrite(4, 1); // IN3
      digitalWrite(2, 0 ); // IN4
    }
  }
}

/**
   Function stopMotor
   This function is used to stop the DC motor by name.

   @param char motorName - left|right

   @return void
*/
void stopMotor(char motorName) {
  if ('right' == motorName) {
    analogWrite(5, 255); // EN1
    digitalWrite(7, 1); // IN1
    digitalWrite(6, 1); // IN2

  } else if ('left' == motorName) {
    analogWrite(3, 255); // EN2
    digitalWrite(4, 1); // IN3
    digitalWrite(2, 1); // IN4
  }
}
