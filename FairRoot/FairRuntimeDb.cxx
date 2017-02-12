#include "FairParSet.h"
#include "FairRuntimeDb.h"

FairRuntimeDb* FairRuntimeDb::gRtdb=0;

FairRuntimeDb::FairRuntimeDb(){gRtdb = this;}
FairRuntimeDb::~FairRuntimeDb(){}
FairRuntimeDb* FairRuntimeDb::instance(){return gRtdb;}

Bool_t FairRuntimeDb::addContainer(char*BrName,FairParSet*fPar)
{
    TString name(BrName);
    std::map<TString, FairParSet*>::iterator p;
    p = fMap.find(name);
    if(p==fMap.end()) fMap.insert(std::pair<TString,FairParSet*>(name, fPar));
    return 1;
}

FairParSet* FairRuntimeDb::getContainer(const char*BrName)
{
    TString name(BrName);
    std::map<TString, FairParSet*>::iterator p;
    p = fMap.find(name);
    if(p!=fMap.end()) return p->second;
}
