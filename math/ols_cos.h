#include<iostream>
#include<QDebug>
#include"qmath.h"

#define DEGREE 2

class OLS_cos
{
public:
    OLS_cos();
    double* getSolve(double*, double*, int);
    ~OLS_cos();
    
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
