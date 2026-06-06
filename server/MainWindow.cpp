#include <MainWindow.hpp>

#include <QHBoxLayout>

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
{
    setupUi();
}

void MainWindow::setupUi() {
    resize(1024, 768);
    QHBoxLayout* layout = new QHBoxLayout(this);
    plot = new QCustomPlot(this);
    layout->addWidget(plot);
}
