#include <stdio.h>

#include <parse.h>

int serializeCoreInfoTo(CoreInfo* info, char* buf, int max) {
    int writed = 0;
    int current = 0;
    current = sprintf(buf, "%lu", info->user);
    return writed;
}

int main() {
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
