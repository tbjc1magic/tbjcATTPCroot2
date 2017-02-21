#ifndef tbjcPSA_H
#define tbjcPSA_H

struct tbjcHit
{
    int EventNum;
    int PadNum;
    int hitNum;
    float x;
    float y;
    float z;
    float charge;
};

struct tbjcPSA
{
    int EventNum;
    float mesh[512];
    float RhoVariance;
    float QEventTot;
};

#endif
