#include <math.h>
#include <stdio.h>

#include "units.h"

namespace {
const float FT_PER_M = 3.28084f;
const float PSI_PER_BAR = 14.5038f;
}

namespace units {

void format_depth(float metres, unitSystem sys, char *buf, size_t len)
{
    if (sys == unitSystem::Imperial) snprintf(buf, len, "%dft", (int) lroundf(metres * FT_PER_M));
    else snprintf(buf, len, "%.1fm", metres);
}

//Imperial stops use the usual 10 ft grid (3 m -> 10 ft, 6 m -> 20 ft, ...).
void format_stop(int metres, unitSystem sys, char *buf, size_t len)
{
    if (sys == unitSystem::Imperial) snprintf(buf, len, "%dft", (int) lroundf(metres / 3.0f) * 10);
    else snprintf(buf, len, "%dm", metres);
}

void format_temperature(float celsius, unitSystem sys, char *buf, size_t len)
{
    if (sys == unitSystem::Imperial) snprintf(buf, len, "%dF", (int) lroundf(celsius * 9.0f / 5.0f + 32.0f));
    else snprintf(buf, len, "%dC", (int) lroundf(celsius));
}

void format_pressure(float bar, unitSystem sys, char *buf, size_t len)
{
    if (sys == unitSystem::Imperial) snprintf(buf, len, "%.1fpsi", bar * PSI_PER_BAR);
    else snprintf(buf, len, "%dmbar", (int) lroundf(bar * 1000.0f));
}

void format_gas(const GasMix &gas, char *buf, size_t len)
{
    const int o2 = (int) (gas.o2 * 100.0f + 0.5f);
    const int he = (int) (gas.he * 100.0f + 0.5f);
    if (he > 0) snprintf(buf, len, "TX%d/%d", o2, he);
    else if (o2 == 21) snprintf(buf, len, "AIR");
    else snprintf(buf, len, "EAN%d", o2);
}

}
