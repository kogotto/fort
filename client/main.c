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

#define SUCCESS 0
#define CAN_NOT_OPEN_FILE 1

int readProcStat(CpuInfo* info) {
    FILE* file = fopen("/proc/stat", "r");
    if (!file) {
        return CAN_NOT_OPEN_FILE;
    }

    char line[MAX_LINE];
    info->coreCount = 0;
    printf("  before loop\n");
    while (!feof(file)) {
        fscanf(file, "%s", line);
        const int len = strlen(line);
        printf("%d %s\n", len, line);
        fgets(line, MAX_LINE - 1, file);
    }

    return SUCCESS;

}

int main() {
    printf("Hello\n");
    CpuInfo info;
    switch (readProcStat(&info)) {
    case 0:
        printf("success\n");
        break;
    default:
        printf("failure\n");
    }
    return 0;
}
