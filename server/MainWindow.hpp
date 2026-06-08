#pragma once

#include <optional>

#include <QWidget>
#include <qcustomplot.h>

#include <deserialize.hpp>

struct PlotData {
    QVector<double> all;
    QVector<QVector<double>> cores;
    QVector<double> time;
};

class MainWindow : public QWidget {
Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
public slots:
    void dataReady(Load load);
protected:
    void closeEvent(QCloseEvent* event) override;
private:
    void setupUi();

    void addDataPiece(const Load& load);
    void updatePlot();
    void addGraphsIfNeaded(int newSize);

    QCustomPlot* plot;
    std::optional<PlotData> plotData;
};
