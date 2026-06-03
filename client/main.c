#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAX_CORES 32
#define MAX_LINE 1024

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

void printCoreInfo(CoreInfo* info) {
    printf("user: %lu\n", info->user);
    printf("nice: %lu\n", info->nice);
    printf("system: %lu\n", info->system);
    printf("idle: %lu\n", info->idle);
    printf("iowait: %lu\n", info->iowait);
    printf("irq: %lu\n", info->irq);
    printf("softirq: %lu\n", info->softirq);
    printf("steal: %lu\n", info->steal);
    printf("guest: %lu\n", info->guest);
    printf("guestNice: %lu\n", info->guestNice);
}

void printCpuInfo(CpuInfo* info) {
    printf("All:\n");
    printCoreInfo(&info->all);
    printf("\n");
    for (int core = 0; core < info->coreCount; ++core) {
        printf("cpu%d:\n", core);
        printCoreInfo(&info->cores[core]);
        printf("\n");
    }
}

#define SUCCESS 0
#define CAN_NOT_OPEN_FILE 1
#define CAN_NOT_READ_FIELD 2

int readCpuStat(CoreInfo* coreInfo, FILE* file) {
    if (EOF == fscanf(file, "%lu", &coreInfo->user)) {
        return CAN_NOT_READ_FIELD;
    }
    if (EOF == fscanf(file, "%lu", &coreInfo->nice)) {
        return CAN_NOT_READ_FIELD;
    }
    if (EOF == fscanf(file, "%lu", &coreInfo->system)) {
        return CAN_NOT_READ_FIELD;
    }
    if (EOF == fscanf(file, "%lu", &coreInfo->idle)) {
        return CAN_NOT_READ_FIELD;
    }
    if (EOF == fscanf(file, "%lu", &coreInfo->iowait)) {
        return CAN_NOT_READ_FIELD;
    }
    if (EOF == fscanf(file, "%lu", &coreInfo->irq)) {
        return CAN_NOT_READ_FIELD;
    }
    if (EOF == fscanf(file, "%lu", &coreInfo->softirq)) {
        return CAN_NOT_READ_FIELD;
    }
    if (EOF == fscanf(file, "%lu", &coreInfo->steal)) {
        return CAN_NOT_READ_FIELD;
    }
    if (EOF == fscanf(file, "%lu", &coreInfo->guest)) {
        return CAN_NOT_READ_FIELD;
    }
    if (EOF == fscanf(file, "%lu", &coreInfo->guestNice)) {
        return CAN_NOT_READ_FIELD;
    }

    return SUCCESS;
}

int readProcStat(CpuInfo* info) {
    FILE* file = fopen("/proc/stat", "r");
    if (!file) {
        return CAN_NOT_OPEN_FILE;
    }

    char line[MAX_LINE];
    info->coreCount = 0;
    while (!feof(file)) {
        fscanf(file, "%s", line);
        if (strncmp(line, "cpu", 3)) {
            continue;
        }
        const int len = strlen(line);
        if (len == 3) {
            const int res = readCpuStat(&info->all, file);
            if (res != SUCCESS) {
                fclose(file);
                return res;
            }
            continue;
        }

        int n;
        sscanf(line, "cpu%d", &n);
        const int res = readCpuStat(&info->cores[n], file);
        if (res != SUCCESS) {
            fclose(file);
            return res;
        }
        ++info->coreCount;
    }

    fclose(file);
    return SUCCESS;

}

int main() {
    printf("Hello\n");
    CpuInfo info;
    switch (readProcStat(&info)) {
    case 0:
        printCpuInfo(&info);
        break;
    default:
        printf("failure\n");
        break;
    }
    return 0;
}
