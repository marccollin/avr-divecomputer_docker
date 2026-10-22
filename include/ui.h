#ifndef UI_H
#define UI_H

#include <stdint.h>
#include <u8g.h>

#include "buttonPanel.h"

class Ui;

//A full-screen view (main screen, menu, edit page, ...).
class Page {
public:
    virtual void on_button(Ui &ui, ButtonId id) = 0;
    virtual void draw(u8g_t *u8g) = 0;
    //Called when the page is opened with Ui::show (not when returning to it with back).
    virtual void on_enter() {}
protected:
    ~Page() {}
};

//Routes button presses to the current page, keeps a navigation stack
//and returns to the home page after a period without button presses.
class Ui {
public:
    static const uint32_t IDLE_TIMEOUT_MS = 10000;

    Ui(ButtonPanel &buttons, Page &home);
    void show(Page &page);
    void back(void);
    void go_home(void);
    bool at_home(void) const { return depth == 1; }
    void render(void);
    //Waits duration_ms while handling button presses and redrawing.
    void run_for(uint32_t duration_ms);
private:
    static const int MAX_DEPTH = 6;

    ButtonPanel &buttons;
    Page *stack[MAX_DEPTH];
    int depth;
    uint32_t last_press;
};

#endif
