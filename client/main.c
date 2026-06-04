#include <stdio.h>
#include <unistd.h>

#include <parse.h>
#include <load.h>

#define CAN_NOT_WRITE_CHARS 1
#define BUFFER_OVERFLOW 2

int main() {
    CpuInfo first;
    switch (readProcStat(&first)) {
    case 0:
        printCpuInfo(&first);
        break;
    default:
        printf("failure\n");
        break;
    }
    sleep(1);
    CpuInfo second;
    switch (readProcStat(&second)) {
    case 0:
        printCpuInfo(&second);
        break;
    default:
        printf("failure\n");
        break;
    }
    CpuLoad load = calculateCpuLoad(&first, &second);
    printf("%.2f", load.all);
    for (int i = 0; i < load.coreCount; ++i) {
        printf(" %.2f", load.cores[i]);
    }
    return 0;
}
