#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QFileDialog>
#include<QDebug>
#include<QPainter>
#include<QGraphicsView>
#include<QRegExp>
#include<QMessageBox>
#include<QStandardItem>
#include<QStandardItemModel>
#include"data.h"
#include"math/processing.h"

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

private:
    Ui::MainWindow *ui;
    QVector<Data*> tables;
    int timeCounts;
    QStringList dataList;
    int actualStep;

private:
    Data* readData(QString fileName);
};

#endif // MAINWINDOW_H
