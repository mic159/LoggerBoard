#include "Menu.h"
#include <Adafruit_MPL115A2.h>
#include <ClosedCube_HDC1080.h>

extern Adafruit_MPL115A2 pressure;
extern ClosedCube_HDC1080 humidity;

ReadingsMenu::ReadingsMenu() {}

void ReadingsMenu::draw(Adafruit_GFX* display) const {
  drawLayout(display, "Live");

  display->setCursor(0, 15);

  display->print("T1: ");
  display->print(pressure.getTemperature());
  display->println(" C");
  
  display->print("T2: ");
  display->print(humidity.readTemperature());
  display->println(" C");
  
  display->print("P: ");
  display->print(pressure.getPressure());
  display->println(" kPa");
  
  display->print("H: ");
  display->print(humidity.readHumidity());
  display->println("%");
}

bool ReadingsMenu::update(Buttons& buttons) {
  if (buttons.back()) {
    switchMenu(MENU_SETTINGS);
    return true;
  }
  return false;
}

