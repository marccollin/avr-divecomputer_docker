#ifndef DIVESCREEN_H
#define DIVESCREEN_H

#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <u8g.h>

#include "gasMix.h"

//Everything the dive screen shows. Filled from the live DiveController or from demo data.
struct DiveView {
    bool in_dive;
    float pressure_bar;
    float depth_m;
    float max_depth_m;
    uint32_t dive_time_s;
    int ndl_min;
    float ceiling_m;
    bool has_stop;
    int stop_depth_m;
    int stop_min;
    int tts_min;
    GasMix gas;
    float temperature_c;
    bool clock_set;
    time_t now;
};

//Formats and draws the four lines of the dive / surface screen.
class DiveScreen {
public:
    DiveScreen() : line1(), line2(), line3(), line4(), tag() {}
    void format(const DiveView &view);
    //Small inverted label at the bottom right (e.g. "DEMO 1/3"); empty for none.
    void set_tag(const char *text);
    void draw(u8g_t *u8g) const;
private:
    char line1[24];
    char line2[24];
    char line3[24];
    char line4[32];
    char tag[24];
};

#endif
