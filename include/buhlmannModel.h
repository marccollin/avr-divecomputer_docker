#ifndef BUHLMANNMODEL_H_
#define BUHLMANNMODEL_H_

#include <stdint.h>

#include "decoSettings.h"
#include "gasMix.h"

//Inert gas loading of the 16 ZHL-16C compartments, in bar.
struct TissueState {
    static const int COUNT = 16;
    float n2[COUNT];
    float he[COUNT];
};

struct DecoStop {
    uint8_t depth_m;
    uint8_t minutes;
    uint8_t gas;     //index in GasList used at this stop
};

struct DecoPlan {
    static const int MAX_STOPS = 20;
    DecoStop stops[MAX_STOPS];
    int count;
    int tts_min;     //time to surface, ascent + stops
};

//Hardware independent Buhlmann ZHL-16C model with gradient factors.
//Pressures are absolute, in bar. Durations are in seconds unless noted.
class BuhlmannModel {
public:
    BuhlmannModel();

    void reset_to_surface(float p_surface);
    void set_surface_pressure(float p_surface) { p_surface_ = p_surface; }
    float surface_pressure() const { return p_surface_; }

    //Tissue loading for a linear pressure change from p_start to p_end in dt_s seconds.
    void update(float p_start, float p_end, float dt_s, const GasMix &gas);

    //Deepest first stop seen during the dive (GF low anchor), 0 when none.
    float gf_anchor() const { return gf_anchor_; }
    void set_gf_anchor(float p) { gf_anchor_ = p; }
    void reset_anchor() { gf_anchor_ = 0.0f; }
    //Deepens the GF low anchor to the current first stop; call while diving.
    void track_anchor(const DecoSettings &s);

    //Current ceiling in metres using interpolated GF (0 = can surface).
    float ceiling_m(const DecoSettings &s) const;
    //Minutes left at p_amb on gas before a stop is required (0 = in deco, 99 = max).
    int ndl_min(float p_amb, const GasMix &gas, const DecoSettings &s) const;
    //Ascent plan from p_amb, starting on gas index current_gas.
    void plan(float p_amb, const GasList &gases, int current_gas,
              const DecoSettings &s, DecoPlan &out);

    TissueState tissues;

    static const int NDL_MAX = 99;

private:
    float p_surface_;
    float gf_anchor_;
    float k_n2_[TissueState::COUNT];   // 1/min
    float k_he_[TissueState::COUNT];
    float f1_n2_[TissueState::COUNT];  // exp(-k * 1 min)
    float f1_he_[TissueState::COUNT];

    static float tolerated(const TissueState &t, float gf);
    static bool tolerates(const TissueState &t, float p_amb, float gf);
    float gf_at(float p, float anchor, const DecoSettings &s) const;
    float gf_low_first_stop(const TissueState &t, const DecoSettings &s) const;
    void schreiner(TissueState &t, float p_start, float p_end, float dt_min,
                   const GasMix &gas) const;
    void constant_minute(TissueState &t, float p_amb, const GasMix &gas) const;
};

#endif /* BUHLMANNMODEL_H_ */
