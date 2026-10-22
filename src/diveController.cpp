#include "configComputer.h"
#include "diveController.h"

namespace {
const float DIVE_START_M = 1.2f;
const float DIVE_END_M = 0.8f;
}

DiveController::DiveController(BuhlmannModel &model, TissueStore &store)
    : model_(model), store_(store), gases_(), settings_(), plan_(),
      have_last_(false), last_p_(0.0f), last_tick_(0), last_save_(0),
      in_dive_(false), depth_m_(0.0f), max_depth_m_(0.0f), dive_ms_(0), shallow_ms_(0),
      ndl_min_(BuhlmannModel::NDL_MAX), ceiling_m_(0.0f)
{
    plan_.count = 0;
    plan_.tts_min = 0;
}

void DiveController::begin(float p_surface, time_t now)
{
    settings_.bar_per_m = configComputer.get_hydrostatic_pressure() / 10.0f;
    model_.reset_to_surface(p_surface);

    TissueState saved;
    float anchor;
    time_t saved_at;
    if (!store_.load(saved, anchor, saved_at)) return;

    const double elapsed = difftime(now, saved_at);
    if (elapsed >= MAX_OFFGAS_S) return; // fully desaturated
    model_.tissues = saved;
    model_.set_gf_anchor(anchor);
    //Unknown or negative elapsed time (clock not kept): keep tissues as saved, conservative.
    if (elapsed > 0) model_.update(p_surface, p_surface, (float) elapsed, GAS_AIR);
    //A saved anchor means the computer restarted during a dive, that mean a big big issue
    in_dive_ = anchor > 0.0f;
}

void DiveController::start_dive()
{
    in_dive_ = true;
    dive_ms_ = 0;
    shallow_ms_ = 0;
    max_depth_m_ = 0.0f;
    model_.reset_anchor();
}

void DiveController::end_dive(time_t now)
{
    in_dive_ = false;
    model_.reset_anchor();
    gases_.set_active(0);
    save(last_tick_, now);
}

void DiveController::save(uint32_t tick_ms, time_t now)
{
    last_save_ = tick_ms;
    store_.save(model_.tissues, model_.gf_anchor(), now);
}

void DiveController::sample(float p_amb, uint32_t tick_ms, time_t now)
{
    if (!have_last_) {
        have_last_ = true;
        last_save_ = tick_ms;
    } else {
        const uint32_t dt_ms = tick_ms - last_tick_;
        model_.update(last_p_, p_amb, dt_ms / 1000.0f, in_dive_ ? gases_.active() : GAS_AIR);

        if (in_dive_) {
            dive_ms_ += dt_ms;
            shallow_ms_ = depth_m_ < DIVE_END_M ? shallow_ms_ + dt_ms : 0;
        }
    }
    last_p_ = p_amb;
    last_tick_ = tick_ms;

    depth_m_ = (p_amb - model_.surface_pressure()) / settings_.bar_per_m;
    if (depth_m_ < 0.0f) depth_m_ = 0.0f;

    if (!in_dive_ && depth_m_ > DIVE_START_M) start_dive();
    if (in_dive_) {
        if (depth_m_ > max_depth_m_) max_depth_m_ = depth_m_;
        if (shallow_ms_ >= DIVE_END_MS) {
            end_dive(now);
        } else {
            model_.track_anchor(settings_);
        }
    }

    const GasMix &gas = in_dive_ ? gases_.active() : GAS_AIR;
    ndl_min_ = model_.ndl_min(p_amb, gas, settings_);
    ceiling_m_ = model_.ceiling_m(settings_);
    if (ndl_min_ == 0) {
        model_.plan(p_amb, gases_, in_dive_ ? gases_.active_index() : 0, settings_, plan_);
    } else {
        plan_.count = 0;
        plan_.tts_min = 0;
    }

    if (tick_ms - last_save_ >= SAVE_PERIOD_MS) save(tick_ms, now);
}
