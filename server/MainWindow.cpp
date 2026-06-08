#include <MainWindow.hpp>

#include <QHBoxLayout>
#include <QPen>

#include <SyncFlag.hpp>

namespace {

int now() {
    static int tick = 0;
    return tick++;
}

constexpr size_t colorsCount = 8;
QColor colors[colorsCount] = {
    QColor{255, 0, 0, 255},
    QColor{0, 255, 0, 255},
    QColor{255, 255, 0, 255},
    QColor{255, 0, 255, 255},
    QColor{0, 255, 255, 255},
    QColor{255, 255, 255, 255},
    QColor{127, 127, 0, 255},
    QColor{0, 127, 127, 255},
};

const QColor& getColor(int i) {
    return colors[i % colorsCount];
}

} // namespace

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
{
    setupUi();

    plot->addGraph();
    plot->xAxis->setRange(0, 100);
    plot->yAxis->setRange(0, 100);
}

void MainWindow::dataReady(Load load) {
    addDataPiece(load);
    updatePlot();
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

void MainWindow::addDataPiece(const Load& load) {
    if (!plotData) {
        plotData.emplace();
        plotData->cores.resize(load.cores.size());
    }

    plotData->time.emplace_back(now());
    plotData->all.emplace_back(load.all);
    for (int i = 0; i < load.cores.size(); ++i) {
        plotData->cores[i].emplace_back(load.cores[i]);
    }
}

void MainWindow::updatePlot() {
    if (!plotData) {
        return;
    }

    plot->graph(0)->setData(plotData->time, plotData->all);
    addGraphsIfNeaded(plotData->cores.size());
    for (int i = 0; i < plotData->cores.size(); ++i)
        plot->graph(i + 1)->setData(plotData->time, plotData->cores[i]);

    plot->replot();
}

void MainWindow::addGraphsIfNeaded(int newSize) {
    for (int i = plot->graphCount() - 1; i < newSize; ++i) {
        QCPGraph* graph = plot->addGraph();
        QPen pen{getColor(i)};
        graph->setPen(pen);
    }
}
