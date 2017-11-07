#ifndef MENU_H
#define MENU_H

#include <SSD_13XX.h>
#include "Buttons.h"

enum Menu_selection {
  MENU_SETTINGS,
  MENU_READINGS,
  MENU_SET_CLOCK,

  MENU_MAX,
};

class Menu {
public:
  Menu();
  virtual bool update(Buttons& buttons) = 0;
  virtual void draw(SSD_13XX* display) const = 0;
protected:
  void drawLayout(SSD_13XX* display, const char* title) const;
};

class SettingsMenu : public Menu {
public:
  SettingsMenu();
  bool update(Buttons& buttons);
  void draw(SSD_13XX* display) const;
  int selection = MENU_READINGS;
};

class ReadingsMenu : public Menu {
public:
  ReadingsMenu();
  bool update(Buttons& buttons);
  void draw(SSD_13XX* display) const;
};

class SetClockMenu : public Menu {
public:
  SetClockMenu();
  bool update(Buttons& buttons);
  void draw(SSD_13XX* display) const;

private:
  int selection;
};

void registerMenu(Menu_selection type, Menu* instance, const char* title, bool show_in_menu);
void switchMenu(Menu_selection);
Menu* currentMenu();

#endif

