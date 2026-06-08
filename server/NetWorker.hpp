#pragma once

#include <QObject>
#include <QString>
#include <QUdpSocket>

#include <deserialize.hpp>

class NetWorker : public QObject {
Q_OBJECT
public:
    NetWorker(QObject* parent = nullptr);
signals:
    void dataReady(Load load);
    void finished();
private slots:
    void processData();
private:
    QUdpSocket* socket;
};
