#include "ATDecoder2Task.hh"

//#include "STGlobal.hh"
//#include "STDebugLogger.hh"

// FAIRROOT classes
#include "FairRootManager.h"
//#include "FairRunOnline.h"
#include "FairRun.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include "tbjcArray.h"
#include "tbjcClonesArray.h"

ATDecoder2Task::ATDecoder2Task()
{
    fLogger = FairLogger::GetLogger();

    fDecoder = NULL;
    fDataNum = 0;
    fOpt = 0;

    fFPNPedestalRMS = -1;

    fExternalNumTbs = kFALSE;
    fNumTbs = 512;

    //fUseGainCalibration = kFALSE;
    //fGainCalibrationFile = "";
    //fGainConstant = -9999;
    //fGainLinear = -9999;
    //fGainQuadratic = 0;

    fIsPersistence = kFALSE;
    fIsPositive = kFALSE;

    fPar = NULL;
    fRawEventArray = new tbjcClonesArray<ATRawEvent>(10);
    fRawEvent = NULL;

    fIsSeparatedData = kFALSE;
    fIsPseudoTopology = kFALSE;

    fInternalID = 0;

    fEventID = -1;
}

ATDecoder2Task::~ATDecoder2Task()
{

    cout<<"ATDecoder2Task end here 1"<<endl;
    delete fDecoder;
    cout<<"ATDecoder2Task end here 2"<<endl;
    delete fRawEventArray;
    cout<<"ATDecoder2Task end here 3"<<endl;
}

void ATDecoder2Task::SetPersistence(Bool_t value)                                              { fIsPersistence = value; }
void ATDecoder2Task::SetNumTbs(Int_t numTbs)                                                   { fNumTbs = numTbs; fExternalNumTbs = kTRUE; }
void ATDecoder2Task::AddData(TString filename, Int_t coboIdx)                                  { fDataList[coboIdx].push_back(filename); }
void ATDecoder2Task::SetData(Int_t value)                                                      { fDataNum = value; }
void ATDecoder2Task::SetFPNPedestal(Double_t pedestalRMS)                                      { fFPNPedestalRMS = pedestalRMS; }
void ATDecoder2Task::SetPositivePolarity(Bool_t value)                                          { fIsPositive = value; }
//void ATDecoder2Task::SetUseGainCalibration(Bool_t value)                                       { fUseGainCalibration = value; }
//void ATDecoder2Task::SetGainCalibrationData(TString filename)                                  { fGainCalibrationFile = filename; }
//void ATDecoder2Task::SetGainReference(Double_t constant, Double_t linear, Double_t quadratic)  { fGainConstant = constant; fGainLinear = linear; fGainQuadratic = quadratic; }
void ATDecoder2Task::SetUseSeparatedData(Bool_t value)                                         { fIsSeparatedData = value; }
void ATDecoder2Task::SetEventID(Long64_t eventid)                                              { fEventID = eventid; }
void ATDecoder2Task::SetGeo(TString geofile)				                                           { fGeoFile = geofile; }
void ATDecoder2Task::SetProtoMap(TString mapfile)	                                             { fProtoMapFile = mapfile;}
void ATDecoder2Task::SetMapOpt(Int_t value)                                                    { fOpt = value; }
Bool_t ATDecoder2Task::SetMap(Char_t const *map)                                               { fMap = map; }
void ATDecoder2Task::SetPseudoTopologyFrame(Bool_t value)                                      { fIsPseudoTopology = value; }
void ATDecoder2Task::SetInhibitMaps(TString inimap, TString lowgmap, TString xtalkmap)         { fIniMap = inimap; fLowgMap = lowgmap; fXtalkMap = xtalkmap; }

Long64_t ATDecoder2Task::GetEventID() { return fEventIDLast; }

    InitStatus
