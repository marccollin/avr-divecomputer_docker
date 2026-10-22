/*
Live dive and surface page backed by the shared DiveView/DiveScreen renderer.
*/
#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <time.h>

#include "diveController.h"
#include "diveScreen.h"
#include "ui.h"

/*
Live depth, dive time, NDL/stop, ceiling, TTS, gas and temperature page.
Uses DiveView/DiveScreen shared with DemoPage; B2 opens demos only at the surface.
B1/B3 open the menu; during dives B2 also opens the menu.
*/
class MainPage final : public Page {
public:
    /*
    Binds externally owned dive; menu and demo destinations start unset.
    */
    explicit MainPage(DiveController &dive);
    /*
    Sets externally owned menu_page as the menu navigation destination.
    */
    void set_menu(Page &menu_page) { menu = &menu_page; }
    /*
    Sets externally owned demo_page as the surface-only demo destination.
    */
    void set_demo(Page &demo_page) { demo = &demo_page; }
    /*
    Formats a live snapshot with absolute pressure_bar in bar and temperature_c in Celsius.
    Uses UTC epoch now in seconds and the current controller metrics; does not sample tissues.
    */
    void update(float pressure_bar, float temperature_c, time_t now);
    /*
    Routes physical button id through ui to configured menu or surface-only demo.
    */
    void on_button(Ui &ui, ButtonId id) override;
    /*
    Draws the formatted live snapshot on the supplied 128x64 display.
    */
    void draw(u8g_t *u8g) override;
private:
    DiveController &dive;
    Page *menu;
    Page *demo;
    DiveScreen screen;
};

#endif
