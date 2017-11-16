#include <Arduino.h>
#include <SD.h>
#include "Menu.h"
#include "Logger.h"
#include "Color.h"


extern Logger logger;

String currentDir("/");


const PROGMEM uint8_t FOLDER_ICON[] = {
  0b00000000,
  0b01110000,
  0b01111110,
  0b01000010,
  0b01000010,
  0b01000010,
  0b01111110,
  0b00000000,
};


LogViewerMenu::LogViewerMenu()
: selection(0)
, maxFiles(0)
{}

void LogViewerMenu::draw(SSD_13XX* display) const {
  drawLayout(display, "SD Card");

  if (logger.sd != SD_INSERTED) {
    display->setCursor(0, 15);
    display->print("Insert SD card");
    return;
  }

  int y = 10;
  File root = SD.open(currentDir);
  int start = max(0, selection - 3);
  for (int i = 0; i < start; ++i) {
    File entry = root.openNextFile();
    entry.close();
  }
  for (int i = 0; i < 7; ++i) {
    File entry =  root.openNextFile();
    if (!entry) break;
    display->setCursor(8, y);
    if (selection == i + start) {
      display->fillRect(0, y, display->width(), 8, COLOR_HEADER);
    }
    display->print(entry.name());
    if (entry.isDirectory()) {
      display->drawBitmap(0, y, FOLDER_ICON, 8, 8, 0);
    }
    entry.close();
    y += 8;
  }
  root.close();
}

bool LogViewerMenu::update(Buttons& buttons) {
  if (buttons.back()) {
    switchMenu(MENU_SETTINGS);
    return true;
  }
  if (buttons.up() || buttons.down()) {
    if (buttons.up()) {
      selection = selection <= 0 ? maxFiles - 1 : selection - 1;
    } else {
      selection = (selection + 1) % maxFiles;
    }
    return true;
  }
  return false;
}

void LogViewerMenu::updateListing() {
  maxFiles = 0;
  File root = SD.open(currentDir);
  File entry = root.openNextFile();
  while (entry) {
    entry.close();
    ++maxFiles;
    entry = root.openNextFile();
  }
  root.close();
}

void LogViewerMenu::onEnter() {
  updateListing();
}


