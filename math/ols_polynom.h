#include<iostream>
#include<QDebug>
#include"qmath.h"

class OLS_polynom
{
public:
    OLS_polynom(int);
    double* getSolve(double*, double*, int);
    ~OLS_polynom();
    
private:
    void diagonalChecking();
    void allocateMatrixes();
    void fillMatrixes();
    void freeMatrix();
    
private:
    int size;
    int degree;
    double* solve;
    double* constant_terms;
    double** sums;
   	double* x;
    double* y;
};
