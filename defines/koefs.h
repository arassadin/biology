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
    double getKoef(int element);
    void setKoefs(double* _koefs);
    void setAppr(int element, double value);
    double getAppr(int element);

private:
    double* koefs;
    double* appr;
    double R2;
    functionType type;
};

#endif // KOEFS_H
