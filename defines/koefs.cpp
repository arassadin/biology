#include "koefs.h"

Koefs::Koefs()
{
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

void Koefs::setType(functionType _type)
{
    type=_type;
}

functionType Koefs::getType()
{
    return type;
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
        switch (type)
        {
        case tmp_SIN:
            appr=new double[2];
            break;
        case tmp_COS:
            appr=new double[2];
            break;
        default:
            break;
        }
    appr[element]=value;
}

double Koefs::getAppr(int element)
{
    return appr[element];
}
