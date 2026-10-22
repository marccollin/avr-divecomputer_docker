#include "configComputer.h"
#include "constants.h"

ConfigComputer configComputer;

void ConfigComputer::set_system_unit(unitSystem unitsType){
    this->units = unitsType;
}

unitSystem ConfigComputer::get_system_unit() {
    return units;
}

waterType ConfigComputer::get_water_type() {
    return defaultWaterType;
}

void ConfigComputer::set_water_type(waterType type) {
    defaultWaterType = type;
}

float ConfigComputer::get_hydrostatic_pressure() {
    if (defaultWaterType == waterType::FreshWater) {
        return GlobalConstants::fresh_waster_hydrostatic;
    }
    //salt water
    return GlobalConstants::salt_water_hydrostactif;
}
