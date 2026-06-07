#include <MainWindow.hpp>

#include <QHBoxLayout>
#include <SyncFlag.hpp>

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
{
    setupUi();
}

void MainWindow::dataReady(QString msg) {
    setWindowTitle(msg);
    update();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    sync::running = false;
    event->accept();
}

void MainWindow::setupUi() {
    resize(1024, 768);
    QHBoxLayout* layout = new QHBoxLayout(this);
    plot = new QCustomPlot(this);
    layout->addWidget(plot);
    setWindowTitle("CPU monitor");
}
