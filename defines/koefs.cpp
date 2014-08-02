#include "koefs.h"

Koefs::Koefs()
{
    R2=0.0;
}

Koefs::~Koefs()
{
    delete[] a;
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
