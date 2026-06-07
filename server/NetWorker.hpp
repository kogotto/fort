#pragma once

#include <QObject>
#include <QString>
#include <QUdpSocket>

class NetWorker : public QObject {
Q_OBJECT
public:
    NetWorker(QObject* parent = nullptr);
signals:
    void dataReady(QString msg);
    void finished();
private slots:
    void processData();
private:
    QUdpSocket* socket;
};
