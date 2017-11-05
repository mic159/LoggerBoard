#ifndef MENU_H
#define MENU_H

#include <Adafruit_GFX.h>
#include "Buttons.h"

enum Menu_selection {
  MENU_SETTINGS,
  MENU_READINGS,

  MENU_MAX,
};

class Menu {
public:
  Menu();
  virtual bool update(Buttons& buttons) = 0;
  virtual void draw(Adafruit_GFX* display) const = 0;
protected:
  void drawLayout(Adafruit_GFX* display, const char* title) const;
};

class SettingsMenu : public Menu {
public:
  SettingsMenu();
  bool update(Buttons& buttons);
  void draw(Adafruit_GFX* display) const;
  int selection = MENU_READINGS;
};

class ReadingsMenu : public Menu {
public:
  ReadingsMenu();
  bool update(Buttons& buttons);
  void draw(Adafruit_GFX* display) const;
};

void registerMenu(Menu_selection type, Menu* instance, const char* title, bool show_in_menu);
void switchMenu(Menu_selection);
Menu* currentMenu();

#endif

