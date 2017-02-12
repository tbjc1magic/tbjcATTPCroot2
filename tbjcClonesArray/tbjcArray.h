#ifndef HEADER__tbjcArray
#define HEADER__tbjcArray
#include <iostream>
class tbjcArray
{
    public:
        tbjcArray(){};
        virtual ~tbjcArray(){};
        virtual void Clear(std::string)=0;
        virtual TObject* ConstructedAt(int idx)=0;
        virtual TObject* At(int idx)=0;
        virtual int GetLast()=0;
        virtual int GetEntriesFast()=0;
        virtual void Insert(int idx, TObject*obj)=0;

        virtual void Print(){};
};

#endif