ATDecoder2Task::Init()
{
    FairRootManager *ioMan = FairRootManager::Instance();
    if (ioMan == 0) {
        fLogger -> Error(MESSAGE_ORIGIN, "Cannot find RootManager!");

        return kERROR;
    }

    ioMan -> Register("ATRawEvent", "ATTPC", fRawEventArray, fIsPersistence);

    fDecoder = new ATCore2(fOpt);
    fDecoder -> SetUseSeparatedData(fIsSeparatedData);
    fDecoder -> SetInhibitMaps(fIniMap,fLowgMap,fXtalkMap);

    for (Int_t iFile = 0; iFile < fDataList[0].size(); iFile++)
        fDecoder -> AddData(fDataList[0].at(iFile));

    if (fIsSeparatedData)
        for (Int_t iCobo = 1; iCobo < 10; iCobo++)
            for (Int_t iFile = 0; iFile < fDataList[iCobo].size(); iFile++)
                fDecoder -> AddData(fDataList[iCobo].at(iFile), iCobo);

    if(fIsPseudoTopology) fDecoder-> SetPseudoTopologyFrame(0xF,kFALSE);

    fDecoder -> SetData(fDataNum);

    if (fExternalNumTbs)
        fDecoder -> SetNumTbs(fNumTbs);
    else
        fDecoder -> SetNumTbs(fPar -> GetNumTbs());

    if (fFPNPedestalRMS == -1)
        //fFPNPedestalRMS = fPar -> GetFPNPedestalRMS();
        fFPNPedestalRMS =5;

    fDecoder -> SetFPNPedestal(fFPNPedestalRMS);

    Bool_t kMapIn = fDecoder -> SetATTPCMap(fMap);
    //std::cout<<kMapIn<<std::endl;
    if (!kMapIn) {
        fLogger -> Error(MESSAGE_ORIGIN, "Cannot find ATTPC Map!");

        return kERROR;
    }

    if(fOpt==1){
        fDecoder -> SetProtoGeoFile(fGeoFile);
        fDecoder -> SetProtoMapFile(fProtoMapFile);
    }

    return kSUCCESS;
}

    void
ATDecoder2Task::SetParContainers()
{
    FairRun *run = FairRun::Instance();
    if (!run)
        fLogger -> Fatal(MESSAGE_ORIGIN, "No analysis run!");

    FairRuntimeDb *db = run -> GetRuntimeDb();
    if (!db)
        fLogger -> Fatal(MESSAGE_ORIGIN, "No runtime database!");

    fPar = (ATDigiPar *) db -> getContainer("ATDigiPar");
    if (!fPar)
        fLogger -> Fatal(MESSAGE_ORIGIN, "Cannot find ATDigiPar!");
}

void ATDecoder2Task::test(tbjcArray* fRawEventArray)
{
    ATRawEvent* fEvent = static_cast<ATRawEvent*>(fRawEventArray->At(0));
    ATPad* fPad = fEvent->GetPad(0);
    Int_t * value = fPad->GetRawADC();
    for(int i=0;i<512;i++)
    {
        std::cout<<i<<":"<<value[i]<<std::endl;
    }

    std::cout<< fRawEventArray->GetEntriesFast()<<std::endl;
    std::cout<<"Decoder Finished"<<std::endl;
}

void ATDecoder2Task::Exec(Option_t *opt)
{
    fRawEventArray -> Clear("C");

    if (fRawEvent == NULL)
        fRawEvent = fDecoder -> GetRawEvent(fEventID++);
    fInternalID++;
    if(fInternalID%100==0) std::cout<<" Event Number "<<fEventID<<" Internal ID : "<<fInternalID<<" Number of Pads : "<<fRawEvent->GetNumPads()<<std::endl;

    ATRawEvent* fEvent = new ATRawEvent(fRawEvent);
    fRawEventArray->Insert(0,fEvent);

  //  test(fRawEventArray);
    fRawEvent = NULL;
}

    Int_t
ATDecoder2Task::ReadEvent(Int_t eventID)
{
    fRawEventArray -> Clear("C");

    fRawEvent = fDecoder -> GetRawEvent(eventID);
    fEventIDLast = fDecoder -> GetEventID();

    if (fRawEvent == NULL)
        return 1;

    ATRawEvent* tmp = new ATRawEvent(fRawEvent);
    fRawEventArray->Insert(0,tmp);

    //  new ((*fRawEventArray)[0]) ATRawEvent(fRawEvent);

    return 0;
}

    void
ATDecoder2Task::FinishEvent()
{
    fRawEvent = fDecoder -> GetRawEvent();

    if (fRawEvent == NULL)
    {
        fLogger -> Info(MESSAGE_ORIGIN, "End of file. Terminating FairRun.");
        FairRootManager::Instance() -> SetFinishRun();
    }
}
