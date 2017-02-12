#ifndef HEADER__tbjcClonesArray
#define HEADER__tbjcClonesArray
#include <vector>
#include "TObject.h"
#include <string>
#include <iostream>
#include "tbjcArray.h"
using namespace std;
template<class T>
class tbjcClonesArray: public tbjcArray
{
    public:

        tbjcClonesArray(int s);

        ~tbjcClonesArray();
        void Clear(string);
        TObject* ConstructedAt(int idx);
        TObject* At(int idx);
        int GetLast();
        int GetEntriesFast();
        void Insert(int idx, TObject*obj);
        void Print();
    private:
        vector<TObject*> fCont;
};

    template<class T>
int tbjcClonesArray<T>::GetLast()
{
    for(int i=fCont.size()-1;i>0; i--)
        if(fCont[i]) return i;
    return -1;
}
    template<class T>
void tbjcClonesArray<T>::Insert(int idx, TObject*obj)
{
    if(idx>=fCont.size())
        fCont.resize(idx+1,NULL);

    TObject*obj_old = fCont[idx];
    if(obj_old) delete obj_old;

    fCont[idx] = obj;
}

    template<class T>
int tbjcClonesArray<T>::GetEntriesFast() {return GetLast()+1;}

    template<class T>
TObject* tbjcClonesArray<T>::At(int idx)
{
    if(idx>=fCont.size())return 0;
    return fCont[idx];
}

    template<class T>
tbjcClonesArray<T>::tbjcClonesArray(int s)
{
    fCont.resize(s,NULL);
}
    template<class T>
tbjcClonesArray<T>::~tbjcClonesArray()
{
    for(auto one :fCont)
        if(one)delete one;

   // fCont.erase(fCont.begin(), fCont.end());
}
    template<class T>
void tbjcClonesArray<T>::Clear(string)
{
    for(auto one :fCont)
        if(one)delete one;

    fCont.erase(fCont.begin(), fCont.end());
}

    template<class T>
TObject*tbjcClonesArray<T>:: ConstructedAt(int idx)
{
    if(idx>=fCont.size())
        fCont.resize(idx+1,NULL);

    TObject* obj = fCont[idx];
    if(obj==NULL)
    {
        obj= static_cast<TObject*>(new T());
        fCont[idx] = obj;
    }
    return obj;
}

    template<class T>
void tbjcClonesArray<T>::Print()
{

    for(auto one:fCont)
    {
        if(one) static_cast<T*>(one)->Print();

    }
}

#endif
