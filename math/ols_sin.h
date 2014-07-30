#include<iostream>
#include<QDebug>
#include"qmath.h"

#define DEGREE 2

class OLS_sin
{
public:
    OLS_sin();
    double* getSolve(double*, double*, int);
    ~OLS_sin();
    
private:
    void diagonalChecking();
    void allocateMatrixes();
    void fillMatrixes();
    void freeMatrix();
    
private:
    int size;
    double* solve;
    double* constant_terms;
    double** sums;
   	double* x;
    double* y;
};
