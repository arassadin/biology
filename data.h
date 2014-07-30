#ifndef DATA_H
#define DATA_H

#include<QDebug>

class Data
{
public:
    Data();
    ~Data();

private:
    double* x;
    double* y;
    int size;
    double arithemeticMean_x;
    double arithemeticMean_y;
    double standartDeviation_x;
    double standartDeviation_y;
    double standartDeviationOfArithmeticMean_x;
    double standartDeviationOfArithmeticMean_y;
    double confidenceInterval_x;
    double confidenceInterval_y;
    double accurance_x;
    double accurance_y;

public:
    int getSize();
    void setSize(int _size);
    bool allocate();
    void setX(int element, double value);
    void setY(int element, double value);
    double getX(int element);
    double getY(int element);
    double getArithemeticMean_x();
    double getArithemeticMean_y();
    double getStandartDeviation_x();
    double getStandartDeviation_y();
    double getStandartDeviationOfArithmeticMean_x();
    double getStandartDeviationOfArithmeticMean_y();
    double getConfidenceInterval_x();
    double getConfidenceInterval_y();
    double getAccurance_x();
    double getAccurance_y();
    void setArithemeticMean_x(double value);
    void setArithemeticMean_y(double value);
    void setStandartDeviation_x(double value);
    void setStandartDeviation_y(double value);
    void setStandartDeviationOfArithmeticMean_x(double value);
    void setStandartDeviationOfArithmeticMean_y(double value);
    void setConfidenceInterval_x(double value);
    void setConfidenceInterval_y(double value);
    void setAccurance_x(double value);
    void setAccurance_y(double value);
};

#endif // DATA_H
