#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <Arduino.h>
#include "GBUS.h"
#include <HardwareSerial.h>

#define PN532_SCK   (18) // 18
#define PN532_MISO  (19) // 19
#define PN532_MOSI  (23) // 23
#define PN532_SS    (5) // 5

//#define PN532_RESET (21) // 21 Reset module to RSTO
//#define PN532_IRQ   (4) // 4 to IRQ
//Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

#define LED_BUILTIN 2;

const int ledPin = LED_BUILTIN;

Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);
HardwareSerial mySerial(2); // Используем UART2

// адрес 3, буфер 20 байт
GBUS bus(&mySerial, 3, 20);

struct myStruct {
  char w0;
  char w1;
  char w2;
  char w3;
  char w4;
  char w5;
  char w6;
  char w7;
};

void setup() {
  mySerial.begin(115200, SERIAL_8N1, 16, 17); // RX TX UART 2 - 16 17
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  delay(5000);
  nfc.begin();
  nfc.setPassiveActivationRetries(0xFF);
}

void loop() {
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };	// Buffer to store the returned UID
  uint8_t uidLength;				// Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  digitalWrite(ledPin, LOW);
  if (success) {
      Serial.print("UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
      String card13;
      for (uint8_t i = 0; i < uidLength; i++)
      {
        if (uid[i] < 0x10) // Если значение меньше 16, добавляем ведущий ноль
        {
          card13 += "0";
        }
        card13 += String(uid[i], HEX);
      }
      card13.toUpperCase();
      tochars(card13);
      digitalWrite(ledPin, HIGH);
      card13="";
	}
  nfc.begin();
}

void tochars(String card13)
{
  Serial.println(card13);
  if (card13.length() != 8) {
    Serial.println("Ошибка: строка слишком длинная!");
    return;
  }
  myStruct data;
  data.w0 = card13.charAt(0);
  data.w1 = card13.charAt(1);
  data.w2 = card13.charAt(2);
  data.w3 = card13.charAt(3);
  data.w4 = card13.charAt(4);
  data.w5 = card13.charAt(5);
  data.w6 = card13.charAt(6);
  data.w7 = card13.charAt(7);
  bus.sendData(1, data);
}
