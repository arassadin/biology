#include"ols_polynom.h"

OLS_polynom::OLS_polynom(int _degree)
{
	degree=_degree;
}

OLS_polynom::~OLS_polynom()
{

}

void OLS_polynom::freeMatrix()
{
   int i;
   for(i=0; i<degree+1; i++)
   {
       delete[] sums[i];
   }
   delete[] constant_terms;
   delete[] sums;
}

void OLS_polynom::allocateMatrixes()
{
    solve=new double[degree+1];
    constant_terms=new double[degree+1];
    sums=new double*[degree+1];
    for(int i=0; i<degree+1; i++)
    {
        solve[i]=0.0;
        constant_terms[i]=0.0;
        sums[i]=new double[degree+1];
        for(int j=0; j<degree+1; j++)
        {
            sums[i][j]=0.0;
        }
    }
}

void OLS_polynom::fillMatrixes()
{
   //init square sums matrix
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

   //init free coefficients column
   for(int i=0; i<degree+1; i++)
   {
       for(int k=0; k<size; k++)
       {
            constant_terms[i]+=pow(x[k], i)*y[k];
       }
   }
}

void OLS_polynom::diagonalChecking()
{
   float temp=0;
   for(int i=0; i<degree+1; i++)
   {
       if(sums[i][i]==0)
       {
           for(int j=0; j<degree+1; j++)
           {
               if(j==i)
                   continue;
               if(sums[j][i]!=0 && sums[i][j]!=0)
               {
                   for(int k=0; k<degree+1; k++)
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

double *OLS_polynom::getSolve(double* _x, double* _y, int _size)
{
    size=_size;
    x=_x; y=_y;

    allocateMatrixes();
    fillMatrixes();

/* Gaussian Method, I'm suppose.. */

//check if there are 0 on main diagonal and exchange rows in that case
    diagonalChecking();

//process rows
    for(int k=0; k<degree+1; k++)
    {
       for(int i=k+1; i<degree+1; i++)
       {
           if(sums[k][k]==0)
           {
               qDebug() << "OLS: " << "Error! Solution is not exists!";
               return NULL;
           }
           float M=sums[i][k]/sums[k][k];
           for(int j=k; j<degree+1; j++)
           {
               sums[i][j]-=M*sums[k][j];
           }
           constant_terms[i]-=M*constant_terms[k];
       }
    }

   for(int i=(degree+1)-1; i>=0; i--)
   {
       double s=0;
       for(int j=i; j<degree+1; j++)
       {
			s=s+sums[i][j]*solve[j];
       }
       solve[i]=(constant_terms[i]-s)/sums[i][i];
   }

/* ------------------------------ */

   freeMatrix();
   
   return solve;
}
