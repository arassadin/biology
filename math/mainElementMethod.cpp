#include"mainElementMethod.h"

mainElementMethod::mainElementMethod(double** m, double** c_t, int size)
{
	matrix=m;
	constantTerms=c_t;
	systemSize=size;
}

double* mainElementMethod::getSolve()
{
	double* solve=new double[systemSize];
	for(int k=systemSize-1; k>=0; k--)
	{
		position p=findMax(k, (char)NULL);
		solve[p.j]=constantTerms[p.i][0]/matrix[p.i][p.j];
		for(int i=0; i<k; i++)
		{
			constantTerms[i][0] -= matrix[i][p.j]*solve[p.j];
			matrix[i][p.j]=0.0;
		}
	}
	return solve;
}

double* mainElementMethod::solveSystem()
{
	/*double* tmpRow=new double[systemSize];
	double tmpConstTerm;*/
	
	for(int k=0; k<systemSize-1; k++)
	{
		position p=findMax(k);
		//std::cout << p.i << "\t" << p.j << "\t" << matrix[p.i][p.j] << std::endl << std::endl;
	
		/*for(int j=0; j<systemSize; j++)
			tmpRow[j]=matrix[p.i][j];
		tmpConstTerm=constantTerms[p.i][0];*/
		
		for(int i=k; i<p.i; i++)
		{
			double koef=-matrix[i][p.j]/matrix[p.i][p.j];
			for(int j=0; j<systemSize; j++)
			{
				matrix[i][j]+=matrix[p.i][j]*koef;
			}
			constantTerms[i][0]+=constantTerms[p.i][0]*koef;
			matrix[i][p.j]=0.0;
		}
		for(int i=p.i+1; i<systemSize; i++)
		{
			double koef=-matrix[i][p.j]/matrix[p.i][p.j];
			for(int j=0; j<systemSize; j++)
			{
				matrix[i][j]+=matrix[p.i][j]*koef;
			}
			constantTerms[i][0]+=constantTerms[p.i][0]*koef;
			matrix[i][p.j]=0.0;
		}
		
		for(int j=0; j<systemSize; j++)
		{
			//matrix[p.i][j]=matrix[k][j];
			//matrix[k][j]=tmpRow[j];
			double tmp=matrix[p.i][j];
			matrix[p.i][j]=matrix[k][j];
			matrix[k][j]=tmp;
		}
		//constantTerms[p.i][0]=constantTerms[k][0];
		//constantTerms[k][0]=tmpConstTerm;
		double tmp=constantTerms[p.i][0];
		constantTerms[p.i][0]=constantTerms[k][0];
		constantTerms[k][0]=tmp;
		
		/*for(int i=0; i<systemSize; i++)
		{
			for(int j=0; j<systemSize; j++)
				std::cout << matrix[i][j] << "\t";
			std::cout << constantTerms[i][0];
			std::cout << std::endl;
		}
		std::cout << std::endl;*/
	}
	
	return getSolve();
}

position mainElementMethod::findMax(int _i)
{
	position p;
	p.i=_i; p.j=0;
	for(int i=_i; i<systemSize; i++)
	{
		for(int j=0; j<systemSize; j++)
			if(fabs(matrix[i][j])>matrix[p.i][p.j])
			{
				p.i=i;
				p.j=j;
			}
	}
	return p;
}

position mainElementMethod::findMax(int i, char)
{
	position p;
	p.i=i; p.j=0;
	for(int j=0; j<systemSize; j++)
		if(fabs(matrix[i][j])>matrix[p.i][p.j])
		{
			p.j=j;
		}
	return p;
}

mainElementMethod::~mainElementMethod()
{
	for(int i=0; i<systemSize; i++)
	{
		delete[] matrix[i];
		delete[] constantTerms[i];
	}
	delete[] matrix;
    delete[] constantTerms;
}
