#include "data.h"

Data::Data()
{
    size=0;
    x=0;
    y=0;
}

Data::~Data()
{
    if(x!=0)
        delete[] x;
    if(y!=0)
        delete[] y;
    if(yAppr!=0)
        delete[] yAppr;
}

double Data::getT()
{
    return t;
}

void Data::setT(double value)
{
    t = value;
}

void Data::setSize(int _size)
{
    size=_size;
}

int Data::getSize()
{
    return size;
}

bool Data::allocate()
{
    if(size<1)
        return false;
    x=new double[size];
    y=new double[size];
    yAppr=new double[size];
    qDebug() << x << "_" << y << "_" << yAppr;
    if(x!=0 && y!=0 && yAppr!=0)
        return true;
    return false;
}

void Data::setX(int element, double value)
{
    x[element]=value;
}

void Data::setY(int element, double value)
{
    y[element]=value;
}

void Data::setYAppr(double *value)
{
    yAppr=value;
}

double Data::getX(int element)
{
    return x[element];
}

double Data::getY(int element)
{
    return y[element];
}

double Data::getYAppr(int element)
{
    return yAppr[element];
}

double* Data::getX()
{
    return x;
}

double* Data::getY()
{
    return y;
}

double* Data::getYAppr()
{
    return yAppr;
}

double Data::getAccurance_x()
{
    return accurance_x;
}

double Data::getAccurance_y()
{
    return accurance_y;
}

double Data::getArithemeticMean_x()
{
    return arithemeticMean_x;
}

double Data::getArithemeticMean_y()
{
    return arithemeticMean_y;
}

double Data::getConfidenceInterval_x()
{
    return confidenceInterval_x;
}

double Data::getConfidenceInterval_y()
{
    return confidenceInterval_y;
}

double Data::getStandartDeviationOfArithmeticMean_x()
{
    return standartDeviationOfArithmeticMean_x;
}

double Data::getStandartDeviationOfArithmeticMean_y()
{
    return standartDeviationOfArithmeticMean_y;
}

double Data::getStandartDeviation_x()
{
    return standartDeviation_x;
}

double Data::getStandartDeviation_y()
{
    return standartDeviation_y;
}

void Data::setAccurance_x(double value)
{
    accurance_x=value;
}

void Data::setAccurance_y(double value)
{
    accurance_y=value;
}

void Data::setArithemeticMean_x(double value)
{
    arithemeticMean_x=value;
}

void Data::setArithemeticMean_y(double value)
{
    arithemeticMean_y=value;
}

void Data::setConfidenceInterval_x(double value)
{
    confidenceInterval_x=value;
}

void Data::setConfidenceInterval_y(double value)
{
    confidenceInterval_y=value;
}

void Data::setStandartDeviationOfArithmeticMean_x(double value)
{
    standartDeviationOfArithmeticMean_x=value;
}

void Data::setStandartDeviationOfArithmeticMean_y(double value)
{
    standartDeviationOfArithmeticMean_y=value;
}

void Data::setStandartDeviation_x(double value)
{
    standartDeviation_x=value;
}

void Data::setStandartDeviation_y(double value)
{
    standartDeviation_y=value;
}
