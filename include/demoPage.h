/*
Fixed dive examples rendered by the same DiveScreen as the live MainPage.
*/
#ifndef DEMOPAGE_H
#define DEMOPAGE_H

#include "diveScreen.h"
#include "ui.h"

/*
Fixed examples using the same DiveView/DiveScreen pipeline as MainPage.
B2 middle cycles examples; B1 left/B3 right return home.
Ui also returns home after 10 s idle; examples never alter live tissues.
*/
class DemoPage final : public Page {
public:
    /*
    Initialises the shared renderer and first example index.
    */
    DemoPage() : screen(), index(0) {}
    /*
    Resets to the first example and formats its numbered demo tag.
    */
    void on_enter() override;
    /*
    Cycles examples with middle button id; other buttons return home through ui.
    */
    void on_button(Ui &ui, ButtonId id) override;
    /*
    Draws the current fixed example on the supplied 128x64 display.
    */
    void draw(u8g_t *u8g) override;
private:
    void load(void);

    DiveScreen screen;
    int index;
};

#endif
