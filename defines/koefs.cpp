#include "koefs.h"

Koefs::Koefs(f_type _type)
{
    type=_type;
    appr=0;
    R2=0.0;
}

Koefs::~Koefs()
{
    delete[] koefs;
    delete[] appr;
}

void Koefs::setR2(double _R2)
{
    R2=_R2;
}

double Koefs::getR2()
{
    return R2;
}

functionType Koefs::getType()
{
    return type.type;
}

int Koefs::getKoefQ()
{
    return type.koefQ;
}

void Koefs::setKoefs(double* _koefs)
{
    koefs=_koefs;
}

double Koefs::getKoef(int element)
{
    return koefs[element];
}

void Koefs::setAppr(int element, double value)
{
    if(appr==0)
        appr=new double[type.koefQ];
    appr[element]=value;
}

double Koefs::getAppr(int element)
{
    return appr[element];
}
