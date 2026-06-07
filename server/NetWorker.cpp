#include <NetWorker.hpp>

#include <QThread>

#include <SyncFlag.hpp>

NetWorker::NetWorker(QObject* parent)
    : QObject(parent)
{
}

void NetWorker::run() {
    while (sync::running) {
        for (int i = 0; ; ++i) {
            QThread::sleep(1);
            emit dataReady(QString("some text for title " +
                           QString::number(i)));
        }
    }
    emit finished();
}
