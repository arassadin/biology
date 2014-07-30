#include"ols_sin.h"

OLS_sin::OLS_sin()
{
}

void OLS_sin::freeMatrix()
{
   int i;
   for(i=0; i<DEGREE; i++)
   {
       delete[] sums[i];
   }
   delete[] constant_terms;
   delete[] sums;
}

void OLS_sin::allocateMatrixes()
{
    solve=new double[DEGREE];
    constant_terms=new double[DEGREE];
    sums=new double*[DEGREE];
    for(int i=0; i<DEGREE; i++)
    {
        sums[i]=new double[DEGREE];
        for(int j=0; j<DEGREE; j++)
        {
            sums[i][j]=0.0;
        }
        
        solve[i]=0.0;
        constant_terms[i]=0.0;
    }
}

void OLS_sin::fillMatrixes()
{
	sums[0][0]=size;
	for(int k=0; k<size; k++)
		sums[0][1]+=sin(x[k]);
	for(int k=0; k<size; k++)
		sums[1][0]+=sin(x[k]);
	for(int k=0; k<size; k++)
		sums[1][1]+=pow(sin(x[k]), 2.0);

	for(int k=0; k<size; k++)
		constant_terms[0]+=y[k];
	for(int k=0; k<size; k++)
		constant_terms[1]+=y[k]*sin(x[k]);
}

void OLS_sin::diagonalChecking()
{
   float temp=0;
   for(int i=0; i<DEGREE; i++)
   {
       if(sums[i][i]==0)
       {
           for(int j=0; j<DEGREE; j++)
           {
               if(j==i)
                   continue;
               if(sums[j][i]!=0 && sums[i][j]!=0)
               {
                   for(int k=0; k<DEGREE; k++)
                   {
                       temp = sums[j][k];
                       sums[j][k]=sums[i][k];
                       sums[i][k]=temp;
                   }
                   temp=constant_terms[j];
                   constant_terms[j]=constant_terms[i];
                   constant_terms[i]=temp;
                   break;
               }
           }
       }
   }
}

double *OLS_sin::getSolve(double* _x, double* _y, int _size)
{
    size=_size;
    x=_x; y=_y;

    allocateMatrixes();
    fillMatrixes();

/* Gaussian Method, I'm suppose.. */

    diagonalChecking();

    for(int k=0; k<DEGREE; k++)
    {
       for(int i=k+1; i<DEGREE; i++)
       {
           if(sums[k][k]==0)
           {
               qDebug() << "OLS: " << "Error! Solution is not exists!";
               return NULL;
           }
           float M=sums[i][k]/sums[k][k];
           for(int j=k; j<DEGREE; j++)
           {
               sums[i][j]-=M*sums[k][j];
           }
           constant_terms[i]-=M*constant_terms[k];
       }
    }

   for(int i=DEGREE-1; i>=0; i--)
   {
       double s=0.0;
       for(int j=i; j<DEGREE; j++)
       {
			s=s+sums[i][j]*solve[j];
       }
       solve[i]=(constant_terms[i]-s)/sums[i][i];
   }

/* ------------------------------ */

   freeMatrix();
   
   return solve;
}
