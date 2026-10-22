#ifndef GASMIX_H
#define GASMIX_H

//Breathing gas as fractions (0..1). N2 is the remainder.
struct GasMix {
    float o2;
    float he;

    float n2() const { return 1.0f - o2 - he; }
    float ppo2(float p_amb) const { return o2 * p_amb; }
    //Maximum ambient pressure (bar) for a ppO2 limit.
    float max_pressure(float max_ppo2) const { return max_ppo2 / o2; }
};

static const GasMix GAS_AIR = {0.21f, 0.0f};

//Gases carried for a dive. Index 0 is the bottom gas.
class GasList {
public:
    static const int MAX_GASES = 5;

    GasList() : count_(1), active_(0) { gases_[0] = GAS_AIR; }

    int count() const { return count_; }
    int active_index() const { return active_; }
    const GasMix &active() const { return gases_[active_]; }
    const GasMix &get(int idx) const { return gases_[idx]; }

    void clear(const GasMix &bottom) { gases_[0] = bottom; count_ = 1; active_ = 0; }

    bool add(const GasMix &gas) {
        if (count_ >= MAX_GASES || gas.o2 <= 0.0f || gas.o2 + gas.he > 1.0f) return false;
        gases_[count_++] = gas;
        return true;
    }

    bool set(int idx, const GasMix &gas) {
        if (idx < 0 || idx >= count_ || gas.o2 <= 0.0f || gas.o2 + gas.he > 1.0f) return false;
        gases_[idx] = gas;
        return true;
    }

    //Removes a deco gas (index 0, the bottom gas, cannot be removed).
    bool remove(int idx) {
        if (idx <= 0 || idx >= count_) return false;
        for (int i = idx; i < count_ - 1; ++i) gases_[i] = gases_[i + 1];
        --count_;
        if (active_ == idx) active_ = 0;
        else if (active_ > idx) --active_;
        return true;
    }

    bool set_active(int idx) {
        if (idx < 0 || idx >= count_) return false;
        active_ = idx;
        return true;
    }

    //Richest O2 gas usable at p_amb without exceeding max_ppo2; -1 if none.
    int best_for(float p_amb, float max_ppo2) const {
        int best = -1;
        for (int i = 0; i < count_; ++i) {
            if (gases_[i].ppo2(p_amb) > max_ppo2) continue;
            if (best < 0 || gases_[i].o2 > gases_[best].o2) best = i;
        }
        return best;
    }

private:
    GasMix gases_[MAX_GASES];
    int count_;
    int active_;
};

#endif
