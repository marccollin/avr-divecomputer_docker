#ifndef CONFIGCOMPUTER_H
#define CONFIGCOMPUTER_H


#include "langEnum.h"
#include "waterTypeEnum.h"
#include "unitEnum.h"

class ConfigComputer {
    lang default_lang = lang::eng;
    waterType defaultWaterType = waterType::FreshWater;
    unitSystem units = unitSystem::Metric;
    
public:
    float get_hydrostatic_pressure();
    void set_system_unit(unitSystem unit);
    unitSystem get_system_unit();

    waterType get_water_type();
    void set_water_type(waterType waterType);

    lang get_lang() { return default_lang; }
    void set_lang(lang l) { default_lang = l; }
};


extern ConfigComputer configComputer;

#endif
