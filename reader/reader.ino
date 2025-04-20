#include "GBUS.h"
#include <HardwareSerial.h>
#include <Arduino.h>
#include <TM1637Display.h>
#include <GTimer.h>
#include <SD.h>

#define PIN_SPI_CS 5 // The ESP32 pin GPIO5

#define CLK  22 // The ESP32 pin GPIO22 connected to CLK
#define DIO  21 // The ESP32 pin GPIO23 connected to DIO

HardwareSerial mySerial2(1); // UART1
HardwareSerial mySerial(2); // Используем UART2

GBUS bus(&mySerial, 1, 20);  // адрес 3, буфер 20 байт
GBUS bus2(&mySerial2, 2, 20);  // адрес 2, буфер 20 байт

TM1637Display display = TM1637Display(CLK, DIO);

// Определяем символы для отображения
const uint8_t O = 0b00111111; // O
const uint8_t P = 0b01110011; // P
const uint8_t E = 0b01111001; // E
const uint8_t N = 0b01010100; // n
uint8_t segments[4] = {O, P, E, N};

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
// 1 столбец - 13.56MHZ и 2 Столбец - 125MHZ коды доступа
const char* codes[][2] = {
    {"8D9104A4", "8DDA4CAB3C"},
    {"7DB212A4", "08822B0B2D"},
    {"1DB90BA4", "FA6D4B3528"},
    {"AD900CA4", "B0F57B838E"},
    {"CDDD04A4", "E723E8A2B8"}
};


bool f1356 = false;
bool f125 = false;

const char* mhz1356;
const char* mhz125;

int numRows = sizeof(codes) / sizeof(codes[0]);
static GTimer16<millis> tmr1(5000, true, GTMode::Timeout, false); // 5 секунд таймер,До 1 минуты таймер

void setup() {
  display.setBrightness(7);
  Serial.begin(115200);
  mySerial.begin(115200, SERIAL_8N1, 16, 17); // RX TX UART 2 - 16-17 17-16
  mySerial2.begin(115200, SERIAL_8N1, 27, 26); // RX, TX  UART 1 - 27-D8 26-D3
}

void loop() {
  bus.tick();
  bus2.tick();
  r1356();
  r125();
  check();
}

void r1356(){
  static char dataArray2[9];
  if (bus.gotData()) {
    myStruct data;
    bus.readData(data);
    dataArray2[0] = data.w0;
    dataArray2[1] = data.w1;
    dataArray2[2] = data.w2;
    dataArray2[3] = data.w3;
    dataArray2[4] = data.w4;
    dataArray2[5] = data.w5;
    dataArray2[6] = data.w6;
    dataArray2[7] = data.w7;
    dataArray2[8] = '\0';
    mhz1356 = dataArray2;
    f1356=true;
    Serial.println(String("13.56MHZ : ") + String(mhz1356));
  }
}
void r125(){
  static char dataArray[11];
  if (bus2.gotData()) {
    myStruct2 data;
    bus2.readData(data);
    dataArray[0] = data.q0;
    dataArray[1] = data.q1;
    dataArray[2] = data.q2;
    dataArray[3] = data.q3;
    dataArray[4] = data.q4;
    dataArray[5] = data.q5;
    dataArray[6] = data.q6;
    dataArray[7] = data.q7;
    dataArray[8] = data.q8;
    dataArray[9] = data.q9;
    dataArray[10] = '\0';
    mhz125 = dataArray;
    f125=true;
    Serial.println(String("125MHZ : ") + String(mhz125));
  }
}
void check(){
  if (f1356 && f125) {
    Serial.println(String("125MHZ : ") + String(mhz125));
    Serial.println(String("13.56MHZ : ") + String(mhz1356));
    f1356=false;
    f125=false;
    find();
    mhz125 = "";
    mhz1356 = "";
  }
}
void find(){
  int rowIndex=-1;
  for (int i = 0; i < numRows; i++) {
    if (strcmp(mhz1356, codes[i][0]) == 0) {
      Serial.println("Совпадение найдено в 1 столбце! 13.56MHZ");
      rowIndex = i;
      if (strcmp(mhz125, codes[rowIndex][1]) == 0) {
      Serial.println("Совпадение найдено во втором столбце! Открыто 125MHZ");
      access();
      }
    }
  }  
}
void access(){
  tmr1.start();
  while (tmr1.getLeft() > 0 ) {
   display.setSegments(segments);
  }
  display.clear();
}