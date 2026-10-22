/*
Runtime decompression settings in metric units and fractional gradient factors.
*/
#ifndef DECOSETTINGS_H
#define DECOSETTINGS_H

/*
Metric decompression parameters with defaults; selected fields are editable via menus.
Callers must maintain positive rates/grid sizes and gf_low <= gf_high.
*/
struct DecoSettings {
    float gf_low = 0.50f; //Deep-stop gradient factor, fraction 0..1; menu 0.10..1.00.
    float gf_high = 0.70f; //Surface gradient factor, fraction gf_low..1.
    float max_ppo2_bottom = 1.4f; //Bottom-gas oxygen limit, bar; menu 1.0..1.6.
    float max_ppo2_deco = 1.6f; //Deco-gas oxygen limit, bar; menu 1.0..1.6.
    float stop_step_m = 3.0f; //Stop-grid spacing, metres, positive.
    float last_stop_m = 3.0f; //Shallowest planned stop, metres, positive.
    float ascent_rate_m_min = 9.0f; //Ascent speed, metres/minute, positive.
    float bar_per_m = 0.098f; //Pressure/depth factor, bar/metre, positive; fresh-water default, set from ConfigComputer.
};

#endif
