#pragma once

#include <QWidget>
#include <qcustomplot.h>

class MainWindow : public QWidget {
Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
public slots:
    void dataReady(QString msg);
protected:
    void closeEvent(QCloseEvent* event) override;
private:
    void setupUi();

    QCustomPlot* plot;
};
