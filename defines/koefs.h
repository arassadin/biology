#ifndef KOEFS_H
#define KOEFS_H

#include"defines/defines.h"

class Koefs
{
public:
    Koefs(f_type _type);
    ~Koefs();

public:
    void setR2(double _R2);
    double getR2();
    functionType getType();
    int getKoefQ();
    double getKoef(int element);
    void setKoefs(double* _koefs);
    void setAppr(int element, double value);
    double getAppr(int element);

private:
    double* koefs;
    double* appr;
    double R2;
    f_type type;
};

#endif // KOEFS_H
