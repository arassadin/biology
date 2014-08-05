#include "plot2d.h"

Plot2D::Plot2D(QWidget *parent) : QWidget(parent),  ui(new Ui::Plot2D)
{
    ui->setupUi(this);
}

Plot2D::~Plot2D()
{
    delete ui;
}


QCustomPlot* Plot2D::getPlotWidget()
{
    return ui->plotWidget;
}
