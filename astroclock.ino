#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <RTClib.h>
#include <SPI.h>
#include <Wire.h>

#include "images.h"

#define OLED_ADDR     0x3c
#define OLED_RESET    -1

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RTC_DS1307 rtc;

String daysOfTheWeek[] = {
  "SUN", "MON", "TUE",
  "WED", "THU", "FRI",
  "SAT"
};
String months[] = {
  "JAN", "FEB", "MAR",
  "APR", "MAY", "JUN",
  "JUL", "AUG", "SEP",
  "OCT", "NOV", "DEC"
};

uint8_t prevImg = 0;
uint16_t count = 0;

void setup() {
  rtc.begin();

  display.begin(OLED_ADDR);
  display.setTextColor(SH110X_WHITE);

  display.clearDisplay();
  display.drawRect(66, 4, 55, 56, SH110X_WHITE);
  display.drawLine(66, 18, 120, 18, SH110X_WHITE);

  drawImage();
  display.display();
}

void loop() {
  display.fillRect(67, 5, 53, 54, SH110X_BLACK);
  display.drawLine(66, 18, 120, 18, SH110X_WHITE);
  drawCalendar();

  count++;
  if(count == 300) {
    drawImage();
    count = 0;
  }

  display.display();
  delay(1000);
}

void drawImage() {
  display.fillRect(0, 0, 64, 64, SH110X_BLACK);

  switch(prevImg) {
    case 0:
      display.drawBitmap(10, 1, bmp_astronaut, 44, 62, SH110X_WHITE);
      break;

    case 1:
      display.drawBitmap(0, 12, bmp_planet, 64, 46, SH110X_WHITE);
      break;

    case 2:
      display.drawBitmap(0, 9, bmp_comet, 64, 49, SH110X_WHITE);
      break;
  }

  prevImg++;
  if(prevImg == 3)
    prevImg = 0;
}

void drawCalendar() {
    DateTime now = rtc.now();
  uint8_t hour = now.hour(),
    minute = now.minute(),
    second = now.second(),
    year = now.year(),
    month = now.month(),
    day = now.day();

  display.setTextSize(1);
  display.setCursor(70, 8);

  display.print(hour < 10 ? F("0") : F(""));
  display.print(hour);

  display.print(F(":"));

  display.print(minute < 10 ? F("0") : F(""));
  display.print(minute);
  display.print(F(":"));

  display.print(second < 10 ? F("0") : F(""));
  display.print(second);

  display.setTextSize(2);
  display.setCursor(76, 22);
  display.print(daysOfTheWeek[now.dayOfTheWeek()]);

  display.setTextSize(1);
  display.setCursor(76, 39);
  display.print(months[month]);
  display.print(F(" "));
  display.print(day < 9 ? F("0") : F(""));
  display.print(day);

  display.setCursor(82, 49);
  display.print(year + 1792);
}