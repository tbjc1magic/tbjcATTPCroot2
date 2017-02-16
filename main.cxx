#include <vector>
#include "TObject.h"
#include <string>
#include <iostream>
#include "tbjcArray.h"
#include "tbjcClonesArray.h"
using namespace std;

class tt:public TObject
{
    public:
        int value;

        void print(int idx)
        {
            cout<<idx<<":"<<value<<endl;
        }
};

int test1()
{
    tbjcArray *t = new tbjcClonesArray<tt>(10);

    for(int i =0;i<13;i++)
    {
        tt*obj = (tt*)t->ConstructedAt(i);
        obj->value = i*i;
    }

    tt* tmp = new tt();
    t->Insert(3,tmp);
    t->Insert(30,tmp);

    //     t->Clear("C");
    cout<<"all:"<<t->GetEntriesFast()<<endl;
    t->Clear("C");
    //   cout<<"all:"<<t->GetEntriesFast()<<endl;
    t->Print();
    delete t;
}
/*
#include "AtTpcMap.h"
#include "AtTpcProtoMap.h"

#include "GETCoboFrame.hh"
#include "GETLayeredFrame.hh"

#include "GETDecoder2.hh"
#include "GETLayeredFrame.hh"
#include "GETBasicFrame.hh"

#include "TCanvas.h"
int test2()
{
GETDecoder2 * fDecoderPtr;
fDecoderPtr = new GETDecoder2();
fDecoderPtr->AddData("data/GRAWFILE.graw");
fDecoderPtr->SetData(0);
fDecoderPtr->SetNumTbs(512);

GETBasicFrame* _basicFrame = fDecoderPtr->GetBasicFrame(0);

TString GeoFile = "map/proto_geo_hires.root";
TString ProtoMap = "map/proto.map";
TString fMap = "map/LookupProto20150331.xml";

AtTpcProtoMap* fAtMapPtr = new AtTpcProtoMap();

fAtMapPtr->SetGeoFile(GeoFile);
fAtMapPtr->SetProtoMap(ProtoMap);
int MapIn = fAtMapPtr->ParseXMLMap(fMap);

cout<<MapIn<<endl;

int parsed = fAtMapPtr->ParseInhibitMap("","","");
cout<<parsed<<endl;

for(int j=0; j<1;j++)
{
cout<<"frame:"<<j<<"is here"<<endl;
Int_t *rawadc = _basicFrame -> GetSample(0,0);
// for(int i=0;i<512;i++)
//     cout<<i<<":"<<rawadc[i]<<endl;
}

TCanvas* t = new TCanvas("cc","cc",800,600);

delete fAtMapPtr;
delete fDecoderPtr;

return 1;
}
*/

#include "ATCore2.hh"

