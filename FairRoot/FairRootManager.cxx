#include "FairLogger.h"
#include "FairRootManager.h"
#include "TClonesArray.h"
#include "tbjcArray.h"
#include "tbjcClonesArray.h"
#include <iostream>
FairRootManager* FairRootManager:: fgInstance=0;

FairRootManager::FairRootManager()
{
    fOutFile = NULL;
    fgInstance = this;

    fOutFile = new TFile("output.root","RECREATE");
    fOutTree = new TTree("tree","tree");

}
FairRootManager::~FairRootManager()
{
    fOutTree->Write();
    delete fOutTree;

    if(fOutFile)
    {
        fOutFile->Close();
        delete fOutFile;
    }
}

FairRootManager*FairRootManager::Instance()
{
    return fgInstance;
}

tbjcArray* FairRootManager::GetObject(const char* BrName)
{
    TString name(BrName);
    std::map<TString, tbjcArray*>::iterator p;
    p = fMap.find(name);
    if(p!=fMap.end()) return p->second;
}

void FairRootManager::Register(const char*BrName,const char*foldername,tbjcArray*obj,Bool_t toFile)
{
    TString name(BrName);
    std::map<TString, tbjcArray*>::iterator p;
    p = fMap.find(name);
    if(p==fMap.end()) fMap.insert(std::pair<TString,tbjcArray*>(name, obj));
}
