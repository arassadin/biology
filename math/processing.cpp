#include"processing.h"

void math_step_1(Data *table)
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

    tmp=table->getStandartDeviationOfArithmeticMean_x()/1.0*100.0;
    table->setAccurance_x(tmp);
    tmp=table->getStandartDeviationOfArithmeticMean_y()/1.0*100.0;
    table->setAccurance_y(tmp);
}


