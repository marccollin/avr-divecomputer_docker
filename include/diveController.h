#ifndef DIVECONTROLLER_H
#define DIVECONTROLLER_H

#include <stdint.h>
#include <time.h>

#include "buhlmannModel.h"
#include "decoSettings.h"
#include "gasMix.h"
#include "tissueStore.h"

//Feeds pressure samples to the Buhlmann model, detects dives and persists tissues.
class DiveController {
public:
    DiveController(BuhlmannModel &model, TissueStore &store);

    //Restores tissues from the SD card (with surface off-gassing) or saturates at surface.
    void begin(float p_surface, time_t now);
    void sample(float p_amb, uint32_t tick_ms, time_t now);

    bool switch_gas(int idx) { return gases_.set_active(idx); }
    GasList &gases() { return gases_; }
    DecoSettings &settings() { return settings_; }
    float surface_pressure() const { return model_.surface_pressure(); }

    bool in_dive() const { return in_dive_; }
    float depth_m() const { return depth_m_; }
    float max_depth_m() const { return max_depth_m_; }
    uint32_t dive_time_s() const { return dive_ms_ / 1000; }
    int ndl_min() const { return ndl_min_; }
    float ceiling_m() const { return ceiling_m_; }
    const DecoPlan &plan() const { return plan_; }

private:
    static const uint32_t SAVE_PERIOD_MS = 60000;
    static const uint32_t DIVE_END_MS = 60000;
    static const uint32_t MAX_OFFGAS_S = 14UL * 24 * 3600;

    void start_dive();
    void end_dive(time_t now);
    void save(uint32_t tick_ms, time_t now);

    BuhlmannModel &model_;
    TissueStore &store_;
    GasList gases_;
    DecoSettings settings_;
    DecoPlan plan_;

    bool have_last_;
    float last_p_;
    uint32_t last_tick_;
    uint32_t last_save_;

    bool in_dive_;
    float depth_m_;
    float max_depth_m_;
    uint32_t dive_ms_;
    uint32_t shallow_ms_;
    int ndl_min_;
    float ceiling_m_;
};

#endif
