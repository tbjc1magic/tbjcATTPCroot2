// FAIRROOT classes
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "ATEvent.hh"
#include "ATHit.hh"
#include <iostream>

#include "ATPhiRecoTask.hh"
#include "ATPhiReco.hh"
#include "ATProtoEvent.hh"
#include "ATPhiRecoTriple.hh"
#include "ATPhiRecoSimple.hh"

#include "tbjcClonesArray.h"
#include "tbjcArray.h"

ATPhiRecoTask::ATPhiRecoTask(): FairTask("ATPhiRecoTask")
{
    fLogger = FairLogger::GetLogger();
    fPar = NULL;

    fIsPersistence = kFALSE;
    fPhiRecoMode = 0; // Default

    fPEventArray = new tbjcClonesArray<ATProtoEvent>(10);
}

ATPhiRecoTask::~ATPhiRecoTask()
{
    std::cout<<"end is hereATPhiRecoTask"<<std::endl;

}

void ATPhiRecoTask::SetPersistence(Bool_t value)     { fIsPersistence = value; }
void ATPhiRecoTask::SetThreshold(Double_t threshold) { fThreshold = threshold; }
void ATPhiRecoTask::SetPhiRecoMode(Int_t value)      { fPhiRecoMode = value; }

    InitStatus
ATPhiRecoTask::Init()
{

    FairRootManager *ioMan = FairRootManager::Instance();
    if (ioMan == 0) {
        fLogger -> Error(MESSAGE_ORIGIN, "Cannot find RootManager!");
        return kERROR;
    }

    fEventHArray = (tbjcArray *) ioMan -> GetObject("ATEventH");
    if (fEventHArray == 0) {
        fLogger -> Error(MESSAGE_ORIGIN, "Cannot find ATEvent array!");
        return kERROR;
    }

    if (fPhiRecoMode == 0) {
        fLogger -> Info(MESSAGE_ORIGIN, "Using ATPhiRecoSimple!");

        fPhiReco = new ATPhiRecoSimple();
    } else if (fPhiRecoMode == 1) {
        fLogger -> Info(MESSAGE_ORIGIN, "Using ATPhiRecoTriple!");

        fPhiReco = new ATPhiRecoTriple();
    }

    //fPSA -> SetThreshold((Int_t)fThreshold);

    ioMan -> Register("ATProtoEvent", "ATTPC", fPEventArray, fIsPersistence);

    return kSUCCESS;
}

    void
ATPhiRecoTask::SetParContainers()
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

    void
ATPhiRecoTask::Exec(Option_t *opt)
{
    std:: cout<<"I hate shit1"<<std::endl;
    fPEventArray -> Clear("C");

    if (fEventHArray -> GetEntriesFast() == 0)
        return;

    ATEvent *event = (ATEvent *) fEventHArray -> At(0);

    //std::cout << "  Event Number :  " << Event -> GetEventID() << std::endl;
    ATProtoEvent *protoevent = new ATProtoEvent();
    fPEventArray->Insert(0,protoevent);

    protoevent->SetEventID(event->GetEventID());
    fPhiReco->PhiAnalyze(event,protoevent);
/*
    std:: cout<<"I hate shit"<<std::endl;

       ATProtoEvent* fEvent = (ATProtoEvent*)(fPEventArray->At(0));
       vector<ATProtoQuadrant>* fquad = fEvent->GetQuadrantArray();
       for(auto it : *fquad)
       {
       vector<ATHit> *fHit = it.GetHitArray();
       for(auto jt: *fHit)
       {
       TVector3 pos = jt.GetPosition();
       std::cout<<"aa:"<<pos.x()<<":"<<pos.y()<<":"<<pos.z()<<std::endl;
       }
       }
*/
}
