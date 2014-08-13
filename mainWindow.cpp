#include "mainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timesCount=0;
    t=0;
    ui->tableView->setVisible(false);
    actualStep=1;
    errorMessageBox=new QMessageBox("An error occured", "Error", QMessageBox::Critical, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
}

MainWindow::~MainWindow()
{
    if(t!=0)
        delete[] t;
    delete ui;
}

void MainWindow::revertAll()
{
    delete[] t; t=0;
    timesCount=0;
    actualStep=1;
    tables.clear();
    step2Koefs.clear();
    step4Koefs.clear();
    appr.clear();

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
    Data* tmpData=new Data();
    QString tmpStr=f.readLine();
    QStringList tmpList=tmpStr.split('=');
    tmpData->setT(tmpList.at(1).toDouble());
    tmpStr=f.readLine();
    int fieldsCount=tmpStr.toInt();
//    qDebug() << fieldsCount;
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
//            qDebug() << tmpData->getX(i) << " " << tmpData->getY(i);
        }
    }
    return tmpData;
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
//        qDebug() << dataList.at(i) << ":";
        Data* tmp=readData(dataList.at(i));
        if(tmp==(Data*)0)
        {
            revertAll();
            return;
        }
        tables.append(tmp);
    }
/* end of uploading */

    t=new double[tables.count()];
    for(int i=0; i<tables.count(); i++)
        t[i]=tables.at(i)->getT();
    timesCount=tables.count();

    ui->startButton->setEnabled(true);
    ui->tableView->setVisible(true);
    ui->cancelButton->setEnabled(true);
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
    for(int i=0; i<timesCount; i++)
    {
        QString tmpStr="time "+QString::number(t[i])+"h";
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

        appr.clear();
        for(int j=0; j<timesCount*FIRST_OLS_FUNC_Q; j+=timesCount)
        {
            for(int i=0; i<timesCount; i++)
                appr.append(mathStep_2(tables.at(i), step2Koefs.at(j+i)));
        }

/* filling the table */
        QStandardItemModel* model=new QStandardItemModel();

        QStringList horHeader;
        horHeader.append("R^2");
        horHeader.append("Оценка");

        QStringList vertHeader;
        for(int i=0; i<timesCount; i++)
        {
            vertHeader.append("time "+QString::number(t[i])+"h");
            vertHeader.append("Полиномиальная (степень 1)");
            vertHeader.append("Полиномиальная (степень 2)");
            vertHeader.append("Полиномиальная (степень 3)");
        }

        QList<QStandardItem*> column1, column2;
        for(int i=0; i<timesCount; i++)
        {
            QStandardItem *tmpItem=new QStandardItem("");
            column1.append(tmpItem);
            column2.append(tmpItem);
            for(int j=0; j<FIRST_OLS_FUNC_Q; j++)
            {
                QStandardItem *tmpItem1=new QStandardItem();
                QStandardItem *tmpItem2;
                double tmpR2=step2Koefs.at(timesCount*j+i)->getR2();
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
        while(step2Koefs.count()>timesCount)
        {
            double tmpAv1(0.0), tmpAv2(0.0);
            for(int i=0; i<timesCount; i++)
                tmpAv1+=step2Koefs.at(i)->getR2();
            tmpAv1/=(double)timesCount;
            for(int i=timesCount; i<timesCount*2; i++)
                tmpAv2+=step2Koefs.at(i)->getR2();
            tmpAv2/=timesCount;
            if(tmpAv1>tmpAv2)
                for(int i=0; i<timesCount; i++)
                {
                    step2Koefs.removeAt(timesCount);
                    appr.removeAt(timesCount);
                }
            else
                for(int i=0; i<timesCount; i++)
                {
                    step2Koefs.removeAt(0);
                    appr.removeAt(0);
                }
        }
        for(int i=0; i<timesCount; i++)
            tables.at(i)->setYAppr(appr.at(i));
        appr.clear();

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
        for(int i=0; i<timesCount; i++)
        {
            Plot2D* tmpPlot=new Plot2D();
            QString tmpTitle="time "+QString::number(t[i])+"h";
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
            tmpStep=fabs(tables.at(i)->getX(tables.at(i)->getSize()-1)-tables.at(i)->getX(0))/(double)tables.at(i)->getSize();
            plots.at(i)->getPlotWidget()->xAxis->setRange(tables.at(i)->getX(0)-tmpStep*1.0, tables.at(i)->getX(tables.at(i)->getSize()-1)+tmpStep*1.0);
//            plots.at(i)->getPlotWidget()->xAxis->setRange(tables.at(i)->getX(0)-1, tables.at(i)->getX(tables.at(i)->getSize()-1)+1);
            tmpStep=fabs(tables.at(i)->getYAppr(tables.at(i)->getSize()-1)-tables.at(i)->getYAppr(0))/(double)tables.at(i)->getSize();
            plots.at(i)->getPlotWidget()->yAxis->setRange(tables.at(i)->getYAppr(0)-tmpStep*1.0, tables.at(i)->getYAppr(tables.at(i)->getSize()-1)+tmpStep*1.0);
//            plots.at(i)->getPlotWidget()->yAxis->setRange(tables.at(i)->getYAppr(0)-1, tables.at(i)->getYAppr(tables.at(i)->getSize()-1)+1);

            plots.at(i)->getPlotWidget()->replot();
        }
        for(int i=0; i<timesCount; i++)
            plots.at(i)->show();

        ui->tableView->setVisible(false);
//        ui->tableView->model()->removeRows(0, ui->tableView->model()->rowCount());
//        ui->tableView->model()->removeColumns(0, ui->tableView->model()->columnCount());

        actualStep++;
        return;
    }
    case 4:
    {
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
        default:
            break;
        }

        QVector<double*> aSelection;
        for(int i=0; i<koefQ; i++)
        {
            double* tmpA=new double[timesCount];
            for(int j=0; j<timesCount; j++)
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
            tmpKoefs->setKoefs(ols_sin->getSolve(t, aSelection.at(i), timesCount));
            step4Koefs.append(tmpKoefs);
        }
/* end of OLS for sin */

/* OLS for cos */
        for(int i=0; i<aSelection.count(); i++)
        {
            OLS_cos* ols_cos=new OLS_cos();
            Koefs* tmpKoefs=new Koefs();
            tmpKoefs->setType(tmp_COS);
            tmpKoefs->setKoefs(ols_cos->getSolve(t, aSelection.at(i), timesCount));
            step4Koefs.append(tmpKoefs);
        }
/* end of OLS for cos */

        appr.clear();
        for(int j=0; j<koefQ*SECOND_OLS_FUNC_Q; j+=koefQ)
        {
            for(int i=0; i<koefQ; i++)
                appr.append(mathStep_4(t, aSelection.at(i), step4Koefs.at(j+i), timesCount));
        }
        qDebug() << "this";

/* filling the table */
        QStandardItemModel* model=new QStandardItemModel();

        QStringList horHeader;
        horHeader.append("R^2");
        horHeader.append("Оценка");

        QStringList vertHeader;
        for(int i=0; i<2; i++)
        {
            vertHeader.append("a "+QString::number(i));
            vertHeader.append("sin");
            vertHeader.append("cos");
        }

        QList<QStandardItem*> column1, column2;
        for(int i=0; i<2; i++)
        {
            QStandardItem *tmpItem=new QStandardItem("");
            column1.append(tmpItem);
            column2.append(tmpItem);
            for(int j=0; j<SECOND_OLS_FUNC_Q; j++)
            {
                QStandardItem *tmpItem1=new QStandardItem();
                QStandardItem *tmpItem2;
                double tmpR2=step4Koefs.at(2*j+i)->getR2();
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

        ui->tableView->setModel(model);
        model->setHorizontalHeaderLabels(horHeader);
        model->setVerticalHeaderLabels(vertHeader);
/* end of the filling */

        ui->tableView->setVisible(true);

        actualStep++;
        return;
    }
    case 5:
    {
        while(step4Koefs.count()>2)
        {
            double tmpAv1(0.0), tmpAv2(0.0);
            for(int i=0; i<2; i++)
                tmpAv1+=step4Koefs.at(i)->getR2();
            tmpAv1/=2.0;
            for(int i=2; i<2*2; i++)
                tmpAv2+=step4Koefs.at(i)->getR2();
            tmpAv2/=2.0;
            if(tmpAv1>tmpAv2)
                for(int i=0; i<2; i++)
                {
                    step4Koefs.removeAt(2);
                    appr.removeAt(2);
                }
            else
                for(int i=0; i<2; i++)
                {
                    step4Koefs.removeAt(0);
                    appr.removeAt(0);
                }
        }

        QMessageBox* infMessageBox=new QMessageBox("Вид итоговой функции", "", QMessageBox::Information, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
        switch (step4Koefs.at(0)->getType())
        {
        case tmp_SIN:
            infMessageBox->setText("Выбран sin");
            break;
        case tmp_COS:
            infMessageBox->setText("Выбран cos");
            break;
        default:
            infMessageBox->setText("Каак?Каааак?");
            break;
        }
        while(infMessageBox->exec()==QDialog::Accepted)
            ;

        QVector<double*> aSelection;
        for(int i=0; i<2; i++)
        {
            double* tmpA=new double[timesCount];
            for(int j=0; j<timesCount; j++)
                tmpA[j]=step2Koefs.at(j)->getKoef(i);
            aSelection.append(tmpA);
        }

        QVector<Plot2D*> plots;
        for(int i=0; i<2; i++)
        {
            Plot2D* tmpPlot=new Plot2D();
            QString tmpTitle="a"+QString::number(i);
            tmpPlot->setWindowTitle(tmpTitle);
            plots.append(tmpPlot);
            plots.at(i)->getPlotWidget()->addGraph();
            plots.at(i)->getPlotWidget()->graph(0)->setData(t, appr.at(i), timesCount);
            plots.at(i)->getPlotWidget()->addGraph();
            plots.at(i)->getPlotWidget()->graph(1)->setData(t, aSelection.at(i), timesCount);
            QCPScatterStyle tmpStyle(QCPScatterStyle::ssCross, Qt::red, 6.0);
            plots.at(i)->getPlotWidget()->graph(1)->setScatterStyle(tmpStyle);
            QCPGraph::LineStyle dotLineStyle=QCPGraph::lsNone;
            plots.at(i)->getPlotWidget()->graph(1)->setLineStyle(dotLineStyle);
            plots.at(i)->getPlotWidget()->xAxis->setLabel("t");
            plots.at(i)->getPlotWidget()->yAxis->setLabel("a"+QString::number(i));
            double tmpStep(0.0);
            tmpStep=fabs(t[timesCount-1]-t[0])/(double)timesCount;
            plots.at(i)->getPlotWidget()->xAxis->setRange(t[0]-tmpStep*1.0, t[timesCount-1]+tmpStep*1.0);
            tmpStep=fabs(aSelection.at(i)[timesCount-1]-aSelection.at(i)[0])/(double)timesCount;
            plots.at(i)->getPlotWidget()->yAxis->setRange(aSelection.at(i)[0]-tmpStep*1.0, aSelection.at(i)[timesCount-1]+tmpStep*1.0);

            plots.at(i)->getPlotWidget()->replot();
        }
        for(int i=0; i<2; i++)
            plots.at(i)->show();

        ui->tableView->setVisible(false);

        ui->nextButton->setEnabled(false);
        actualStep++;
        return;
    }
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
