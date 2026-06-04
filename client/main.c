#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include <parse.h>
#include <load.h>

#define CAN_NOT_WRITE_CHARS 1
#define BUFFER_OVERFLOW 2

void* netTreadMain(void* param) {
    printf("Hello from net thread");
    return NULL;
}

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

    pthread_t thread;
    pthread_create(&thread, NULL, netTreadMain, NULL);
    void* res;
    pthread_join(thread, &res);
    return 0;
}