int test3()
{
    ATCore2 *fDecoder = new ATCore2(1);
    fDecoder->SetUseSeparatedData(0);
    fDecoder->SetInhibitMaps("","","");
    fDecoder->AddData("data/GRAWFILE.graw");

    fDecoder->SetData(0);
    fDecoder->SetNumTbs(512);
    fDecoder->SetFPNPedestal(5);

    TString fGeo = "map/proto_geo_hires.root";
    TString fProtoMap = "map/proto.map";
    TString fMap = "map/LookupProto20150331.xml";

    fDecoder->SetATTPCMap(fMap);
    fDecoder->SetProtoGeoFile(fGeo);
    fDecoder->SetProtoMapFile(fProtoMap);

    ATRawEvent* fRawEvent = fDecoder -> GetRawEvent(0);
    // fRawEvent = fDecoder -> GetRawEvent(1);
    bool tmp = true;
    ATPad*pad =  fRawEvent->GetPad(0, tmp);
    int * data = pad->GetRawADC();

    //   cout<<"here?"<<endl;
    //   for(int i=0;i<512;i++)
    //       cout<<i<<":"<<data[i]<<endl;

    cout<<"here?"<<endl;
    return 1;
}

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairParSet.h"
#include "FairLogger.h"
#include "TClonesArray.h"
int test4()
{
    FairLogger* fLog = new FairLogger();
    fLog->Error(MESSAGE_ORIGIN, "bobo1d");

    std::string a="123";

    std::cout<<a<<std::endl;

    tbjcArray* ta = new tbjcClonesArray<TMarker>(10);

    for(int i=0;i<1000;i++)
    {
        TMarker * tmp = new TMarker(i,i*i,1);
        ta->Insert(i,tmp);
    }

    FairRootManager* ioMan = new FairRootManager();
    ioMan->Register("a","a",ta,1);

    tbjcArray* b = ioMan->GetObject("a");

    std::cout<<((TMarker*)(b->At(10)))->GetX()<<" "<<((TMarker*)(b->At(10)))->GetY()<<std::endl;

    FairRun* run = new FairRun(1);

    FairParSet * a1 = new FairParSet();

    a1->a = 1;
    FairRuntimeDb* db = new FairRuntimeDb();
    db->addContainer("ATDigiPar",a1);
    run->SetRuntimeDb(db);

    FairRuntimeDb* db1 = run->GetRuntimeDb();

    FairParSet*b1 = db1->getContainer("ATDigiPar");
    std::cout<<b1->a<<std::endl;
    return 1;
}
#include "ATDigiPar.hh"
#include "ATDecoder2Task.hh"
#include "ATPSATask.hh"
#include "ATPhiRecoTask.hh"
#include "ATHoughTask.hh"
#include "ATAnalysisTask.hh"
int test5()
{
    TString outputFile = "output_proto.root";
    TString dataFile = "runfiles/1.txt";
    TString parameterFile = "pATTPC.ND2015.par";

    TString scriptfile = "LookupProto20150331.xml";
    TString protomapfile = "proto.map";
    TString dir = getenv("VMCWORKDIR");
    TString scriptdir = dir + "/scripts/"+ scriptfile;
    TString protomapdir = dir + "/scripts/"+ protomapfile;
    TString geo = "proto_geo_hires.root";
    TString paraDir = dir + "/parameters/";

    FairRunAna* run = new FairRunAna();

    run -> SetOutputFile(outputFile.Data());
    TString paramterFileWithPath = paraDir + parameterFile;

    FairRuntimeDb* rtdb = run->GetRuntimeDb();

    ATDigiPar * fPar = new ATDigiPar();
    rtdb->addContainer("ATDigiPar",(FairParSet*)fPar);

    // FairTask should not be released manually//
    // when the root task is deleted, all child//
    //  will be released sequentially//

    ATDecoder2Task *decoderTask = new ATDecoder2Task();
    decoderTask ->SetMapOpt(1);

    decoderTask->AddData("data/GRAWFILE.graw");
    decoderTask ->SetGeo("map/proto_geo_hires.root");
    decoderTask ->SetProtoMap("map/proto.map");
    decoderTask ->SetMap("map/LookupProto20150331.xml");
    // starting event number
    decoderTask->SetEventID(0);
    run -> AddTask(decoderTask);

    ATPSATask *psaTask = new ATPSATask();
    psaTask -> SetPersistence();
    //psaTask -> SetBackGroundPeakFinder(kFALSE); // Suppress background of each pad for noisy data (Larger computing Time)
    psaTask -> SetBackGroundPeakFinder(kFALSE);
    psaTask -> SetThreshold(20);
    psaTask -> SetPeakFinder(); //Note: For the moment not affecting the prototype PSA Task
    run -> AddTask(psaTask);

    ATPhiRecoTask *phirecoTask = new ATPhiRecoTask();
    phirecoTask -> SetPersistence();
    run -> AddTask(phirecoTask);

    ATHoughTask *HoughTask = new ATHoughTask();
    HoughTask->SetPhiReco();
    HoughTask->SetPersistence();
    HoughTask->SetLinearHough();
    HoughTask->SetRadiusThreshold(3.0); // Truncate Hough Space Calculation
    //HoughTask ->SetCircularHough();
    run ->AddTask(HoughTask);

    ATAnalysisTask *AnaTask = new ATAnalysisTask();
    AnaTask->SetPhiReco();
    AnaTask->SetHoughDist(2.0);
    AnaTask->SetPersistence(kTRUE);
    run->AddTask(AnaTask);

    cout<<"tt"<<run->GetNTasks()<<endl;
    run->Init();
   // run->Run(0,2);
    run -> RunOnTBData();
    //  delete psaTask; TTask
    //  delete decoderTask;
    delete fPar;
    delete run;

    cout<<"thing should be finished"<<endl;
}

int main()
{
    test5();
    return 1;
}
