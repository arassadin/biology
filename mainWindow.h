#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define FIRST_OLS_FUNC_Q 3
#define SECOND_OLS_FUNC_Q 1
#define STEP 0.01

#include<QMainWindow>
#include<QFileDialog>
#include<QDebug>
#include<QPainter>
#include<QGraphicsView>
#include<QRegExp>
#include<QMessageBox>
#include<QStandardItem>
#include<QStandardItemModel>
#include<QKeyEvent>
#include<QPainter>
#include"defines/data.h"
#include"defines/koefs.h"
#include"math/processing.h"
#include"math/ols_polynom.h"
#include"math/ols_sin.h"
#include"math/ols_cos.h"
#include"defines/defines.h"
#include"plots/plot2d.h"
#include"plots/qcustomplot.h"

#include "ui_mainWindow.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_startButton_pressed();
    void on_menuBar_action_OpenData_triggered();
    void on_nextButton_pressed();
    void on_cancelButton_pressed();

private:
    Ui::MainWindow *ui;
    double* t;
    QVector<Data*> tables;
    QVector<Koefs*> step2Koefs;
    QVector<Koefs*> step4Koefs;
    int timesCount;
    QStringList dataList;
    int actualStep;
    QMessageBox* errorMessageBox;
    QVector<double*> appr;
    double getMin(double* array, int q);
    double getMax(double* array, int q);

private:
    Data* readData(QString fileName);
    void revertAll();
    void testFunction();
    virtual void keyPressEvent(QKeyEvent* ke);
};

#endif // MAINWINDOW_H
