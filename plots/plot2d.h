#ifndef PLOT2D_H
#define PLOT2D_H

#include <QWidget>
#include"plots/qcustomplot.h"

#include "ui_plot2d.h"

namespace Ui
{
    class Plot2D;
}

class Plot2D : public QWidget
{
    Q_OBJECT

public:
    explicit Plot2D(QWidget *parent = 0);
    ~Plot2D();

private:
    Ui::Plot2D *ui;

public:
    QCustomPlot* getPlotWidget();
};

#endif // PLOT2D_H
