#include <load.h>

static uint64_t total(CoreInfo* info) {
    return info->user +
        info->nice +
        info->system +
        info->idle +
        info->iowait +
        info->irq +
        info->softirq +
        info->steal;
}

static uint64_t idle(CoreInfo* info) {
    return info->idle + info->iowait;
}

CoreLoad calculateCoreLoad(CoreInfo* first, CoreInfo* second) {
    const double totalDelta = total(second) - total(first);
    const double idleDelta = idle(second) - idle(first);

    return (totalDelta - idleDelta) / totalDelta;
}

CpuLoad calculateCpuLoad(CpuInfo* first, CpuInfo* second) {
    CpuLoad load;
    load.all = calculateCoreLoad(&first->all, &second->all);
    load.coreCount = first->coreCount;
    for (int i = 0; i < load.coreCount; ++i) {
        load.cores[i] = calculateCoreLoad(
                &first->cores[i],
                &second->cores[i]
        );
    }

    return load;
}
