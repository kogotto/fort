#pragma once

#include <QObject>
#include <QString>

class NetWorker : public QObject {
Q_OBJECT
public:
    NetWorker(QObject* parent = nullptr);
public slots:
    void run();
signals:
    void dataReady(QString msg);
    void finished();
private:
};
