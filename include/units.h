#ifndef UNITS_H
#define UNITS_H

#include <stddef.h>

#include "gasMix.h"
#include "unitEnum.h"

//Display-only conversions. All calculations stay metric (bar, m, degC).
namespace units {

//"12.3m" or "40ft"
void format_depth(float metres, unitSystem sys, char *buf, size_t len);
//Deco stop on the 3 m grid: "6m" or "20ft"
void format_stop(int metres, unitSystem sys, char *buf, size_t len);
//"18C" or "64F"
void format_temperature(float celsius, unitSystem sys, char *buf, size_t len);
//"1013mbar" or "14.7psi"
void format_pressure(float bar, unitSystem sys, char *buf, size_t len);
//"AIR", "EAN32" or "TX18/45"
void format_gas(const GasMix &gas, char *buf, size_t len);

}

#endif
