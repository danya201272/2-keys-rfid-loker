#include <SoftwareSerial.h>
#include <Arduino.h>
#include <GTimer.h>
#include "GBUS.h"
SoftwareSerial RFID(14, 12); // RX and TX D5,D6
SoftwareSerial mySerial2(0, 15); // RX TX D3 D8

// адрес 4, буфер 20 байт
GBUS bus2(&mySerial2, 4, 20);

static GTimer16<millis> tmr1(1000, true, GTMode::Interval, false); // 1 секунда таймер,До 1 минуты таймер

struct myStruct2 {
  char q0;
  char q1;
  char q2;
  char q3;
  char q4;
  char q5;
  char q6;
  char q7;
  char q8;
  char q9;
};

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  mySerial2.begin(115200); // RX TX D3 D8
  RFID.begin(9600);
}

void loop()
{
  tmr1.tick();
  char c;
  String text;
  digitalWrite(LED_BUILTIN, LOW);
  while (RFID.available() > 0) {
    delay(5);
    c = RFID.read();
    text += c;
  }
  digitalWrite(LED_BUILTIN, HIGH);
  if (text.length() > 20)
  check(text);
  text="";
}

void check(String text) {
  if (tmr1) {
	text = text.substring(1, 11);
	tochars(text);
	}
}
void tochars(String text)
{
  Serial.println(text);
  if (text.length() != 10) {
    Serial.println("Ошибка: строка слишком длинная!");
    return;
  }
  myStruct2 data;
  data.q0 = text.charAt(0);
  data.q1 = text.charAt(1);
  data.q2 = text.charAt(2);
  data.q3 = text.charAt(3);
  data.q4 = text.charAt(4);
  data.q5 = text.charAt(5);
  data.q6 = text.charAt(6);
  data.q7 = text.charAt(7);
  data.q8 = text.charAt(8);
  data.q9 = text.charAt(9);
  bus2.sendData(2, data);
}
