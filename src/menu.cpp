#include <stdio.h>
#include <string.h>
#include <time.h>

#include "configComputer.h"
#include "menu.h"
#include "rtc.h"
#include "units.h"

namespace {
const int ROW_H = 10;
const int TITLE_H = 11;
const int MENU_ROWS = 5;

void draw_title(u8g_t *u8g, const char *title)
{
    u8g_SetColorIndex(u8g, 3);
    u8g_DrawStr(u8g, 2, 0, title);
    u8g_DrawHLine(u8g, 0, TITLE_H - 2, 128);
}

//One row: label on the left, optional value on the right, inverted when highlighted.
void draw_row(u8g_t *u8g, int y, const char *label, const char *value, bool highlight)
{
    u8g_SetColorIndex(u8g, 3);
    if (highlight) {
        u8g_DrawBox(u8g, 0, y, 128, ROW_H);
        u8g_SetColorIndex(u8g, 0);
    }
    u8g_DrawStr(u8g, 2, y + 1, label);
    if (value != nullptr && value[0] != '\0') {
        u8g_DrawStr(u8g, 126 - u8g_GetStrWidth(u8g, value), y + 1, value);
    }
}

int percent(float fraction) { return (int) (fraction * 100.0f + 0.5f); }

bool is_leap(int y) { return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0; }

int days_in_month(int year, int month)
{
    static const int DAYS[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && is_leap(year)) return 29;
    return DAYS[month - 1];
}

const char *const WATER_NAMES[] = {"FRESH", "SALT"};
const char *const UNIT_NAMES[] = {"METRIC", "IMPERIAL"};
const char *const LANG_NAMES[] = {"FRANCAIS", "ENGLISH"}; // order of enum lang { fr, eng }
} // namespace

// ---------------------------------------------------------------- MenuPage

void MenuPage::on_button(Ui &ui, ButtonId id)
{
    const int n = count();
    if (n <= 0) {
        ui.back();
        return;
    }
    if (selected >= n) selected = n - 1;

    switch (id) {
    case ButtonId::First:
        selected = (selected + 1) % n;
        break;
    case ButtonId::Second:
        ui.back();
        return;
    case ButtonId::Third:
        select(ui, selected);
        return;
    }
    if (selected < top) top = selected;
    if (selected >= top + MENU_ROWS) top = selected - MENU_ROWS + 1;
}

void MenuPage::draw(u8g_t *u8g)
{
    u8g_SetFont(u8g, u8g_font_helvR08);
    u8g_SetFontPosTop(u8g);
    draw_title(u8g, title);

    const int n = count();
    if (selected >= n) selected = n > 0 ? n - 1 : 0;
    if (top > selected) top = selected;

    char buf[28];
    for (int row = 0; row < MENU_ROWS && top + row < n; ++row) {
        const int i = top + row;
        label(i, buf, sizeof(buf));
        //A tab separates the item name from its current value (right aligned).
        char *value = strchr(buf, '\t');
        if (value != nullptr) *value++ = '\0';
        draw_row(u8g, TITLE_H + row * ROW_H, buf, value, i == selected);
    }
    u8g_SetFontPosBaseline(u8g);
}

// ---------------------------------------------------------------- EditPage

void EditPage::clamp(void)
{
    for (int i = 0; i < count; ++i) {
        if (fields[i].value < min_for(i)) fields[i].value = min_for(i);
        if (fields[i].value > max_for(i)) fields[i].value = max_for(i);
    }
}

void EditPage::on_button(Ui &ui, ButtonId id)
{
    EditField &f = fields[field];
    const int lo = min_for(field);
    const int hi = max_for(field);

    switch (id) {
    case ButtonId::First:
        f.value = f.value + f.step > hi ? lo : f.value + f.step;
        break;
    case ButtonId::Second:
        f.value = f.value - f.step < lo ? hi : f.value - f.step;
        break;
    case ButtonId::Third:
        if (field + 1 < count) {
            ++field;
        } else {
            clamp();
            apply(ui);
            return;
        }
        break;
    }
    clamp();
}

