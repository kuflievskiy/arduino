/**
  Custom RC Model Robot.
  Transmitter part.

  List of modules used: 
    - Arduino UNO (microcontroler)
    - nRF24L01 (radio module)
  
  Author: Kuflievskiy Aleksey kuflievskiy@gmail.com
*/


#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9, 10); // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб

byte counter;

/**
 * Constants for Controll
 */
const int analogVRXPin = A0;
const int analogVRYPin = A1;
const int analogSWPin = A2;

void setup() {
  Serial.begin(9600); //открываем порт для связи с ПК

  radio.begin(); //активировать модуль
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах

  radio.openWritingPipe(address[0]);   //мы - труба 0, открываем канал для передачи данных
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp(); //начать работу
  radio.stopListening();  //не слушаем радиоэфир, мы передатчик


  // Inint analogVRXPin, analogVRYPin, analogSWPin
  pinMode(analogVRXPin, INPUT); // VRX
  pinMode(analogVRYPin, INPUT); // VRY
  pinMode(analogSWPin, INPUT_PULLUP); // SW(Button)
}

void loop(void) {

  boolean controlButtonState = digitalRead(analogSWPin); // считываем состояние кнопки  
  int controlXPosition = analogRead(analogVRXPin);    // считываем значение оси Х
  int controlYPosition = analogRead(analogVRYPin);    // считываем значение оси Y
  controlXPosition = map(controlXPosition, 0, 1023, 0, 255); // преобразуем значение X в другой диапазон
  controlYPosition = map(controlYPosition, 0, 1023, 0, 255); // преобразуем значение Y в другой диапазон

  byte controlData[] = {(byte)controlXPosition,(byte)controlYPosition,(byte)controlButtonState};
  
  Serial.print("Sent: ");
  Serial.print("");
    
  //for (byte i = 0; i <= 3; i = i + 1) {
    //Serial.println(controlData[i]);
  //}

    Serial.println("X: ");
    Serial.println(controlData[0]);
    Serial.println("Y: ");
    Serial.println(controlData[1]);
    Serial.println("Button: ");
    Serial.println(controlData[2]);
    Serial.println("size:");
Serial.println(sizeof(controlData));
    
  radio.write(&controlData, sizeof(controlData));

  delay(1000);
}