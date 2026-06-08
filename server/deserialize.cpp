#include <deserialize.hpp>

#include <QTextStream>

namespace {

using RawCoreLoad = uint32_t;

constexpr double EXP = 1000000.0;
constexpr int PERCENT = 100;

CoreLoad deserializeCoreLoad(RawCoreLoad load) {
    return load / EXP * PERCENT;
}

} // namespace

Load deserializeLoad(const char* msg) {
    QString str{msg};
    QTextStream stream{&str, QIODeviceBase::ReadOnly};
    Load result;
    RawCoreLoad rawAll;
    stream >> rawAll;
    result.all = deserializeCoreLoad(rawAll);
    int n;
    stream >> n;
    for (int i = 0; i < n; ++i) {
        RawCoreLoad rawCore;
        stream >> rawCore;
        result.cores.push_back(deserializeCoreLoad(rawCore));
    }
    return result;
}
