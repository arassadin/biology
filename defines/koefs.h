#ifndef KOEFS_H
#define KOEFS_H

#include"defines/defines.h"

class Koefs
{
public:
    Koefs();
    ~Koefs();

public:
    void setR2(double _R2);
    double getR2();
    void setType(functionType _type);
    functionType getType();
    double getA(int element);
    void setA(double* _a);

private:
    double* a;
    double R2;
    functionType type;
};

#endif // KOEFS_H
