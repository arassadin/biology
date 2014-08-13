#include"ols_cos.h"

OLS_cos::OLS_cos()
{
}

void OLS_cos::allocateMatrixes()
{
    solve=new double[DEGREE];
    constant_terms=new double*[DEGREE];
    for(int i=0; i<DEGREE; i++)
        constant_terms[i]=new double[1];
    sums=new double*[DEGREE];
    for(int i=0; i<DEGREE; i++)
    {
        sums[i]=new double[DEGREE];
        for(int j=0; j<DEGREE; j++)
        {
            sums[i][j]=0.0;
        }
        
        solve[i]=0.0;
        constant_terms[i][0]=0.0;
    }
}

void OLS_cos::fillMatrixes()
{
	sums[0][0]=size;
	for(int k=0; k<size; k++)
		sums[0][1]+=cos(x[k]);
	for(int k=0; k<size; k++)
		sums[1][0]+=cos(x[k]);
	for(int k=0; k<size; k++)
		sums[1][1]+=pow(cos(x[k]), 2.0);

	for(int k=0; k<size; k++)
		constant_terms[0][0]+=y[k];
	for(int k=0; k<size; k++)
		constant_terms[1][0]+=y[k]*cos(x[k]);
}

double *OLS_cos::getSolve(double* _x, double* _y, int _size)
{
    size=_size;
    x=_x; y=_y;

    allocateMatrixes();
    fillMatrixes();

    mainElementMethod* m=new mainElementMethod(sums, constant_terms, DEGREE);
    solve=m->solveSystem();

    delete m;
   
    return solve;
}
