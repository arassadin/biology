#include"ols_polynom.h"

OLS_polynom::OLS_polynom(int _degree)
{
	degree=_degree;
}

OLS_polynom::~OLS_polynom()
{

}

void OLS_polynom::allocateMatrixes()
{
    solve=new double[degree+1];
    constant_terms=new double*[degree+1];
    for(int i=0; i<degree+1; i++)
        constant_terms[i]=new double[1];
    sums=new double*[degree+1];
    for(int i=0; i<degree+1; i++)
    {
        solve[i]=0.0;
        constant_terms[i][0]=0.0;
        sums[i]=new double[degree+1];
        for(int j=0; j<degree+1; j++)
        {
            sums[i][j]=0.0;
        }
    }
}

void OLS_polynom::fillMatrixes()
{
   for(int i=0; i<degree+1; i++)
   {
       for(int j=0; j<degree+1; j++)
       {
           for(int k=0; k<size; k++)
           {
				sums[i][j]+=pow(x[k], i+j);
           }
       }
   }

   for(int i=0; i<degree+1; i++)
   {
       for(int k=0; k<size; k++)
       {
            constant_terms[i][0]+=pow(x[k], i)*y[k];
       }
   }
}

double *OLS_polynom::getSolve(double* _x, double* _y, int _size)
{
   size=_size;
   x=_x; y=_y;

   allocateMatrixes();
   fillMatrixes();

   mainElementMethod* m=new mainElementMethod(sums, constant_terms, degree+1);
   solve=m->solveSystem();

   delete m;
   
   return solve;
}
