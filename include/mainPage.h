#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <time.h>

#include "diveController.h"
#include "diveScreen.h"
#include "ui.h"

//Default screen: depth, dive time, NDL / deco stop, ceiling, TTS, gas and temperature.
//B1 / B3 open the menu, B2 opens the demo screens.
class MainPage final : public Page {
public:
    explicit MainPage(DiveController &dive);
    void set_menu(Page &menu_page) { menu = &menu_page; }
    void set_demo(Page &demo_page) { demo = &demo_page; }
    void update(float pressure_bar, float temperature_c, time_t now);
    void on_button(Ui &ui, ButtonId id) override;
    void draw(u8g_t *u8g) override;
private:
    DiveController &dive;
    Page *menu;
    Page *demo;
    DiveScreen screen;
};

#endif
