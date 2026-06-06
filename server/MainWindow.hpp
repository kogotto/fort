#pragma once

#include <QWidget>
#include <qcustomplot.h>

class MainWindow : public QWidget {
public:
    MainWindow(QWidget* parent = nullptr);
private:
    void setupUi();

    QCustomPlot* plot;
};
