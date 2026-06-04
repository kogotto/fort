#ifndef LOAD_H
#define LOAD_H

#include <stdint.h>

#include <defs.h>
#include <parse.h>

typedef double CoreLoad;

typedef struct {
    CoreLoad all;
    CoreLoad cores[MAX_CORES];
    int coreCount;
} CpuLoad;

CpuLoad calculateCpuLoad(CpuInfo* first, CpuInfo* second);

#endif
