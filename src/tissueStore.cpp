#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stm32f4xx_hal.h>

#include "sdcard.h"
#include "tissueStore.h"

TissueStore tissueStore;

namespace {
const char *FILE_NAME = "tissue.txt";
const char *TMP_NAME = "tissue.tmp";
const int VERSION = 1;

bool write_line(FIL &f, const char *line)
{
    return f_puts(line, &f) >= 0;
}

bool write_array(FIL &f, const char *key, const float *v, int n)
{
    char line[24];
    if (!write_line(f, key)) return false;
    for (int i = 0; i < n; ++i) {
        snprintf(line, sizeof(line), " %.5f", v[i]);
        if (!write_line(f, line)) return false;
    }
    return write_line(f, "\n");
}

bool read_array(char *line, const char *key, float *v, int n)
{
    const size_t len = strlen(key);
    if (strncmp(line, key, len) != 0) return false;
    char *p = line + len;
    for (int i = 0; i < n; ++i) {
        char *end;
        v[i] = strtof(p, &end);
        if (end == p || v[i] < 0.0f || v[i] > 20.0f) return false;
        p = end;
    }
    return true;
}
}

bool TissueStore::mount()
{
    if (!mounted_) mounted_ = f_mount(&sdcard_ff, "", 1) == FR_OK;
    return mounted_;
}

//Writes a temporary file then renames it, so a power loss never leaves a half-written tissue.txt.
bool TissueStore::save(const TissueState &t, float gf_anchor, time_t timestamp)
{
    if (!mount()) return false;

    FIL f;
    if (f_open(&f, TMP_NAME, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) {
        mounted_ = false;
        return false;
    }
    char line[48];
    snprintf(line, sizeof(line), "version %d\ntime %ld\nanchor %.4f\n",
             VERSION, (long) timestamp, gf_anchor);
    bool ok = write_line(f, line)
        && write_array(f, "n2", t.n2, TissueState::COUNT)
        && write_array(f, "he", t.he, TissueState::COUNT);
    ok = (f_close(&f) == FR_OK) && ok;
    if (!ok) return false;

    f_unlink(FILE_NAME);
    return f_rename(TMP_NAME, FILE_NAME) == FR_OK;
}

bool TissueStore::load(TissueState &t, float &gf_anchor, time_t &timestamp)
{
    if (!mount()) return false;

    FIL f;
    if (f_open(&f, FILE_NAME, FA_OPEN_EXISTING | FA_READ) != FR_OK) return false;

    TissueState tmp;
    char line[256];
    int version = 0;
    long ts = 0;
    float anchor = 0.0f;
    bool have_n2 = false, have_he = false;
    while (f_gets(line, sizeof(line), &f)) {
        if (sscanf(line, "version %d", &version) == 1) continue;
        if (sscanf(line, "time %ld", &ts) == 1) continue;
        if (strncmp(line, "anchor ", 7) == 0) { anchor = strtof(line + 7, NULL); continue; }
        if (read_array(line, "n2", tmp.n2, TissueState::COUNT)) { have_n2 = true; continue; }
        if (read_array(line, "he", tmp.he, TissueState::COUNT)) { have_he = true; continue; }
    }
    f_close(&f);

    if (version != VERSION || !have_n2 || !have_he) return false;
    t = tmp;
    gf_anchor = anchor;
    timestamp = (time_t) ts;
    return true;
}

bool TissueStore::saved_time(time_t &timestamp)
{
    TissueState t;
    float anchor;
    return load(t, anchor, timestamp);
}
