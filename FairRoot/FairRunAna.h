
#ifndef FAIRRUNANA_H
#define FAIRRUNANA_H

#include "FairRun.h"                    // for FairRun

#include "FairRootManager.h"            // for FairRootManager

#include "Rtypes.h"                     // for Bool_t, Double_t, UInt_t, etc
#include "TString.h"                    // for TString

class TF1;
class TFile;
class TTree;

class FairRunAna : public FairRun
{

  public:

    static FairRunAna* Instance();
    virtual ~FairRunAna();
    FairRunAna();
    void        Init();
    void        Run(Int_t NStart=0 ,Int_t NStop=0);
    void        Run(Double_t delta_t);
    void        Run(Long64_t entry);
    void        RunEventReco(Int_t NStart ,Int_t NStop);
    void        RunTSBuffers();
    void        DummyRun(Int_t NStart ,Int_t NStop);

    void RunMQ(Long64_t entry);
    void        RunOnLmdFiles(UInt_t NStart=0, UInt_t NStop=0);

    void RunOnTBData();
    void        TerminateRun();

    void        SetInputFile(TString fname);
    void        AddFile(TString name);
    void        AddFriend(TString fName);
    void        SetSignalFile(TString name, UInt_t identifier );

    void        AddSignalFile(TString name, UInt_t identifier );
    void        SetBackgroundFile(TString name);
    void        AddBackgroundFile(TString name);

    void BGWindowWidthNo(UInt_t background, UInt_t Signalid);
    void BGWindowWidthTime(Double_t background, UInt_t Signalid);
    void SetEventTimeInterval(Double_t min, Double_t max);
    void SetEventMeanTime(Double_t mean);
    void SetBeamTime(Double_t beamTime, Double_t gapTime);

    void        Reinit(UInt_t runId);
   // UInt_t      getRunId() {return fRunId;}

    void        SetGeomFile(const char* GeoFileName);
    TFile*      GetGeoFile() {return fInputGeoFile;}
    void        SetContainerStatic(Bool_t tempBool=kTRUE);
    Bool_t      GetContainerStatic() { return fStatic; };
    void        RunWithTimeStamps();
    Bool_t      IsTimeStamp() {return fTimeStamps;}
    void StopProcessingLMD( void ) {fFinishProcessingLMDFile = kTRUE;}
    Bool_t GetLMDProcessingStatus( void ) {return fFinishProcessingLMDFile;}

  private:

    FairRunAna(const FairRunAna& M);
    FairRunAna& operator= (const  FairRunAna&) {
      return *this;
    }

  protected:
    Bool_t                                  fIsInitialized;
    TFile*                                  fInputGeoFile;
    static FairRunAna*                      fgRinstance;
    Bool_t                                  fLoadGeo;
    Bool_t                                  fStatic;//!
    Bool_t                                  fTimeStamps;
    Bool_t                                  fInFileIsOpen;//!
    Double_t                                fEventTimeMin;  //!
    Double_t                                fEventTimeMax;  //!
    Double_t                                fEventTime;     //!
    Double_t                                fEventMeanTime; //!
    TF1*                                    fTimeProb;      //!
    Bool_t                                  fFinishProcessingLMDFile;  //!

};

#endif //FAIRRUNANA_H