void EditPage::draw(u8g_t *u8g)
{
    u8g_SetFont(u8g, u8g_font_helvR08);
    u8g_SetFontPosTop(u8g);
    draw_title(u8g, title);

    char value[16];
    for (int i = 0; i < count; ++i) {
        const EditField &f = fields[i];
        if (f.choices != nullptr) {
            snprintf(value, sizeof(value), "%s", f.choices[f.value - f.min]);
        } else if (f.decimals == 1) {
            snprintf(value, sizeof(value), "%d.%d%s", f.value / 10, f.value % 10, f.suffix ? f.suffix : "");
        } else {
            snprintf(value, sizeof(value), "%d%s", f.value, f.suffix ? f.suffix : "");
        }
        draw_row(u8g, TITLE_H + i * ROW_H, f.label, value, i == field);
    }

    char text[28];
    info(text, sizeof(text));
    if (text[0] != '\0') {
        u8g_SetColorIndex(u8g, 2);
        u8g_DrawStr(u8g, 2, 64 - ROW_H + 1, text);
    }
    u8g_SetFontPosBaseline(u8g);
}

// ---------------------------------------------------------------- concrete pages

namespace {

DiveController *dive = nullptr;

EditField number(const char *label, int value, int min, int max, int step, const char *suffix = nullptr)
{
    EditField f = {label, value, min, max, step, nullptr, 0, suffix};
    return f;
}

EditField choice(const char *label, int value, const char *const *names, int n)
{
    EditField f = {label, value, 0, n - 1, 1, names, 0, nullptr};
    return f;
}

//MIX > MIX n > EDIT  /  MIX > ADD MIX
class MixEditPage final : public EditPage {
public:
    void open(Ui &ui, int gas_index)
    {
        index = gas_index;
        const GasMix g = index < 0 ? GasMix{0.32f, 0.0f} : dive->gases().get(index);
        if (index < 0) snprintf(title, sizeof(title), "ADD MIX");
        else snprintf(title, sizeof(title), "EDIT MIX %d", index + 1);
        fields[0] = number("O2", percent(g.o2), 5, 100, 1, "%");
        fields[1] = number("HE", percent(g.he), 0, 95, 1, "%");
        count = 2;
        ui.show(*this);
    }
protected:
    int max_for(int i) const override
    {
        return i == 1 ? 100 - fields[0].value : fields[i].max;
    }
    void info(char *buf, size_t len) const override
    {
        const DecoSettings &s = dive->settings();
        const float ppo2 = index == 0 ? s.max_ppo2_bottom : s.max_ppo2_deco;
        float mod = (ppo2 / (fields[0].value / 100.0f) - dive->surface_pressure()) / s.bar_per_m;
        if (mod < 0.0f) mod = 0.0f;
        char depth[12];
        units::format_depth(mod, configComputer.get_system_unit(), depth, sizeof(depth));
        snprintf(buf, len, "MOD %s (PPO2 %d.%d)", depth, (int) (ppo2 * 10.0f + 0.5f) / 10,
                 (int) (ppo2 * 10.0f + 0.5f) % 10);
    }
    void apply(Ui &ui) override
    {
        const GasMix g = {fields[0].value / 100.0f, fields[1].value / 100.0f};
        if (index < 0) {
            dive->gases().add(g);
            ui.back();
        } else {
            dive->gases().set(index, g);
            ui.back(); //action menu
            ui.back(); //mix list
        }
    }
private:
    int index = -1;
};
MixEditPage mixEdit;

//MIX > MIX n
class MixActionMenu final : public MenuPage {
public:
    void open(Ui &ui, int gas_index)
    {
        index = gas_index;
        snprintf(title, sizeof(title), "MIX %d", index + 1);
        ui.show(*this);
    }
protected:
    int count() const override { return index > 0 ? 3 : 2; }
    void label(int i, char *buf, size_t len) const override
    {
        static const char *const NAMES[] = {"USE FOR DIVE", "EDIT", "DELETE"};
        snprintf(buf, len, "%s", NAMES[i]);
    }
    void select(Ui &ui, int i) override
    {
        if (i == 0) {
            dive->switch_gas(index);
            ui.go_home();
        } else if (i == 1) {
            mixEdit.open(ui, index);
        } else {
            dive->gases().remove(index);
            ui.back();
        }
    }
private:
    int index = 0;
};
MixActionMenu mixAction;

//MIX: list of gases. During a dive, selecting a mix switches to it.
class MixListMenu final : public MenuPage {
public:
    MixListMenu() { snprintf(title, sizeof(title), "MIX"); }
protected:
    bool can_add() const
    {
        return !dive->in_dive() && dive->gases().count() < GasList::MAX_GASES;
    }
    int count() const override { return dive->gases().count() + (can_add() ? 1 : 0); }
    void label(int i, char *buf, size_t len) const override
    {
        const GasList &gases = dive->gases();
        if (i >= gases.count()) { //maybe put a gas limit
            snprintf(buf, len, "ADD MIX");
            return;
        }
        char name[12];
        units::format_gas(gases.get(i), name, sizeof(name));
        snprintf(buf, len, "MIX %d\t%s%s", i + 1, name, i == gases.active_index() ? " *" : "");
    }
    void select(Ui &ui, int i) override
    {
        if (i >= dive->gases().count()) {
            mixEdit.open(ui, -1);
        } else if (dive->in_dive()) {
            dive->switch_gas(i);
            ui.go_home();
        } else {
            mixAction.open(ui, i);
        }
    }
};
MixListMenu mixList;

//DIVE > GF
class GfEditPage final : public EditPage {
public:
    void open(Ui &ui)
    {
        snprintf(title, sizeof(title), "GRADIENT FACTORS");
        fields[0] = number("GF LOW", percent(dive->settings().gf_low), 10, 100, 5, "%");
        fields[1] = number("GF HIGH", percent(dive->settings().gf_high), 10, 100, 5, "%");
        count = 2;
        ui.show(*this);
    }
protected:
    int min_for(int i) const override { return i == 1 ? fields[0].value : fields[i].min; }
    void apply(Ui &ui) override
    {
        dive->settings().gf_low = fields[0].value / 100.0f;
        dive->settings().gf_high = fields[1].value / 100.0f;
        ui.back();
    }
};
GfEditPage gfEdit;

//DIVE > SALINITY
class SalinityEditPage final : public EditPage {
public:
    void open(Ui &ui)
    {
        snprintf(title, sizeof(title), "SALINITY");
        fields[0] = choice("WATER", static_cast<int>(configComputer.get_water_type()), WATER_NAMES, 2);
        count = 1;
        ui.show(*this);
    }
protected:
    void apply(Ui &ui) override
    {
        configComputer.set_water_type(static_cast<waterType>(fields[0].value));
        dive->settings().bar_per_m = configComputer.get_hydrostatic_pressure() / 10.0f;
        ui.back();
    }
};
SalinityEditPage salinityEdit;

// DIVE > PPO2
class Ppo2EditPage final : public EditPage {
public:
    void open(Ui &ui)
    {
        snprintf(title, sizeof(title), "PPO2 MAX");
        fields[0] = number("BOTTOM", (int) (dive->settings().max_ppo2_bottom * 10.0f + 0.5f), 10, 16, 1);
        fields[1] = number("DECO", (int) (dive->settings().max_ppo2_deco * 10.0f + 0.5f), 10, 16, 1);
        fields[0].decimals = 1;
        fields[1].decimals = 1;
        count = 2;
        ui.show(*this);
    }
protected:
    void apply(Ui &ui) override
    {
        dive->settings().max_ppo2_bottom = fields[0].value / 10.0f;
        dive->settings().max_ppo2_deco = fields[1].value / 10.0f;
        ui.back();
    }
};
Ppo2EditPage ppo2Edit;

//DIVE
class DiveMenu final : public MenuPage {
public:
    DiveMenu() { snprintf(title, sizeof(title), "DIVE"); }
protected:
    int count() const override { return 3; }
    void label(int i, char *buf, size_t len) const override
    {
        const DecoSettings &s = dive->settings();
        switch (i) {
        case 0:
            snprintf(buf, len, "GF\t%d/%d", percent(s.gf_low), percent(s.gf_high));
            break;
        case 1:
            snprintf(buf, len, "SALINITY\t%s", WATER_NAMES[static_cast<int>(configComputer.get_water_type())]);
            break;
        default: {
            const int b = (int) (s.max_ppo2_bottom * 10.0f + 0.5f);
            const int d = (int) (s.max_ppo2_deco * 10.0f + 0.5f);
            snprintf(buf, len, "PPO2\t%d.%d/%d.%d", b / 10, b % 10, d / 10, d % 10);
            break;
        }
        }
    }
    void select(Ui &ui, int i) override
    {
        if (i == 0) gfEdit.open(ui);
        else if (i == 1) salinityEdit.open(ui);
        else ppo2Edit.open(ui);
    }
};
DiveMenu diveMenu;

//SYSTEM > CLOCK (UTC, no time zone)
class ClockEditPage final : public EditPage {
public:
    void open(Ui &ui)
    {
        snprintf(title, sizeof(title), "DATE / TIME");
        const time_t now = rtc.now();
        struct tm t;
        gmtime_r(&now, &t);
        int year = t.tm_year + 1900;
        if (year < 2024 || year > 2099) year = 2025;
        fields[0] = number("YEAR", year, 2024, 2099, 1);
        fields[1] = number("MONTH", t.tm_mon + 1, 1, 12, 1);
        fields[2] = number("DAY", t.tm_mday, 1, 31, 1);
        fields[3] = number("HOUR", t.tm_hour, 0, 23, 1);
        fields[4] = number("MINUTE", t.tm_min, 0, 59, 1);
        count = 5;
        clamp();
        ui.show(*this);
    }
protected:
    int max_for(int i) const override
    {
        return i == 2 ? days_in_month(fields[0].value, fields[1].value) : fields[i].max;
    }
    void apply(Ui &ui) override
    {
        struct tm t = {};
        t.tm_year = fields[0].value - 1900;
        t.tm_mon = fields[1].value - 1;
        t.tm_mday = fields[2].value;
        t.tm_hour = fields[3].value;
        t.tm_min = fields[4].value;
        t.tm_isdst = 0;
        rtc.set(mktime(&t), true);
        ui.back();
    }
};
ClockEditPage clockEdit;

//SYSTEM > UNITS / LANGUAGE
class ChoiceEditPage final : public EditPage {
public:
    enum Kind { Units, Language };

