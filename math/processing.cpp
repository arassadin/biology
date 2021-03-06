#include"processing.h"

void mathStep_1(Data *table)
{
    double tmp(0.0);

    for(int i=0; i<table->getSize(); i++)
    {
        tmp+=table->getX(i);
    }
    table->setArithemeticMean_x(tmp/(double)table->getSize());

    tmp=0.0;
    for(int i=0; i<table->getSize(); i++)
    {
        tmp+=table->getY(i);
    }
    table->setArithemeticMean_y(tmp/(double)table->getSize());

    tmp=0.0;
    for(int i=0; i<table->getSize(); i++)
    {
        tmp+=pow(table->getX(i)-table->getArithemeticMean_x(), 2.0);
    }
    table->setStandartDeviation_x(pow(tmp/((double)table->getSize()-1), 0.5));

    tmp=0.0;
    for(int i=0; i<table->getSize(); i++)
    {
        tmp+=pow(table->getY(i)-table->getArithemeticMean_y(), 2.0);
    }
    table->setStandartDeviation_y(pow(tmp/((double)table->getSize()-1), 0.5));

    tmp=table->getStandartDeviation_x()/pow((double)table->getSize(), 0.5);
    table->setStandartDeviationOfArithmeticMean_x(tmp);
    tmp=table->getStandartDeviation_y()/pow((double)table->getSize(), 0.5);
    table->setStandartDeviationOfArithmeticMean_y(tmp);

    tmp=1.0*table->getStandartDeviationOfArithmeticMean_x();
    table->setConfidenceInterval_x(tmp);
    tmp=1.0*table->getStandartDeviationOfArithmeticMean_y();
    table->setConfidenceInterval_y(tmp);

    tmp=table->getStandartDeviationOfArithmeticMean_x()/table->getArithemeticMean_x()*100.0;
    table->setAccurance_x(tmp);
    tmp=table->getStandartDeviationOfArithmeticMean_y()/table->getArithemeticMean_y()*100.0;
    table->setAccurance_y(tmp);

//    qDebug() << table->getArithemeticMean_x() << table->getStandartDeviation_x() << table->getStandartDeviationOfArithmeticMean_x() \
//                << table->getConfidenceInterval_x() << table->getAccurance_x();
//    qDebug() << table->getArithemeticMean_y() << table->getStandartDeviation_y() << table->getStandartDeviationOfArithmeticMean_y() \
//                << table->getConfidenceInterval_y() << table->getAccurance_y();
}

void mathStep_2(Data *table, Koefs* koefs)
{
    double* yAppr=new double[table->getSize()];
    switch (koefs->getType())
    {
    case POLYNOM_3:
    case POLYNOM_2:
    case POLYNOM_1:
        for(int i=0; i<table->getSize(); i++)
        {
            double tmp(0.0);
            for(int k=0; k<koefs->getKoefQ(); k++)
                tmp+=koefs->getKoef(k)*pow(table->getX(i), k);
            yAppr[i]=tmp;
        }
        break;

    default:
        break;
    }

    double tmp1(0.0), tmp2(0.0);
    for(int i=0; i<table->getSize(); i++)
    {
        tmp1+=pow(yAppr[i]-table->getArithemeticMean_y(), 2.0);
        tmp2+=pow(table->getY(i)-table->getArithemeticMean_y(), 2.0);
    }
    koefs->setR2(pow(tmp1/tmp2, 0.5));

//    qDebug() << "R^2 step2: " << koefs->getR2();

    delete[] yAppr;
}

void mathStep_4(double* t, double* a, Koefs* koefs, int count)
{
    double* aAppr=new double[count];
    switch (koefs->getType())
    {
    case POLYNOM_1:
    case POLYNOM_2:
    case POLYNOM_3:
        for(int i=0; i<count; i++)
        {
            double tmp(0.0);
            for(int k=0; k<koefs->getKoefQ(); k++)
                tmp+=koefs->getKoef(k)*pow(t[i], k);
            aAppr[i]=tmp;
        }
        break;

    case tmp_SIN:
        for(int i=0; i<count; i++)
        {
            double tmp=koefs->getKoef(0)+koefs->getKoef(1)*sin(t[i]);
            aAppr[i]=tmp;
        }
        break;

    case tmp_COS:
        for(int i=0; i<count; i++)
        {
            double tmp=koefs->getKoef(0)+koefs->getKoef(1)*cos(t[i]);
            aAppr[i]=tmp;
        }
        break;

    default:
        break;
    }

    double arithemeticMean_a(0.0);
    for(int i=0; i<count; i++)
        arithemeticMean_a+=a[i]/(double)count;

    double tmp1(0.0), tmp2(0.0);
    for(int i=0; i<count; i++)
    {
        tmp1+=pow(aAppr[i]-arithemeticMean_a, 2.0);
        tmp2+=pow(a[i]-arithemeticMean_a, 2.0);
    }
    koefs->setR2(pow(tmp1/tmp2, 0.5));

    delete[] aAppr;
}

