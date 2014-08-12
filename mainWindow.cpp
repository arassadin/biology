#include "mainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timeCounts=0;
    ui->tableView->setVisible(false);
    actualStep=1;
    errorMessageBox=new QMessageBox("An error occured", "Error", QMessageBox::Critical, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
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
    step4Koefs.clear();

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
        errorMessageBox->setText("Can't allocate memory!");
        return (Data*)0;
    }
    for(int i=0; i<fieldsCount; i++)
    {
        tmpStr=f.readLine();
        QStringList tmpStrList=tmpStr.split(" ");
        if(tmpStrList.count()>2)
        {
            errorMessageBox->setText("Bad data file!");
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
    horHeader.append("x' +/- dx");
    horHeader.append("y' +/- dy");
    horHeader.append("Cs(x)");
    horHeader.append("Cs(y)");

    QStringList vertHeader;
    for(int i=0; i<timeCounts; i++)
    {
        QString tmpStr="time #"+QString::number(i+1);
        vertHeader.append(tmpStr);
    }

    model->setHorizontalHeaderLabels(horHeader);
    model->setVerticalHeaderLabels(vertHeader);

    for(int i=0; i<tables.count(); i++)
    {
        QStandardItem *tmpItem=new QStandardItem();
        QString tmpStr="";
        tmpStr+=QString::number(tables.at(i)->getArithemeticMean_x());
        tmpStr+=" +/- ";
        tmpStr+=QString::number(tables.at(i)->getConfidenceInterval_x());
        tmpItem->setText(tmpStr);
        model->setItem(i, 0, tmpItem);

        tmpItem=new QStandardItem();
        tmpStr="";
        tmpStr+=QString::number(tables.at(i)->getArithemeticMean_y());
        tmpStr+=" +/- ";
        tmpStr+=QString::number(tables.at(i)->getConfidenceInterval_y());
        tmpItem->setText(tmpStr);
        model->setItem(i, 1, tmpItem);

        tmpItem=new QStandardItem();
        tmpStr="";
        tmpStr+=QString::number(tables.at(i)->getAccurance_x());
        tmpItem->setText(tmpStr);
        model->setItem(i, 2, tmpItem);

        tmpItem=new QStandardItem();
        tmpStr="";
        tmpStr+=QString::number(tables.at(i)->getAccurance_y());
        tmpItem->setText(tmpStr);
        model->setItem(i, 3, tmpItem);
    }

    ui->tableView->setModel(model);
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();

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
            revertAll();
            return;
        }
        tables.append(tmp);
    }
/* end of uploading */

    timeCounts=tables.count();

    ui->startButton->setEnabled(true);
    ui->tableView->setVisible(true);
    ui->cancelButton->setEnabled(true);
}

