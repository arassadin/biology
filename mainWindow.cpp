#include "mainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timeCounts=0;
    ui->tableView->setVisible(false);
    actualStep=0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

Data *MainWindow::readData(QString fileName)
{
    QFile f;
    f.setFileName(fileName);
    f.open(QIODevice::ReadOnly);
    QString tmpStr=f.readLine();
    int fieldsCount=tmpStr.toInt();
    qDebug() << fieldsCount;
    Data* tmpData=new Data();
    tmpData->setSize(fieldsCount);
    if(!tmpData->allocate())
    {
        qDebug() << "error1!";
        return (Data*)0;
    }
    for(int i=0; i<fieldsCount; i++)
    {
        tmpStr=f.readLine();
        QStringList tmpStrList=tmpStr.split(" ");
        if(tmpStrList.count()>2)
        {
            qDebug() << "error2!" << " " << i+1;
            return (Data*)0;
        }
        else
        {
            tmpData->setX(i, tmpStrList.at(0).toDouble());
            tmpData->setY(i, tmpStrList.at(1).toDouble());
            qDebug() << tmpData->getX(i) << " " << tmpData->getY(i);
        }
    }
    return tmpData;
}

void MainWindow::on_startButton_pressed()
{
    if(tables.count()>0)
        tables.clear();

    for(int i=0; i<dataList.count(); i++)
    {
        qDebug() << dataList.at(i) << ":";
        Data* tmp=readData(dataList.at(i));
        if(tmp==(Data*)0)
        {
            qDebug() << "big error1";
            tables.clear();
            return;
        }
        tables.append(tmp);
        qDebug() << tables.at(i)->getX(0) << "_" << tables.at(i)->getY(0);
    }

    for(int i=0; i<dataList.count(); i++)
    {
        math_step_1(tables.at(i));
    }

    qDebug() << "this";
    tables.clear();
}

void MainWindow::on_menuBar_action_OpenData_triggered()
{
    QFileDialog openDataFiles;
    dataList.clear();
    dataList=openDataFiles.getOpenFileNames(0, "Open Data Files", "/home/alexandr/develop/c++/biology", "*");
    qDebug() << "count: " << dataList.count();
    ui->startButton->setEnabled(true);
    ui->tableView->setVisible(true);
}

void MainWindow::on_nextButton_pressed()
{
//    QGraphicsView *g=(QGraphicsView*)qpd;
//    g->setWindowTitle("Graphic #1");
//    g->setFixedSize(600, 300);

//    plot2d* plot=new plot2d();

//    QPainter graphPainter;
//    graphPainter.begin();

//    QPoint* line=new QPoint[8];
//    for(int i=0; i<8; i++)
//        line[i]=QPoint(i+1, i+1);
//    graphPainter.drawPolyline(line, 8);

//    graphPainter.end();

//    g->show();

//    plot->show();

//    QWidget *graph=new QWidget();
//    graph->setWindowTitle("Graphic #1");
//    graph->setFixedSize(600, 300);

    //graph->show();

    QStandardItemModel* model=new QStandardItemModel();
    QStandardItem* tmpItem=new QStandardItem();

    QStringList horHeader;
    horHeader.append("X");
    horHeader.append("Y");

    QStringList vertHeader;
    vertHeader.append("time #1");
    vertHeader.append("time #2");
    vertHeader.append("time #3");

    model->setHorizontalHeaderLabels(horHeader);
    model->setVerticalHeaderLabels(vertHeader);

    //tmpItem->setText();

    ui->tableView->setModel(model);

    int tmpHeight=ui->tableView->horizontalHeader()->height();
    ui->tableView->setRowHeight(0, (ui->tableView->height()-tmpHeight)/3-1);
    ui->tableView->setRowHeight(1, (ui->tableView->height()-tmpHeight)/3);
    ui->tableView->setRowHeight(2, (ui->tableView->height()-tmpHeight)/3);

    int tmpWidth=ui->tableView->verticalHeader()->width();
    ui->tableView->setColumnWidth(0, (ui->tableView->width()-tmpWidth)/2-1);
    ui->tableView->setColumnWidth(1, (ui->tableView->width()-tmpWidth)/2);
}
