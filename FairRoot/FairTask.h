

#ifndef FAIRTASK_H
#define FAIRTASK_H

#pragma link C++ class FairTask+;
#include "TTask.h"                      // for TTask

#include "Rtypes.h"                     // for Int_t, FairTask::Class, etc
#include "TString.h"                    // for TString

#include <map>

enum InitStatus {kSUCCESS, kERROR, kFATAL};

class FairLogger;

class FairTask : public TTask
{

  public:

    int value;

    FairTask();
    FairTask(const FairTask&);
    FairTask(const char* name, Int_t iVerbose = 1);
    FairTask& operator=(const FairTask&);

    void InitTask();
    void InitTasks();

    virtual ~FairTask();
    virtual void FinishTask();
    virtual void FinishEvent();
    virtual void ExecuteTask(Option_t *option="0");  // *MENU*

    void SetParTask();
    void SetParTasks();
  protected:

    virtual InitStatus Init() { return kSUCCESS; };
    virtual InitStatus ReInit() { return kSUCCESS; };
    virtual void SetParContainers() { };
    virtual void Finish() { };
    virtual void ExecuteTasks(Option_t *option);

    void FinishTasks();
    void FinishEvents();

    FairLogger* fLogger;

};

#endif
