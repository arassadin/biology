#ifndef KOEFS_H
#define KOEFS_H

#include"defines.h"

class Koefs
{
public:
    Koefs();
    ~Koefs();

    void setR2(double _R2);
    double getR2();
    void setType(functionType _type);
    functionType getType();

public:
    double* a;

private:
    double R2;
    functionType type;
};

#endif // KOEFS_H
