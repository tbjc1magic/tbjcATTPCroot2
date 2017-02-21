#ifndef ATPSATASK_H
#define ATPSATASK_H

// FAIRROOT classes
#include "FairTask.h"
class FairLogger;
class ATRawEvent;
class ATDigiPar;
class ATPSA;

// ROOT classes
#include "TClonesArray.h"
class tbjcArray;
class TBranch;

#include "tbjcPSA.h"
class ATPSATask : public FairTask {
  public:
    ATPSATask();
    ~ATPSATask();

    void SetPSAMode(Int_t value = 0);
    void SetPersistence(Bool_t value = kTRUE);
    void SetThreshold(Double_t threshold);
    void SetBackGroundPeakFinder(Bool_t value);
    void SetPeakFinder();
    void SetMaxFinder();
    void SetBaseCorrection(Bool_t value);
    void SetTimeCorrection(Bool_t value);

    virtual InitStatus Init();
    virtual void SetParContainers();
    virtual void Exec(Option_t *opt);

  private:

    void Fill(tbjcArray*);

    TBranch  **tbjcBranch;

    FairLogger *fLogger;

    ATDigiPar *fPar;
    tbjcArray *fRawEventArray;
    tbjcArray *fEventHArray;

    ATPSA *fPSA;
    Int_t fPSAMode;

    Bool_t fIsPersistence;
    Bool_t fIsBGPK;
    Bool_t fIsPeakFinder;
    Bool_t fIsMaxFinder;
    Bool_t fIsBaseCorr;
    Bool_t fIsTimeCorr;

    Double_t fThreshold;

    tbjcHit tbjcHitReg;
    tbjcPSA tbjcPSAReg;

    static int fInternalID;
};

#endif
