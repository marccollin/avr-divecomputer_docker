/*
Full-screen page interface and bounded navigation stack.
Routes buttons, renders pages and returns home after inactivity.
*/
#ifndef UI_H
#define UI_H

#include <stdint.h>
#include <u8g.h>

#include "buttonPanel.h"

/*
Forward declaration of the page navigation controller.
*/
class Ui;

/*
Full-screen page contract for the 128x64 display; Ui does not own page lifetimes.
*/
class Page {
public:
    /*
    Handles physical button id; ui provides navigation to other pages.
    */
    virtual void on_button(Ui &ui, ButtonId id) = 0;
    /*
    Draws one u8g render pass on the supplied display; may be called repeatedly.
    */
    virtual void draw(u8g_t *u8g) = 0;
    /*
    Runs when Ui::show opens the page, not when Ui::back returns to it.
    */
    virtual void on_enter() {}
protected:
    ~Page() {}
};

/*
Routes buttons to the current page using a bounded stack of non-owning pointers.
Returns to home after 10 s idle, abandoning unconfirmed editor changes.
*/
class Ui {
public:
    static const uint32_t IDLE_TIMEOUT_MS = 10000; //Inactivity timeout, ms, 10 s.

    /*
    Binds externally owned buttons and home; initialises the stack with home.
    */
    Ui(ButtonPanel &buttons, Page &home);
    /*
    Pushes page and calls on_enter, then redraws.
    At stack capacity, replaces the current top instead; page must outlive its use.
    */
    void show(Page &page);
    /*
    Pops one navigation level without calling on_enter; home cannot be popped.
    */
    void back(void);
    /*
    Drops all navigation levels above home and redraws without applying edits.
    */
    void go_home(void);
    //Returns true when only the home page remains on the stack.
    bool at_home(void) const { return depth == 1; }
    /*
    Renders the current page through the complete u8g paged display loop.
    */
    void render(void);
    /*
    Processes buttons and redraws for duration_ms milliseconds using the HAL tick.
    After 10 s without a button press, returns non-home pages to home.
    With allow_stop, idles in STOP mode while the home page is shown;
    the caller must ensure no peripheral transfer is in progress.
    */
    void run_for(uint32_t duration_ms, bool allow_stop = false);
private:
    static const int MAX_DEPTH = 6; //Page-stack capacity including home, 6.

    ButtonPanel &buttons;
    Page *stack[MAX_DEPTH];
    int depth;
    uint32_t last_press;
};

#endif
