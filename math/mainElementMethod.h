#include<iostream>
#include<math.h>

struct position
{
	int i;
	int j;
};

class mainElementMethod
{
public:
	mainElementMethod(double**, double**, int);
	double* solveSystem();
	double* getSolve();
	~mainElementMethod();
	
private:
	int systemSize;
	double** matrix;
	double** constantTerms;
	position p;
	
private:
	position findMax(int);
	position findMax(int, char);
};
