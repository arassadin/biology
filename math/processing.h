#ifndef PROCESSING_H
#define PROCESSING_H

#include<qmath.h>

#include"defines/data.h"
#include"defines/koefs.h"

void mathStep_1(Data* table);
double *mathStep_2(Data* table, Koefs* koefs);
double *mathStep_4(double *t, double *a, Koefs* koefs, int count);

#endif // PROCESSING_H
