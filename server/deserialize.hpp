#pragma once

#include <QVector>

using CoreLoad = double;

struct Load {
    CoreLoad all;
    QVector<CoreLoad> cores;
};

Load deserializeLoad(const char* msg);
