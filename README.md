# 2-keys-rfid-loker-openers

A security system that supports opening a door or safe by simultaneously reading a dual-chip card at 125 KHz and 13.56Mhz only UID.

Esp8266 and esp32 and pn532 and rdm6300 and tm1637

Reader.ino for esp32 main(tm1637)

1356mhz.ino for esp32(pn532)

125mhz.ino for esp8266(rdm6300)

# IMAGE

![image](https://github.com/user-attachments/assets/7283730e-d265-4445-b874-19eb5912df0d)
![image](https://github.com/user-attachments/assets/4d56bb10-d1c0-439b-82a2-149c25c693b5)
![image](https://github.com/user-attachments/assets/640d7fd2-782e-4048-acea-ec96e136cbc0)

# Video

https://github.com/user-attachments/assets/61fcc075-d1cd-4c16-a22e-7debb02aced2

# ADD KEYS in reader.ino

Change and add values key only UID

first column - 13.56 Mhz, second - 125Khz cards

Support 1000 values

```
// 1 столбец - 13.56MHZ и 2 Столбец - 125KHZ коды доступа
const char* codes[][2] = {
    {"8D9104A4", "8DDA4CAB3C"},
    {"7DB212A4", "08822B0B2D"},
    {"1DB90BA4", "FA6D4B3528"},
    {"AD900CA4", "B0F57B838E"},
    {"CDDD04A4", "E723E8A2B8"}
};
```

# Support access servo or all lockers

Add in reader.ino you function, when pass cards in while.

```
void access(){
  uint32_t now = millis();
  while (millis () - now < 5000) {
   display.setSegments(segments);
  }
  display.clear();
}

```

# System support monitor port

All esp sent date in monitor port
