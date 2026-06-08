#include <serialize.h>

#include <stdint.h>
#include <math.h>

typedef uint32_t ConvertedLoad;
#define EXP 1000000.0

ConvertedLoad convertLoad(CoreLoad load) {
    return (ConvertedLoad)round(load * EXP);
}

CoreLoad restoreLoad(ConvertedLoad load) {
    return (CoreLoad)(load / EXP);
}

int serializeCpuLoadTo(CpuLoad* load, char* buf, int max) {
    int writed = 0;
    writed += sprintf(buf + writed, "%u", convertLoad(load->all));
    writed += sprintf(buf + writed, " %d", load->coreCount);
    for (int i = 0; i < load->coreCount; ++i) {
        writed += sprintf(buf + writed, " %u", convertLoad(load->cores[i]));
    }
    return writed;
}
