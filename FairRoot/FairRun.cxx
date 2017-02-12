#include "FairRun.h"
#include "FairRootManager.h"
#include "FairRuntimeDb.h"
#include "FairTask.h"
#include <iostream>
using namespace std;
FairRun* FairRun::fRunInstance=0;

FairRun::FairRun(Bool_t isMaster)
{
    fRootManager = new FairRootManager();
    fRtdb = new FairRuntimeDb();
    fTask = new FairTask("FairTaskList");
    fNTasks=0;
    fRunInstance=this;
}
FairRun::~FairRun(){}
FairRun*  FairRun::Instance(){return fRunInstance;}

void FairRun::AddTask(FairTask* t)
{
    fTask->Add(t);
    fNTasks++;
}

void FairRun::SetTask(FairTask* t)
{
    if ( fTask ) { delete fTask; }
    fTask = t;
}

void FairRun::SetOutputFile(const char* fname)
{
    fOutname=fname;
    if (fRootManager) fOutFile = fRootManager->OpenOutFile(fOutname);
}
