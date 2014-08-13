#include<iostream>
#include<QDebug>
#include"qmath.h"
#include"mainElementMethod.h"

#define DEGREE 2

class OLS_cos
{
public:
    OLS_cos();
    double* getSolve(double*, double*, int);
    ~OLS_cos();
    
private:
    void allocateMatrixes();
    void fillMatrixes();
    
private:
    int size;
    double* solve;
    double** constant_terms;
    double** sums;
   	double* x;
    double* y;
};
