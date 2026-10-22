#ifndef MENU_H
#define MENU_H

#include <stddef.h>
#include <stdint.h>

#include "diveController.h"
#include "ui.h"

//List of items. B1 = next item, B2 = back, B3 = select.
class MenuPage : public Page {
public:
    void on_button(Ui &ui, ButtonId id) override;
    void draw(u8g_t *u8g) override;
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

struct EditField {
    const char *label;
    int value;
    int min;
    int max;
    int step;
    const char *const *choices; //names for min..max, or nullptr for numbers
    uint8_t decimals;           //1 shows value/10 with one decimal
    const char *suffix;
};

//Edits a few numeric / choice fields. B1 = +, B2 = -, B3 = next field (confirm on the last).
//Leaving the page without confirming (10 s idle) discards the changes.
class EditPage : public Page {
public:
    void on_button(Ui &ui, ButtonId id) override;
    void draw(u8g_t *u8g) override;
    void on_enter() override { field = 0; }
protected:
    static const int MAX_FIELDS = 5;

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

//Owns all menu pages. root() is what the main page opens on a button press.
class MenuSystem {
public:
    explicit MenuSystem(DiveController &dive);
    Page &root(void);
};

#endif
