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
    std::cout<<"FairRoot Write"<<std::endl;
    fOutFile->cd();
    fOutTree->Write();
   // fOutTree->Print();
    delete fOutTree;
    fOutFile->Save();
    fOutFile->Close();
    std::cout<<"FairRoot Write Finished"<<std::endl;
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
