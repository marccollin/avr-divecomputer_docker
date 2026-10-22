#ifndef BUTTONPANEL_H
#define BUTTONPANEL_H

#include <stdint.h>

enum class ButtonId : uint8_t {
    First,
    Second,
    Third,
};

//Owns the three front-panel buttons: GPIO/EXTI setup and press events.
class ButtonPanel {
public:
    void init(void);
    //Returns true and sets id when a debounced press is pending.
    bool poll(ButtonId &id);
    //Returns true when any press is waiting, without consuming it.
    bool pending(void) const;
};

extern ButtonPanel buttonPanel;

#endif
