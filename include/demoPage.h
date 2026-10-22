#ifndef DEMOPAGE_H
#define DEMOPAGE_H

#include "diveScreen.h"
#include "ui.h"

//Shows example dive screens with fixed data.
//B2 = next example, B1 / B3 = back to the main screen. Also closes after 10 s idle.
class DemoPage final : public Page {
public:
    DemoPage() : screen(), index(0) {}
    void on_enter() override;
    void on_button(Ui &ui, ButtonId id) override;
    void draw(u8g_t *u8g) override;
private:
    void load(void);

    DiveScreen screen;
    int index;
};

#endif
