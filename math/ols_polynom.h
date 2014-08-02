#include<iostream>
#include<QDebug>
#include"qmath.h"

#include"mainElementMethod.h"

class OLS_polynom
{
public:
    OLS_polynom(int);
    double* getSolve(double*, double*, int);
    ~OLS_polynom();
    
private:
    void allocateMatrixes();
    void fillMatrixes();
    void freeMatrixes();
    
private:
    int size;
    int degree;
    double* solve;
    double** constant_terms;
    double** sums;
   	double* x;
    double* y;
};
