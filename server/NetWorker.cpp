#include <NetWorker.hpp>

#include <SyncFlag.hpp>

namespace {

constexpr int MAX_MSG_SIZE = 1024;

} // namespace

NetWorker::NetWorker(QObject* parent)
    : QObject(parent)
    , socket(new QUdpSocket(this))
{
    socket->bind(QHostAddress::LocalHost, 1234);
    connect(socket, &QUdpSocket::readyRead,
            this, &NetWorker::processData);
}

void NetWorker::processData() {
    char msg[MAX_MSG_SIZE];
    const int result = socket->readDatagram(
            msg,
            MAX_MSG_SIZE - 1
            );
    if (result == -1) {
        return;
    }
    msg[result] = '\0';
    emit dataReady(deserializeLoad(msg));
}
