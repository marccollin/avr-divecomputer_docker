#ifndef DECOSETTINGS_H
#define DECOSETTINGS_H

//Decompression parameters. Fixed defaults for now; meant to be edited via a menu later.
struct DecoSettings {
    float gf_low = 0.50f;
    float gf_high = 0.70f;
    float max_ppo2_bottom = 1.4f;
    float max_ppo2_deco = 1.6f;
    float stop_step_m = 3.0f;
    float last_stop_m = 3.0f;
    float ascent_rate_m_min = 9.0f;
    float bar_per_m = 0.098f; // fresh water; set from ConfigComputer at runtime
};

#endif
