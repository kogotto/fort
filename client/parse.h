#ifndef PARSE_H
#define PARSE_H

#include <stdint.h>
#include <stdio.h>

#include <defs.h>

typedef struct {
    uint64_t user;
    uint64_t nice;
    uint64_t system;
    uint64_t idle;
    uint64_t iowait;
    uint64_t irq;
    uint64_t softirq;
    uint64_t steal;
    uint64_t guest;
    uint64_t guestNice;
} CoreInfo;

typedef struct {
    CoreInfo all;
    CoreInfo cores[MAX_CORES];
    int coreCount;
} CpuInfo;

void printCoreInfo(CoreInfo* info);
void printCpuInfo(CpuInfo* info);

int readCpuStat(CoreInfo* coreInfo, FILE* file);
int readProcStat(CpuInfo* info);

#endif
