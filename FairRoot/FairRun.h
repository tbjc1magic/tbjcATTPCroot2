#ifndef FAIRRUN_H
#define FAIRRUN_H

#include "TNamed.h"
#include "Rtypes.h"
#include "TMCtls.h"
#include "TFile.h"

class FairRuntimeDb;
class FairTask;
class FairRootManager;

class FairRun : public TNamed
{
  public:

    FairRun(Bool_t isMaster=true);

    virtual ~FairRun();

    static FairRun* Instance();

    virtual void       AddTask(FairTask* t);
    virtual void       SetTask(FairTask* t);

    virtual void    Run(Int_t NStart =0,Int_t NStop=0){};

    FairRuntimeDb* GetRuntimeDb(void) {return fRtdb;}
    void SetRuntimeDb(FairRuntimeDb*db) {fRtdb=db;}
   // FairTask* GetTask(const char* taskName);
    Int_t     GetNTasks() {return fNTasks;}

  private:

    Int_t fNTasks;

  protected:
    static                FairRun* fRunInstance;
    FairRuntimeDb*           fRtdb;
    FairTask*                fTask;
    FairRootManager*         fRootManager;

    TFile* fOutFile;
    const char * fOutname;

};
#endif //FAIRRUN_H
