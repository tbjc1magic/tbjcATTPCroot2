#include "ATPSATask.hh"
#include "ATPSA.hh"
#include "ATPSASimple.hh"
#include "ATPSASimple2.hh"
#include "ATPSAProto.hh"
#include "FairLogger.h"
#include "ATHit.hh"
#include "ATEvent.hh"

// ATTPCROOT classes
#include "ATRawEvent.hh"
#include "ATDigiPar.hh"
#include "ATPSA.hh"

// FAIRROOT classes
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"

#include <iostream>
#ifdef _OPENMP
#include <omp.h>
#endif

#include "tbjcClonesArray.h"
#include "tbjcArray.h"

#define tbjcPSABranch 2

ATPSATask::ATPSATask():FairTask("ATPSATask")
{
    fLogger = FairLogger::GetLogger();
    fPar = NULL;

    fIsPersistence = kFALSE;
    fIsBGPK = kFALSE;
    fIsPeakFinder = kFALSE;
    fIsMaxFinder = kFALSE;
    fIsBaseCorr = kFALSE;
    fIsTimeCorr = kFALSE;

    fEventHArray = new tbjcClonesArray<ATEvent>(10);

    fPSAMode = 2;

    tbjcBranch = new TBranch*[tbjcPSABranch];

}

ATPSATask::~ATPSATask()
{
    std::cout<<"end is hereATPSATask"<<std::endl;

    delete tbjcBranch;
    delete fEventHArray;
}

void ATPSATask::SetPSAMode(Int_t value)               { fPSAMode = value; }
void ATPSATask::SetPersistence(Bool_t value)          { fIsPersistence = value; }
void ATPSATask::SetThreshold(Double_t threshold)      { fThreshold = threshold; }
void ATPSATask::SetBackGroundPeakFinder(Bool_t value) { fIsBGPK = value;}
void ATPSATask::SetPeakFinder()                       { fIsPeakFinder= kTRUE;fIsMaxFinder= kFALSE;}
void ATPSATask::SetMaxFinder()                        { fIsMaxFinder= kTRUE;fIsPeakFinder= kFALSE;}
void ATPSATask::SetBaseCorrection(Bool_t value)       { fIsBaseCorr = value;}
void ATPSATask::SetTimeCorrection(Bool_t value)       { fIsTimeCorr = value;}

    InitStatus
ATPSATask::Init()
{
    FairRootManager *ioMan = FairRootManager::Instance();
    if (ioMan == 0) {
        fLogger -> Error(MESSAGE_ORIGIN, "Cannot find RootManager!");
        return kERROR;
    }

    TTree* fOutTree = ioMan->GetOutTree();

  tbjcBranch[0] = fOutTree->Branch("PSA",&tbjcPSAReg,"EventNum/i:mesh[512]/float:RhoVariance/f:QEventTot/f");
  tbjcBranch[1] = fOutTree->Branch("Hit",&tbjcHitReg,"EventNum/i:PadNum/i:hitNum/i:x/f:y/f:z/f:charge/f");

    fRawEventArray = (tbjcArray *) ioMan -> GetObject("ATRawEvent");
    if (fRawEventArray == 0) {
        fLogger -> Error(MESSAGE_ORIGIN, "Cannot find ATRawEvent array!");
        return kERROR;
    }

    if (fPSAMode == 0) {
        fLogger -> Info(MESSAGE_ORIGIN, "Using ATPSASimple!");

        fPSA = new ATPSASimple();
    } else if (fPSAMode == 1) {
        fLogger -> Info(MESSAGE_ORIGIN, "Using ATPSASimple2!");

        fPSA = new ATPSASimple2();

    } else if (fPSAMode == 2) {
        fLogger -> Info(MESSAGE_ORIGIN, "Using ATPSAProto!");
        fPSA = new ATPSAProto();
    }

    fPSA -> SetThreshold((Int_t)fThreshold);
    fPSA -> SetBaseCorrection(fIsBaseCorr);
    fPSA -> SetTimeCorrection(fIsTimeCorr);

    if(fIsBGPK){
        fLogger -> Info(MESSAGE_ORIGIN, "Suppression of background in Peak Finder Enabled");
        fPSA -> SetBackGroundSuppression();
    }

    if(fIsPeakFinder){
        fLogger -> Info(MESSAGE_ORIGIN, " Peak Finder enabled for hit pattern reconstruction");
        fPSA -> SetPeakFinder();
    }else if(fIsMaxFinder){
        fLogger -> Info(MESSAGE_ORIGIN, " Maximum Finder enabled for hit pattern reconstruction");
        fPSA -> SetMaxFinder();
    }else if(!fIsMaxFinder && !fIsPeakFinder){
        fLogger -> Error(MESSAGE_ORIGIN, " Please select a method for hit pattern reconstruction");
        return kERROR;
    }

    ioMan -> Register("ATEventH", "ATTPC", fEventHArray, fIsPersistence);

    return kSUCCESS;
}

    void
ATPSATask::SetParContainers()
{
    FairRun *run = FairRun::Instance();
    if (!run)
        fLogger -> Fatal(MESSAGE_ORIGIN, "No analysis run!");

    FairRuntimeDb *db = run -> GetRuntimeDb();
    if (!db)
        fLogger -> Fatal(MESSAGE_ORIGIN, "No runtime database!");

    fPar = (ATDigiPar *) db -> getContainer("ATDigiPar");
    if (!fPar)
        fLogger -> Fatal(MESSAGE_ORIGIN, "ATDigiPar not found!!");
}

void  ATPSATask::Fill(tbjcArray* fEventHArray)
{
    ATEvent* event = static_cast<ATEvent*>(fEventHArray->At(0));

    tbjcPSAReg.EventNum = fInternalID;
    Float_t *_mesh = event->GetMesh();
    for(int i=0; i<512; i++)
        tbjcPSAReg.mesh[i] = _mesh[i];
    tbjcPSAReg.RhoVariance = event->GetRhoVariance();
    tbjcPSAReg.QEventTot = event->GetEventCharge();

    tbjcBranch[0]->Fill();

    for(int i=0; i<event->GetNumHits(); i++)
    {
    tbjcHitReg.EventNum = fInternalID;
    ATHit* hit = event->GetHit(i);
    tbjcHitReg.PadNum = hit->GetHitPadNum();
    tbjcHitReg.hitNum = hit->GetHitID();
    tbjcHitReg.x = hit->GetPosition().x();
    tbjcHitReg.y = hit->GetPosition().y();
    tbjcHitReg.z = hit->GetPosition().z();
    tbjcHitReg.charge = hit->GetCharge();
    tbjcBranch[1]->Fill();
    }
}
int  ATPSATask::fInternalID=0;
void ATPSATask::Exec(Option_t *opt)
{

    fEventHArray -> Clear("C");

    if (fRawEventArray -> GetEntriesFast() == 0)
        return;

    ATRawEvent *rawEvent = (ATRawEvent *) fRawEventArray -> At(0);
    std::cout << "  Event Number :  " << rawEvent -> GetEventID() << " Valid pads : " << rawEvent -> GetNumPads() << std::endl;

    ATEvent *event = new ATEvent();
    fEventHArray->Insert(0,event);

    event -> SetEventID(rawEvent -> GetEventID());

    if (!(rawEvent -> IsGood()))
        event -> SetIsGood(kFALSE);
    else {
        fPSA -> Analyze(rawEvent, event);
        event -> SetIsGood(kTRUE);
    }

    ////// fill ////////
    Fill(fEventHArray);

    fInternalID++;
}
