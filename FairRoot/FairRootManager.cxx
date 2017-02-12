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
}
FairRootManager::~FairRootManager()
{}

FairRootManager*FairRootManager::Instance()
{
    return fgInstance;
}

TFile* FairRootManager::OpenOutFile(const char* fname)
{
  LOG(DEBUG) << "Opening output file, " << fname << FairLogger::endl;

  if(fOutFile) {
    CloseOutFile();
  }
  LOG(INFO) << "FairRootManager::OpenOutFile(\"" << fname << "\")" << FairLogger::endl;
  fOutFile = new TFile(fname, "recreate");
  return fOutFile;
}

//TClonesArray* FairRootManager::GetObject(const char* BrName)
tbjcArray* FairRootManager::GetObject(const char* BrName)
{
    TString name(BrName);
    std::map<TString, tbjcArray*>::iterator p;
    p = fMap.find(name);
    if(p!=fMap.end()) return p->second;
}

//void FairRootManager::Register(const char*BrName,const char*foldername,TClonesArray*obj,Bool_t toFile)
void FairRootManager::Register(const char*BrName,const char*foldername,tbjcArray*obj,Bool_t toFile)
{
    TString name(BrName);
    std::map<TString, tbjcArray*>::iterator p;
    p = fMap.find(name);
    if(p==fMap.end()) fMap.insert(std::pair<TString,tbjcArray*>(name, obj));
}

void FairRootManager::SetFinishRun(Bool_t val){}
