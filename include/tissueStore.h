#ifndef TISSUESTORE_H
#define TISSUESTORE_H

#include <time.h>

#include "buhlmannModel.h"

//Persists tissue loading to tissue.txt on the SD card.
class TissueStore {
public:
    TissueStore() : mounted_(false) {}
    bool save(const TissueState &t, float gf_anchor, time_t timestamp);
    bool load(TissueState &t, float &gf_anchor, time_t &timestamp);
    //Timestamp of the last save, used to restore a lost clock.
    bool saved_time(time_t &timestamp);
private:
    bool mount();
    bool mounted_;
};

extern TissueStore tissueStore;

#endif
