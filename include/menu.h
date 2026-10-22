/*
Three-button menu navigation and staged numeric or choice editors.
Provides gas, decompression, clock and display-setting pages.
*/
#ifndef MENU_H
#define MENU_H

#include <stddef.h>
#include <stdint.h>

#include "diveController.h"
#include "ui.h"

/*
Scrollable menu: B1 left selects next item, B2 middle goes back, B3 right selects.
A tab character in labels separates the item name from its right-aligned value.
*/
class MenuPage : public Page {
public:
    /*
    Handles physical button id through ui using the page navigation rules.
    */
    void on_button(Ui &ui, ButtonId id) override;
    /*
    Draws the current selection/fields on the supplied 128x64 u8g display.
    */
    void draw(u8g_t *u8g) override;
    /*
    Resets the selected item and scrolling origin when opened.
    */
    void on_enter() override { selected = 0; top = 0; }
protected:
    MenuPage() : title(), selected(0), top(0) {}
    ~MenuPage() {}
    virtual int count() const = 0;
    virtual void label(int i, char *buf, size_t len) const = 0;
    virtual void select(Ui &ui, int i) = 0;

    char title[20];
    int selected;
    int top;
};

/*
One staged numeric or choice field with inclusive integer bounds.
For decimals == 1, integers represent tenths of the displayed unit.
*/
struct EditField {
    const char *label; //Field name, null-terminated text.
    int value; //Staged integer or choice index, min..max after clamping.
    int min; //Inclusive lower bound, in stored integer units.
    int max; //Inclusive upper bound, in stored integer units.
    int step; //Positive increment/decrement in stored integer units.
    const char *const *choices; //Choice names indexed by value - min, or nullptr for numbers.
    uint8_t decimals;           //Display precision: 0 integer, 1 shows value/10 with one decimal.
    const char *suffix; //Optional display-unit suffix; nullptr for none.
};

/*
Staged numeric/choice editor: B1 left increments, B2 middle decrements.
B3 right advances fields and confirms on the last; edits wrap within bounds.
Returning home after 10 s idle discards unconfirmed changes without applying them.
*/
class EditPage : public Page {
public:
    /*
    Handles physical button id through ui using the page navigation rules.
    */
    void on_button(Ui &ui, ButtonId id) override;
    /*
    Draws the current selection/fields on the supplied 128x64 u8g display.
    */
    void draw(u8g_t *u8g) override;
    /*
    Selects the first staged field when the editor opens.
    */
    void on_enter() override { field = 0; }
protected:
    static const int MAX_FIELDS = 5; //Maximum fields per editor, 5.

    EditPage() : title(), fields(), count(0), field(0) {}
    ~EditPage() {}
    virtual int min_for(int i) const { return fields[i].min; }
    virtual int max_for(int i) const { return fields[i].max; }
    virtual void info(char *buf, size_t len) const { (void) len; buf[0] = '\0'; }
    virtual void apply(Ui &ui) = 0;

    void clamp(void);

    char title[20];
    EditField fields[MAX_FIELDS];
    int count;
    int field;
};

/*
Connects the shared static menu pages to a live dive controller.
The main page opens root() on a menu button press.
*/
class MenuSystem {
public:
    /*
    Binds shared menu pages to externally owned dive for gas and setting operations.
    */
    explicit MenuSystem(DiveController &dive);
    /*
    Returns the root menu page by reference; during a dive only MIX is offered.
    */
    Page &root(void);
};

#endif
