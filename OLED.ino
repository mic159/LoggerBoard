#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <Adafruit_MPL115A2.h>
#include <ClosedCube_HDC1080.h>
#include <RTClib.h>
#include "Menu.h"
#include "Buttons.h"

//OLED
#define OLED_CS  8
#define OLED_DC  10
#define OLED_RST 9

Adafruit_SSD1331 display = Adafruit_SSD1331(OLED_CS, OLED_DC, OLED_RST); 
Adafruit_MPL115A2 pressure;
ClosedCube_HDC1080 humidity;
RTC_DS1307 rtc;
Buttons buttons;

unsigned long rtc_last = 0;
DateTime now;

const uint16_t COLOR_CHROME = display.Color565(0x5d, 0x99, 0xc6);
const uint16_t COLOR_HEADER = display.Color565(0x90, 0xca, 0xf9);
const uint16_t COLOR_LIGHT = display.Color565(0xc3, 0xfd, 0xff);
const uint16_t COLOR_BG_1 = display.Color565(0xf5, 0xf5, 0xf6);
const uint16_t COLOR_BG_2 = display.Color565(0xe1, 0xe2, 0xe1);
const uint16_t COLOR_TEXT = 0;


void setup() {
  Serial.begin(115200);
  display.begin();
  rtc.begin();
  pressure.begin();
  humidity.begin(0x40);
  buttons.begin();

  now = rtc.now();
  rtc_last = millis();

  registerMenu(MENU_SETTINGS, new SettingsMenu(), NULL, false);
  registerMenu(MENU_READINGS, new ReadingsMenu(), "Live readings", true);
}

void loop() {
  bool draw = false;
  if (millis() - rtc_last > 1000) {
    now = rtc.now();
    rtc_last = millis();
    draw = true;
  }
  if (buttons.update()) {
    draw = currentMenu()->update(buttons);
  }
  if (draw) {
    currentMenu()->draw(&display);
  }
}

