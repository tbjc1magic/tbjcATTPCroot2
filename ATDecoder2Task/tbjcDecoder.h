#ifndef tbjcDecoder_H
#define tbjcDecoder_H

struct tbjcPad
{
    int EventNum;
    int PadNum;
    float PadX;
    float PadY;
    int IsValid;
    int RawAdc[512];

    int MaxAdcIdx;
    int IsPedestalSubtracted;
    float Adc[512];
    int IsAux;

};

#endif
