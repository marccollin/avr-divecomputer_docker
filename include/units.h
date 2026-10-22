/*
Display formatting for metric and imperial units and breathing-gas names.
Model values remain in bar, metres and degrees Celsius.
*/
#ifndef UNITS_H
#define UNITS_H

#include <stddef.h>

#include "gasMix.h"
#include "unitEnum.h"

//Display-only conversions. All calculations stay metric (bar, m, degC).
namespace units {

/*
Formats metres using sys as decimal metres or rounded whole feet, e.g. 12.3m or 40ft.
Writes a bounded string to buf of capacity len bytes; no value is returned.
*/
void format_depth(float metres, unitSystem sys, char *buf, size_t len);
/*
Formats stop depth metres using sys; imperial stops map the 3 m grid to 10 ft steps.
Writes a bounded string to buf of capacity len bytes; no value is returned.
*/
void format_stop(int metres, unitSystem sys, char *buf, size_t len);
/*
Formats celsius using sys as rounded degrees Celsius or Fahrenheit.
Writes a bounded string to buf of capacity len bytes; no value is returned.
*/
void format_temperature(float celsius, unitSystem sys, char *buf, size_t len);
/*
Formats absolute bar using sys as whole mbar or one-decimal psi.
Writes a bounded string to buf of capacity len bytes; no value is returned.
*/
void format_pressure(float bar, unitSystem sys, char *buf, size_t len);
/*
Formats fractional gas as AIR, EAN32 or TX18/45, rounding fractions to percent.
Writes a bounded string to buf of capacity len bytes; no value is returned.
*/
void format_gas(const GasMix &gas, char *buf, size_t len);

}

#endif
