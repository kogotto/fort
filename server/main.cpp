#include <QApplication>

#include <MainWindow.hpp>
#include <QThread>
#include <NetWorker.hpp>

int main(int argc, char* argv[]) {
    QApplication app{argc, argv};
    MainWindow window;

    QThread* netThread = new QThread();
    QObject::connect(netThread, &QThread::finished, netThread, &QThread::deleteLater);
    NetWorker* worker = new NetWorker{};
    worker->moveToThread(netThread);
    QObject::connect(netThread, &QThread::started, worker, &NetWorker::run);
    QObject::connect(worker, &NetWorker::finished, netThread, &QThread::quit);
    QObject::connect(worker, &NetWorker::finished, worker, &QObject::deleteLater);
    QObject::connect(worker, &NetWorker::dataReady, &window, &MainWindow::dataReady);

    netThread->start();
    window.show();
    return app.exec();
}