void MainWindow::on_nextButton_pressed()
{
    switch (actualStep)
    {
    case 1:
        errorMessageBox->setText("Unpredicated behavior!");
        errorMessageBox->show();
        revertAll();
        return;

    case 2:
    {
        ui->tableView->model()->removeRows(0, ui->tableView->model()->rowCount());
        ui->tableView->model()->removeColumns(0, ui->tableView->model()->columnCount());

        if(step2Koefs.count()>0)
            step2Koefs.clear();

//        QFile mathLog("/home/alexandr/Desktop/log5");
//        mathLog.open(QIODevice::WriteOnly);
//        QTextStream* logStream=new QTextStream(&mathLog);

/* OLS for polynom, degree 1 */
        OLS_polynom* ols_polynom_1=new OLS_polynom(1);
        for(int i=0; i<tables.count(); i++)
        {
            Koefs* tmpKoefs=new Koefs();
            tmpKoefs->setType(POLYNOM_1);
            tmpKoefs->setKoefs(ols_polynom_1->getSolve(tables.at(i)->getX(), tables.at(i)->getY(), tables.at(i)->getSize()));
            qDebug() << tmpKoefs->getKoef(0) << tmpKoefs->getKoef(1);
            step2Koefs.append(tmpKoefs);
        }
        delete ols_polynom_1;
/* end of OLS for polynom */
//        *logStream << "p_1: \n";
//        for(int i=timeCounts*0; i<timeCounts*1; i++)
//            *logStream << step2Koefs.at(i)->a[0] << " " << step2Koefs.at(i)->a[1] << "\n";

/* OLS for polynom, degree 2 */
        OLS_polynom* ols_polynom_2=new OLS_polynom(2);
        for(int i=0; i<tables.count(); i++)
        {
            Koefs* tmpKoefs=new Koefs();
            tmpKoefs->setType(POLYNOM_2);
            tmpKoefs->setKoefs(ols_polynom_2->getSolve(tables.at(i)->getX(), tables.at(i)->getY(), tables.at(i)->getSize()));
            qDebug() << tmpKoefs->getKoef(0) << tmpKoefs->getKoef(1) << tmpKoefs->getKoef(2);
            step2Koefs.append(tmpKoefs);
        }
        delete ols_polynom_2;
/* end of OLS for polynom */
//        *logStream << "p_2: \n";
//        for(int i=timeCounts*1; i<timeCounts*2; i++)
//            *logStream << step2Koefs.at(i)->a[0] << " " << step2Koefs.at(i)->a[1] << " " << step2Koefs.at(i)->a[2]<< "\n";

/* OLS for polynom, degree 3 */
        OLS_polynom* ols_polynom_3=new OLS_polynom(3);
        for(int i=0; i<tables.count(); i++)
        {
            Koefs* tmpKoefs=new Koefs();
            tmpKoefs->setType(POLYNOM_3);
            tmpKoefs->setKoefs(ols_polynom_3->getSolve(tables.at(i)->getX(), tables.at(i)->getY(), tables.at(i)->getSize()));
            qDebug() << tmpKoefs->getKoef(0) << tmpKoefs->getKoef(1) << tmpKoefs->getKoef(2) << tmpKoefs->getKoef(3);
            step2Koefs.append(tmpKoefs);
        }
        delete ols_polynom_3;
/* end of OLS for polynom */
//        *logStream << "p_3: \n";
//        for(int i=timeCounts*2; i<timeCounts*3; i++)
//            *logStream << step2Koefs.at(i)->a[0] << " " << step2Koefs.at(i)->a[1] << " " << step2Koefs.at(i)->a[2] << " " << step2Koefs.at(i)->a[3] << "\n";

//        mathLog.flush();
//        mathLog.close();
//        delete logStream;

//        for(int j=0; j<FIRST_OLS_FUNC_Q; j++)
//        {
//            int mp=0;
//            for(int i=0; i<tables.count(); i++)
//                mathStep_2(tables.at(i), step2Koefs.at(mp+i));
//            mp+=timeCounts;
//        }
        for(int j=0; j<timeCounts*FIRST_OLS_FUNC_Q; j+=timeCounts)
        {
            for(int i=0; i<timeCounts; i++)
                mathStep_2(tables.at(i), step2Koefs.at(j+i));
        }

/* filling the table */
        QStandardItemModel* model=new QStandardItemModel();

        QStringList horHeader;
        horHeader.append("R^2");
        horHeader.append("Оценка");

        QStringList vertHeader;
        for(int i=0; i<timeCounts; i++)
        {
            vertHeader.append("time #"+QString::number(i+1));
            vertHeader.append("Полиномиальная (степень 1)");
            vertHeader.append("Полиномиальная (степень 2)");
            vertHeader.append("Полиномиальная (степень 3)");
        }

        QList<QStandardItem*> column1, column2;
        for(int i=0; i<timeCounts; i++)
        {
            QStandardItem *tmpItem=new QStandardItem("");
            column1.append(tmpItem);
            column2.append(tmpItem);
            for(int j=0; j<FIRST_OLS_FUNC_Q; j++)
            {
                QStandardItem *tmpItem1=new QStandardItem();
                QStandardItem *tmpItem2;
                double tmpR2=step2Koefs.at(timeCounts*j+i)->getR2();
                QString tmpStr=QString::number(tmpR2);
                tmpItem1->setText(tmpStr);
                column1.append(tmpItem1);

                if(tmpR2>0.9 && tmpR2<=1.0)
                    tmpItem2=new QStandardItem("очень высокая");
                else if(tmpR2>0.7 && tmpR2<=0.9)
                    tmpItem2=new QStandardItem("высокая");
                else if(tmpR2>0.5 && tmpR2<=0.7)
                    tmpItem2=new QStandardItem("заметная");
                else if(tmpR2>0.0 && tmpR2<=0.5)
                    tmpItem2=new QStandardItem("незначительная");
                else
                    tmpItem2=new QStandardItem("error!");
                column2.append(tmpItem2);
            }
        }
        model->appendColumn(column1);
        model->appendColumn(column2);

        model->setHorizontalHeaderLabels(horHeader);
        model->setVerticalHeaderLabels(vertHeader);

        ui->tableView->setModel(model);
        ui->tableView->resizeRowsToContents();
        ui->tableView->resizeColumnsToContents();
/* end of the filling */

        actualStep++;
        return;
    }
    case 3:
    {
        while(step2Koefs.count()>tables.count())
        {
            double tmpAv1(0.0), tmpAv2(0.0);
            for(int i=0; i<timeCounts; i++)
                tmpAv1+=step2Koefs.at(i)->getR2();
            tmpAv1/=timeCounts;
            for(int i=timeCounts; i<timeCounts*2; i++)
                tmpAv2+=step2Koefs.at(i)->getR2();
            tmpAv2/=timeCounts;
            if(tmpAv1>tmpAv2)
                for(int i=0; i<timeCounts; i++)
                    step2Koefs.removeAt(timeCounts);
            else
                for(int i=0; i<timeCounts; i++)
                    step2Koefs.removeAt(0);
        }
        QMessageBox* infMessageBox=new QMessageBox("Вид итоговой функции", "", QMessageBox::Information, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
        switch (step2Koefs.at(0)->getType())
        {
        case POLYNOM_1:
            infMessageBox->setText("Выбрана полиномиальная функция 1ой степени");
            break;
        case POLYNOM_2:
            infMessageBox->setText("Выбрана полиномиальная функция 2ой степени");
            break;
        case POLYNOM_3:
            infMessageBox->setText("Выбрана полиномиальная функция 3ой степени");
            break;
        default:
            infMessageBox->setText("Каак?Каааак?");
            break;
        }
        while(infMessageBox->exec()==QDialog::Accepted)
            ;

        QVector<Plot2D*> plots;
        for(int i=0; i<timeCounts; i++)
        {
            Plot2D* tmpPlot=new Plot2D();
            QString tmpTitle="time #"+QString::number(i+1);
            tmpPlot->setWindowTitle(tmpTitle);
            plots.append(tmpPlot);
            plots.at(i)->getPlotWidget()->addGraph();
            plots.at(i)->getPlotWidget()->graph(0)->setData(tables.at(i)->getX(), tables.at(i)->getYAppr(), tables.at(i)->getSize());
            plots.at(i)->getPlotWidget()->addGraph();
            plots.at(i)->getPlotWidget()->graph(1)->setData(tables.at(i)->getX(), tables.at(i)->getY(), tables.at(i)->getSize());
            QCPScatterStyle tmpStyle(QCPScatterStyle::ssCross, Qt::red, 6.0);
            plots.at(i)->getPlotWidget()->graph(1)->setScatterStyle(tmpStyle);
            QCPGraph::LineStyle dotLineStyle=QCPGraph::lsNone;
            plots.at(i)->getPlotWidget()->graph(1)->setLineStyle(dotLineStyle);
            plots.at(i)->getPlotWidget()->xAxis->setLabel("x");
            plots.at(i)->getPlotWidget()->yAxis->setLabel("y");
            double tmpStep(0.0);
            tmpStep=abs(tables.at(i)->getX(tables.at(i)->getSize()-1)-tables.at(i)->getX(0))/(double)tables.at(i)->getSize();
            plots.at(i)->getPlotWidget()->xAxis->setRange(tables.at(i)->getX(0)-tmpStep*2.0, tables.at(i)->getX(tables.at(i)->getSize()-1)+tmpStep*2.0);
            tmpStep=abs(tables.at(i)->getYAppr(tables.at(i)->getSize()-1)-tables.at(i)->getYAppr(0))/(double)tables.at(i)->getSize();
            plots.at(i)->getPlotWidget()->yAxis->setRange(tables.at(i)->getYAppr(0)-tmpStep*2.0, tables.at(i)->getYAppr(tables.at(i)->getSize()-1)+tmpStep*2.0);

            plots.at(i)->getPlotWidget()->replot();
        }
        for(int i=0; i<timeCounts; i++)
            plots.at(i)->show();

        ui->tableView->setVisible(false);

        actualStep++;
        return;
    }
    case 4:
    {
        ui->tableView->model()->removeRows(0, ui->tableView->model()->rowCount());
        ui->tableView->model()->removeColumns(0, ui->tableView->model()->columnCount());

        double* t=new double[4];
        t[0]=18; t[1]=72; t[2]=168; t[3]=720;
        int koefQ(0);
        switch (step2Koefs.at(0)->getType())
        {
        case POLYNOM_1:
            koefQ=2;
            break;
        case POLYNOM_2:
            koefQ=3;
            break;
        case POLYNOM_3:
            koefQ=4;
            break;
        }

        QVector<double*> aSelection;
        for(int i=0; i<koefQ; i++)
        {
            double* tmpA=new double[timeCounts];
            for(int j=0; j<timeCounts; j++)
                tmpA[j]=step2Koefs.at(j)->getKoef(i);
            aSelection.append(tmpA);
        }

        if(step4Koefs.count()>0)
            step4Koefs.clear();

/* OLS for sin */
        for(int i=0; i<aSelection.count(); i++)
        {
            OLS_sin* ols_sin=new OLS_sin();
            Koefs* tmpKoefs=new Koefs();
            tmpKoefs->setType(tmp_SIN);
            tmpKoefs->setKoefs(ols_sin->getSolve(t, aSelection.at(i), timeCounts));
            step4Koefs.append(tmpKoefs);
        }
/* end of OLS for sin */

/* OLS for cos */
        for(int i=0; i<aSelection.count(); i++)
        {
            OLS_cos* ols_cos=new OLS_cos();
            Koefs* tmpKoefs=new Koefs();
            tmpKoefs->setType(tmp_COS);
            tmpKoefs->setKoefs(ols_cos->getSolve(t, aSelection.at(i), timeCounts));
            step4Koefs.append(tmpKoefs);
        }
/* end of OLS for cos */

        for(int j=0; j<SECOND_OLS_FUNC_Q; j++)
        {
            int mp=0;
            for(int i=0; i<koefQ; i++)
                mathStep_4(t, aSelection.at(i), step4Koefs.at(mp+i), timeCounts);
            mp+=koefQ;
        }
        qDebug() << "this";

/* filling the table */
        QStandardItemModel* model=new QStandardItemModel();

        QStringList horHeader;
        horHeader.append("R^2");
        horHeader.append("Оценка");

        QStringList vertHeader;
//        for(int i=0; i<tables.count(); i++)
//        {
            vertHeader.append("sin (tmp)");
//        }

        model->setHorizontalHeaderLabels(horHeader);
        model->setVerticalHeaderLabels(vertHeader);

        ui->tableView->setModel(model);
        ui->tableView->resizeRowsToContents();
        ui->tableView->resizeColumnsToContents();
/* end of the filling */

        ui->tableView->setVisible(true);

        actualStep++;
        return;
    }
    case 5:
        ui->tableView->setVisible(false);

        actualStep++;
        return;

    default:
        qDebug() << "Next Button: " << "error! step " << actualStep << " is wrong";
        return;
    }
}

void MainWindow::on_cancelButton_pressed()
{
    revertAll();
}

void MainWindow::keyPressEvent(QKeyEvent* ke)
{
    switch (ke->key())
    {
    case Qt::Key_T:
        if(ke->modifiers() & (Qt::MetaModifier | Qt::ControlModifier))
            testFunction();
        break;
    }
}

void MainWindow::testFunction()
{
    QString tmp("time = 0.5 ");
    QStringList tmpList=tmp.split('=');
    qDebug() << tmpList.at(1).toDouble();
}
