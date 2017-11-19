#include <vector>
#include <Arduino.h>
#include <SD.h>
#include "Menu.h"
#include "Logger.h"
#include "Color.h"


extern Logger logger;

struct FileInfo {
  String name;
  bool isDirectory;
  FileInfo(const String& name, bool isDirectory)
  : name(name), isDirectory(isDirectory) {}
};

String currentDir("/");
std::vector<FileInfo> currentFileListing;


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


BrowseMenu::BrowseMenu()
: selection(0)
{}

void BrowseMenu::draw(SSD_13XX* display) const {
  drawLayout(display, "SD Card");

  if (logger.sd != SD_INSERTED) {
    display->setCursor(0, 15);
    display->print("Insert SD card");
    return;
  }

  int y = 10;
  int start = max(0, selection - 3);
  for (int i = start; i < currentFileListing.size() && y < display->height(); ++i) {
    display->setCursor(8, y);
    if (selection == i) {
      display->fillRect(0, y, display->width(), 8, COLOR_HEADER);
    }
    display->print(currentFileListing[i].name);
    if (currentFileListing[i].isDirectory) {
      display->drawBitmap(0, y, FOLDER_ICON, 8, 8, 0);
    }
    y += 8;
  }
}

bool BrowseMenu::update(Buttons& buttons) {
  if (logger.sd != SD_INSERTED && currentFileListing.size() > 0) {
     currentFileListing.clear();
     return true;
  } else if (logger.sd == SD_INSERTED && currentFileListing.size() == 0) {
    updateListing();
    return true;
  }
  if (buttons.back()) {
    switchMenu(MENU_SETTINGS);
    return true;
  }
  if (buttons.up() || buttons.down()) {
    if (buttons.up()) {
      selection = selection <= 0 ? currentFileListing.size() - 1 : selection - 1;
    } else {
      selection = (selection + 1) % currentFileListing.size();
    }
    return true;
  }
  if (buttons.enter()) {
    if (selection >= currentFileListing.size() || selection < 0) return false;
    if (currentFileListing[selection].isDirectory) {
      if (currentFileListing[selection].name == "..") {
        currentDir = "/";
      } else {
        currentDir = currentDir + currentFileListing[selection].name + "/";
      }
      updateListing();
      return true;
    }
  }
  return false;
}

void BrowseMenu::updateListing() {
  selection = 0;
  currentFileListing.clear();
  if (currentDir.length() != 1) {
    currentFileListing.push_back(
      FileInfo("..", true)
    );
  }
  File root = SD.open(currentDir);
  if (!root) return;
  File entry = root.openNextFile();
  while (entry) {
    currentFileListing.push_back(
      FileInfo(
        entry.name(),
        entry.isDirectory()
      )
    );
    entry.close();
    entry = root.openNextFile();
  }
  root.close();
}

void BrowseMenu::onEnter() {
  updateListing();
}


