#include "mainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timeCounts=0;
    ui->tableView->setVisible(false);
    actualStep=1;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::revertAll()
{
    timeCounts=0;
    actualStep=1;
    tables.clear();
    step2Koefs.clear();

    ui->tableView->setVisible(false);
    ui->startButton->setEnabled(false);
    ui->nextButton->setEnabled(false);
    ui->cancelButton->setEnabled(false);
    ui->menuBar_action_OpenData->setEnabled(true);
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
    for(int i=0; i<dataList.count(); i++)
    {
        mathStep_1(tables.at(i));
    }

/* filling the table */
    QStandardItemModel* model=new QStandardItemModel();

    QStringList horHeader;
    horHeader.append("x' +- dx");
    horHeader.append("y' +- dy");
    horHeader.append("Cs(x)");
    horHeader.append("Cs(y)");

    QStringList vertHeader;
    for(int i=0; i<tables.count(); i++)
    {
        vertHeader.append("time #");
    }

    model->setHorizontalHeaderLabels(horHeader);
    model->setVerticalHeaderLabels(vertHeader);

    for(int i=0; i<tables.count(); i++)
    {
        QStandardItem *tmpItem=new QStandardItem();
        QString tmpStr="";
        //tmpStr+=tables.at(i)->getArithemeticMean_x();
        tmpStr+=" +- ";
        //tmpStr+=tables.at(i)->getConfidenceInterval_x();
        tmpStr.operator +=(1.0);
        tmpItem->setText(tmpStr);
        model->setItem(i, 0, tmpItem);
    }

    ui->tableView->setModel(model);

//    int tmpHeight=ui->tableView->horizontalHeader()->height();
//    ui->tableView->setRowHeight(0, (ui->tableView->height()-tmpHeight)/3-1);
//    ui->tableView->setRowHeight(1, (ui->tableView->height()-tmpHeight)/3);
//    ui->tableView->setRowHeight(2, (ui->tableView->height()-tmpHeight)/3);

//    int tmpWidth=ui->tableView->verticalHeader()->width();
//    ui->tableView->setColumnWidth(0, (ui->tableView->width()-tmpWidth)/2-1);
//    ui->tableView->setColumnWidth(1, (ui->tableView->width()-tmpWidth)/2);

    ui->tableView->setVisible(true);
/* end of filling */

    ui->menuBar_action_OpenData->setEnabled(false);
    ui->startButton->setEnabled(false);
    actualStep++;
    ui->nextButton->setEnabled(true);
}

void MainWindow::on_menuBar_action_OpenData_triggered()
{
    QFileDialog openDataFiles;
    dataList.clear();
    dataList=openDataFiles.getOpenFileNames(0, "Open Data Files", "/home/alexandr/develop/c++/biology", "*");
    qDebug() << "count: " << dataList.count();

    if(tables.count()>0)
        tables.clear();

/* start uploding data files */
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
/* end of uploading */

    ui->startButton->setEnabled(true);
    ui->tableView->setVisible(true);
    ui->cancelButton->setEnabled(true);
}

void MainWindow::on_nextButton_pressed()
{
    switch (actualStep)
    {
    case 1:
        qDebug() << "Next Button: " << "error! step " << actualStep << " is wrong";
        break;

    case 2:
        ui->tableView->model()->removeRows(0, tables.count());
        ui->tableView->model()->removeColumns(0, 4);

        if(step2Koefs.count()>0)
            step2Koefs.clear();

/* OLS for polynom, degree 1 */
        OLS_polynom* ols_polynom_1=new OLS_polynom(1);
        for(int i=0; i<tables.count(); i++)
        {
            Koefs* tmpKoefs=new Koefs();
            tmpKoefs->setType(POLYNOM_1);
            tmpKoefs->a=ols_polynom_1->getSolve(tables.at(i)->getX(), tables.at(i)->getY(), tables.at(i)->getSize());
            qDebug() << tmpKoefs->a[0] << tmpKoefs->a[2] << tmpKoefs->a[2] << tmpKoefs->a[3];
            step2Koefs.append(tmpKoefs);
        }
        delete ols_polynom_1;
/* end of OLS for polynom */

/* OLS for polynom, degree 2 */
        OLS_polynom* ols_polynom_2=new OLS_polynom(2);
        for(int i=0; i<tables.count(); i++)
        {
            Koefs* tmpKoefs=new Koefs();
            tmpKoefs->setType(POLYNOM_2);
            tmpKoefs->a=ols_polynom_2->getSolve(tables.at(i)->getX(), tables.at(i)->getY(), tables.at(i)->getSize());
            qDebug() << tmpKoefs->a[0] << tmpKoefs->a[2] << tmpKoefs->a[2] << tmpKoefs->a[3];
            step2Koefs.append(tmpKoefs);
        }
        delete ols_polynom_2;
/* end of OLS for polynom */

/* OLS for polynom, degree 3 */
        OLS_polynom* ols_polynom_3=new OLS_polynom(3);
        for(int i=0; i<tables.count(); i++)
        {
            Koefs* tmpKoefs=new Koefs();
            tmpKoefs->setType(POLYNOM_3);
            tmpKoefs->a=ols_polynom_3->getSolve(tables.at(i)->getX(), tables.at(i)->getY(), tables.at(i)->getSize());
            qDebug() << tmpKoefs->a[0] << tmpKoefs->a[2] << tmpKoefs->a[2] << tmpKoefs->a[3];
            step2Koefs.append(tmpKoefs);
        }
        delete ols_polynom_3;
/* end of OLS for polynom */

        for(int j=0; j<3; j++)
        {
            int mp=0;
            for(int i=0; i<tables.count(); i++)
                mathStep_2(tables.at(i), step2Koefs.at(mp+i));
            mp+=3;
        }

/* filling the table */
        QStandardItemModel* model=new QStandardItemModel();

        QStringList horHeader;
        horHeader.append("R^2");
        horHeader.append("Оценка");

        QStringList vertHeader;
//        for(int i=0; i<tables.count(); i++)
//        {
            vertHeader.append("Полиномиальная (степень 3)");
//        }

        model->setHorizontalHeaderLabels(horHeader);
        model->setVerticalHeaderLabels(vertHeader);

        ui->tableView->setModel(model);
        ui->tableView->resizeRowsToContents();
        ui->tableView->resizeColumnsToContents();
/* end of the filling */

        actualStep++;
        break;

    }
}

void MainWindow::on_cancelButton_pressed()
{
    revertAll();
}

void MainWindow::on_actionTest_triggered()
{

}
