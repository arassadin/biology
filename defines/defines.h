#ifndef DEFINES_H
#define DEFINES_H

enum functionType
{
    notRealType=0,
    POLYNOM_1,
    POLYNOM_2,
    POLYNOM_3,
    SIN_1,
    SIN_2,
    COS_1,
    COS_2,
    tmp_SIN,
    tmp_COS
};

static struct f_type
{
    f_type()
    {
        type=notRealType;
        koefQ=0;
    }
    f_type(int count, functionType _type)
    {
        type=_type;
        koefQ=count;
    }
    functionType type;
    int koefQ;
} polynom_1(2, POLYNOM_1), polynom_2(3, POLYNOM_2), polynom_3(4, POLYNOM_3), tmp_sin(2, tmp_SIN), tmp_cos(2, tmp_COS);

#endif // DEFINES_H
