#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <RTClib.h>
#include "Menu.h"
#include "State.h"
#include "Color.h"


const PROGMEM uint8_t SD_ICON[] = {
  0b00000000,
  0b00001110,
  0b00011110,
  0b00111110,
  0b00111110,
  0b00111110,
  0b00111110,
  0b00000000,
};

extern DateTime now;
extern state_t state;
Menu* menu[MENU_MAX] = {0};
const char* menu_title[MENU_MAX] = {0};
bool menu_show[MENU_MAX] = {0};
Menu_selection menu_current = MENU_SETTINGS;


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

void Menu::drawLayout(SSD_13XX* display, const char* title) const {
  display->fillScreen(COLOR_BG_1);
  display->fillRect(0, 0, display->width(), 8, COLOR_CHROME);

  // Title
  display->setTextColor(COLOR_TEXT);
  display->setTextScale(1);
  display->setCursor(2, 1);
  display->print(title);

  // Clock
  char buff[8];
  display->setCursor(display->width() - 35, 1);
  snprintf_P(buff, 7, PSTR("%02d:%02d"), now.hour(), now.minute());
  display->print(buff);

//  if (state.sd_inserted) {
  display->drawBitmap(display->width() - 8, 0, SD_ICON, 8, 8, state.sd_inserted ? 0 : COLOR_RED);
//  }
}

SettingsMenu::SettingsMenu() {}

void SettingsMenu::draw(SSD_13XX* display) const {
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

