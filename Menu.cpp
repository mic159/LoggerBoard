#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <RTClib.h>
#include "Menu.h"


extern DateTime now;
Menu* menu[MENU_MAX] = {0};
const char* menu_title[MENU_MAX] = {0};
bool menu_show[MENU_MAX] = {0};
Menu_selection menu_current = MENU_SETTINGS;

uint16_t Color565(uint8_t r, uint8_t g, uint8_t b);

const uint16_t COLOR_TEXT = 0;
const uint16_t COLOR_BG_1 = Color565(0xf5, 0xf5, 0xf6);
const uint16_t COLOR_CHROME = Color565(0x5d, 0x99, 0xc6);
const uint16_t COLOR_HEADER = Color565(0x90, 0xca, 0xf9);
const uint16_t COLOR_LIGHT = Color565(0xc3, 0xfd, 0xff);

void switchMenu(Menu_selection s) {
  menu_current = s;
}

void registerMenu(Menu_selection type, Menu* instance, const char* title, bool show_in_menu) {
  menu[type] = instance;
  menu_title[type] = title;
  menu_show[type] = show_in_menu;
}

Menu* currentMenu() {
  return menu[menu_current];
}

Menu::Menu() {}

void Menu::drawLayout(Adafruit_GFX* display, const char* title) const {
  display->fillScreen(COLOR_BG_1);
  display->fillRect(0, 0, display->width(), 8, COLOR_CHROME);

  
  // Title
  display->setTextColor(COLOR_TEXT);
  display->setTextSize(1);
  display->setCursor(2, 1);
  display->print(title);

  // Clock
  char buff[8];
  display->setCursor(display->width() - 31, 1);
  snprintf_P(buff, 7, PSTR("%02d:%02d"), now.hour(), now.minute());
  display->print(buff);
}

SettingsMenu::SettingsMenu() {}

void SettingsMenu::draw(Adafruit_GFX* display) const {
  drawLayout(display, "Main");
  int offset = 0;
  for (int i = 0; i < MENU_MAX; ++i) {
    if (menu_show[i] == false) continue;
    int y = offset * 8 + 10;
    display->setCursor(8, y);
    if (selection == i) {
      display->fillRect(0, y, display->width(), 8, COLOR_HEADER);
    }
    display->print(menu_title[i]);
    ++offset;
  }
}

bool SettingsMenu::update(Buttons& buttons) {
  if (buttons.up()) {
    do {
      selection = (selection + MENU_MAX - 1) % MENU_MAX;
    } while (!menu_show[selection]);
    return true;
  } else if (buttons.down()) {
    do {
      selection = (selection + 1) % MENU_MAX;
    } while (!menu_show[selection]);
    return true;
  }
  if (buttons.enter()) {
    switchMenu((Menu_selection)selection);
    return true;
  }
  return false;
}

uint16_t Color565(uint8_t r, uint8_t g, uint8_t b) {
  uint16_t c;
  c = r >> 3;
  c <<= 6;
  c |= g >> 2;
  c <<= 5;
  c |= b >> 3;

  return c;
}

