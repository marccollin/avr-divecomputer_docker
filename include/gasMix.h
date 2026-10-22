/*
Breathing-gas fractions and the bounded list of carried gases.
Provides oxygen-pressure limits and gas selection for ascent planning.
*/
#ifndef GASMIX_H
#define GASMIX_H

/*
Breathing gas as fractions 0..1, with o2 + he <= 1 and nitrogen as remainder.
Callers must provide physically valid fractions; this structure does not validate them.
*/
struct GasMix {
    float o2; //Oxygen fraction, 0..1; positive for pressure-limit calculations.
    float he; //Helium fraction, 0..1, no more than 1 - o2.

    //Returns nitrogen fraction 1 - o2 - he, 0..1 for a valid mixture.
    float n2() const { return 1.0f - o2 - he; }
    /*
    Returns oxygen partial pressure in bar at absolute ambient p_amb in bar.
    */
    float ppo2(float p_amb) const { return o2 * p_amb; }
    /*
    Returns maximum absolute pressure in bar for max_ppo2 in bar; requires o2 > 0.
    */
    float max_pressure(float max_ppo2) const { return max_ppo2 / o2; }
};

static const GasMix GAS_AIR = {0.21f, 0.0f}; //Air fractions: 21% oxygen, no helium.

/*
Bounded carried-gas list; index 0 is the non-removable bottom gas.
Indices refer to current ordering and can shift when a gas is removed.
*/
class GasList {
public:
    static const int MAX_GASES = 5; //Capacity, 1..5 gases in use.

    /*
    Initialises a single active bottom gas containing air.
    */
    GasList() : count_(1), active_(0) { gases_[0] = GAS_AIR; }

    //Returns number of carried gases, 1..MAX_GASES.
    int count() const { return count_; }
    //Returns active gas index, 0..count()-1.
    int active_index() const { return active_; }
    //Returns the active mixture by reference.
    const GasMix &active() const { return gases_[active_]; }
    //Returns gas at idx by reference; caller must supply 0..count()-1.
    const GasMix &get(int idx) const { return gases_[idx]; }

    /*
    Replaces all gases with bottom and activates index 0; assumes a valid mixture.
    */
    void clear(const GasMix &bottom) { gases_[0] = bottom; count_ = 1; active_ = 0; }

    /*
    Appends gas and returns true if capacity, positive oxygen and fraction sum permit.
    Gas fractions are 0..1; caller must ensure nonnegative helium.
    */
    bool add(const GasMix &gas) {
        if (count_ >= MAX_GASES || gas.o2 <= 0.0f || gas.o2 + gas.he > 1.0f) return false;
        gases_[count_++] = gas;
        return true;
    }

    /*
    Replaces valid gas index idx; returns false for bad index, oxygen or fraction sum.
    Caller must ensure nonnegative helium in the fractional mixture.
    */
    bool set(int idx, const GasMix &gas) {
        if (idx < 0 || idx >= count_ || gas.o2 <= 0.0f || gas.o2 + gas.he > 1.0f) return false;
        gases_[idx] = gas;
        return true;
    }

    /*
    Removes deco gas idx and shifts later indices; index 0 cannot be removed.
    Returns false for invalid indices; removing the active gas selects the bottom gas.
    */
    bool remove(int idx) {
        if (idx <= 0 || idx >= count_) return false;
        for (int i = idx; i < count_ - 1; ++i) gases_[i] = gases_[i + 1];
        --count_;
        if (active_ == idx) active_ = 0;
        else if (active_ > idx) --active_;
        return true;
    }

    /*
    Activates valid index idx and returns true, or false without changing selection.
    Does not check oxygen partial pressure at the current depth.
    */
    bool set_active(int idx) {
        if (idx < 0 || idx >= count_) return false;
        active_ = idx;
        return true;
    }

    /*
    Returns index of richest oxygen gas at absolute p_amb in bar within max_ppo2 in bar.
    Returns -1 if none qualifies; equal oxygen fractions retain the earliest index.
    */
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