    void open(Ui &ui, Kind k)
    {
        kind = k;
        if (kind == Units) {
            snprintf(title, sizeof(title), "UNITS");
            fields[0] = choice("UNITS", static_cast<int>(configComputer.get_system_unit()), UNIT_NAMES, 2);
        } else {
            snprintf(title, sizeof(title), "LANGUAGE");
            fields[0] = choice("LANGUAGE", static_cast<int>(configComputer.get_lang()), LANG_NAMES, 2);
        }
        count = 1;
        ui.show(*this);
    }
protected:
    void apply(Ui &ui) override
    {
        if (kind == Units) configComputer.set_system_unit(static_cast<unitSystem>(fields[0].value));
        else configComputer.set_lang(static_cast<lang>(fields[0].value));
        ui.back();
    }
private:
    Kind kind = Units;
};
ChoiceEditPage choiceEdit;

//SYSTEM
class SystemMenu final : public MenuPage {
public:
    SystemMenu() { snprintf(title, sizeof(title), "SYSTEM"); }
protected:
    int count() const override { return 3; }
    void label(int i, char *buf, size_t len) const override
    {
        switch (i) {
        case 0:
            if (rtc.needs_setting()) {
                snprintf(buf, len, "CLOCK\tNOT SET");
            } else {
                const time_t now = rtc.now();
                struct tm t;
                gmtime_r(&now, &t);
                snprintf(buf, len, "CLOCK\t%02d/%02d %02d:%02d", t.tm_mday, t.tm_mon + 1, t.tm_hour, t.tm_min);
            }
            break;
        case 1:
            snprintf(buf, len, "UNITS\t%s", UNIT_NAMES[static_cast<int>(configComputer.get_system_unit())]);
            break;
        default:
            snprintf(buf, len, "LANGUAGE\t%s", LANG_NAMES[static_cast<int>(configComputer.get_lang())]);
            break;
        }
    }
    void select(Ui &ui, int i) override
    {
        if (i == 0) clockEdit.open(ui);
        else if (i == 1) choiceEdit.open(ui, ChoiceEditPage::Units);
        else choiceEdit.open(ui, ChoiceEditPage::Language);
    }
};
SystemMenu systemMenu;

//MENU (only MIX during a dive)
class RootMenu final : public MenuPage {
public:
    RootMenu() { snprintf(title, sizeof(title), "MENU"); }
protected:
    int count() const override { return dive->in_dive() ? 1 : 3; }
    void label(int i, char *buf, size_t len) const override
    {
        static const char *const NAMES[] = {"MIX", "DIVE", "SYSTEM"};
        snprintf(buf, len, "%s", NAMES[i]);
    }
    void select(Ui &ui, int i) override
    {
        if (i == 0) ui.show(mixList);
        else if (i == 1) ui.show(diveMenu);
        else ui.show(systemMenu);
    }
};
RootMenu rootMenu;

} // namespace

MenuSystem::MenuSystem(DiveController &dive_controller)
{
    dive = &dive_controller;
}

Page &MenuSystem::root(void)
{
    return rootMenu;
}
