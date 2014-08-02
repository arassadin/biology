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

    qDebug() << table->getArithemeticMean_x() << table->getStandartDeviation_x() << table->getStandartDeviationOfArithmeticMean_x() \
                << table->getConfidenceInterval_x() << table->getAccurance_x();
    qDebug() << table->getArithemeticMean_y() << table->getStandartDeviation_y() << table->getStandartDeviationOfArithmeticMean_y() \
                << table->getConfidenceInterval_y() << table->getAccurance_y();
}

void mathStep_2(Data *table, Koefs* koefs)
{
    switch (koefs->getType())
    {
    case POLYNOM_3:
        for(int i=0; i<table->getSize(); i++)
        {
            double tmp(0.0);
            for(int k=0; k<=3; k++)
                tmp+=koefs->getA(k)*pow(table->getX(i), k);
            table->setYAppr(i, tmp);
        }
        break;

    case POLYNOM_2:
        for(int i=0; i<table->getSize(); i++)
        {
            double tmp(0.0);
            for(int k=0; k<=2; k++)
                tmp+=koefs->getA(k)*pow(table->getX(i), k);
            table->setYAppr(i, tmp);
        }
        break;

    case POLYNOM_1:
        for(int i=0; i<table->getSize(); i++)
        {
            double tmp(0.0);
            for(int k=0; k<=1; k++)
                tmp+=koefs->getA(k)*pow(table->getX(i), k);
            table->setYAppr(i, tmp);
        }
        break;

    default:
        break;
    }

    double tmp1(0.0), tmp2(0.0);
    for(int i=0; i<table->getSize(); i++)
    {
        tmp1+=pow(table->getY(i)-table->getYAppr(i), 2.0);
        tmp2+=pow(table->getY(i)-table->getArithemeticMean_y(), 2.0);
    }
    koefs->setR2(tmp1/tmp2);

    qDebug() << "R^2: " << koefs->getR2();
}

