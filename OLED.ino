#include <Wire.h>
#include <SPI.h>
#include <Bounce2.h>
#include <SD.h>
#include <SSD_13XX.h>
#include <Adafruit_GFX.h>
#include <Adafruit_MPL115A2.h>
#include <ClosedCube_HDC1080.h>
#include <RTClib.h>
#include "Menu.h"
#include "Buttons.h"
#include "State.h"

//OLED
#define OLED_CS   8
#define OLED_DC   10
#define OLED_RST  9
#define SD_CS     4
#define SD_DETECT 12
#define LED_RED   13
#define LED_GREEN 6
#define LED_BLUE  7

SSD_13XX display = SSD_13XX(OLED_CS, OLED_DC, OLED_RST); 
Adafruit_MPL115A2 pressure;
ClosedCube_HDC1080 humidity;
RTC_DS1307 rtc;
Buttons buttons;
Bounce sdDetect;

unsigned long rtc_last = 0;
unsigned long sd_last = 0;
state_t state = {0};
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
  sdDetect.attach(SD_DETECT);
  sdDetect.interval(5);

  pinMode(LED_RED, OUTPUT); digitalWrite(LED_RED, HIGH);
  pinMode(LED_GREEN, OUTPUT); digitalWrite(LED_GREEN, HIGH);
  pinMode(LED_BLUE, OUTPUT); digitalWrite(LED_BLUE, HIGH);

  now = rtc.now();
  rtc_last = millis();

  registerMenu(MENU_SETTINGS, new SettingsMenu(), NULL, false);
  registerMenu(MENU_READINGS, new ReadingsMenu(), "Live readings", true);
  registerMenu(MENU_SET_CLOCK, new SetClockMenu(), "Set Clock", true);
}

void loop() {
  bool draw = false;
  if (millis() - rtc_last > 1000) {
    now = rtc.now();
    rtc_last = millis();
    draw = true;
  }

  // Check for SD insertion
  if (state.sd_inserted == false && millis() - sd_last > 1000) {
    state.sd_inserted = SD.begin(SD_CS);
    if (state.sd_inserted) {
      draw = true;
      digitalWrite(LED_RED, HIGH);
    } else {
      digitalWrite(LED_RED, LOW);
    }
    sd_last = millis();
  }

  if (buttons.update()) {
    draw = currentMenu()->update(buttons);
  }
  if (draw) {
    currentMenu()->draw(&display);
  }
}

