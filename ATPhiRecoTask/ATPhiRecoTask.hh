#ifndef ATPHIRECOTASK_H
#define ATPHIRECOTASK_H

// FAIRROOT classes
#include "FairTask.h"
#include "FairLogger.h"

// ATTPCTROOT classes
class ATEvent;
class ATProtoEvent;
class ATDigiPar;
class ATPhiReco;
class ATPhiRecoSimple;
class ATPhiRecoTriple;

// ROOT classes
#include "TClonesArray.h"
class tbjcArray;
class ATPhiRecoTask : public FairTask {
  public:
    ATPhiRecoTask();
    ~ATPhiRecoTask();

    void SetPersistence(Bool_t value = kTRUE);
    void SetThreshold(Double_t threshold);
    void SetPhiRecoMode(Int_t value = 0);

    virtual InitStatus Init();
    virtual void SetParContainers();
    virtual void Exec(Option_t *opt);

  private:
    void test(tbjcArray*);
    FairLogger *fLogger;

    ATDigiPar *fPar;

    tbjcArray *fEventHArray;
    tbjcArray *fPEventArray;

    ATPhiReco *fPhiReco;
    Int_t fPhiRecoMode;

    Bool_t fIsPersistence;

    Double_t fThreshold;

};

#endif
