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
    tbjcArray *t = new tbjcClonesArray<tt>(1000);

    for(int i =0;i<1300;i++)
    {
        tt*obj = (tt*)t->ConstructedAt(i);
        obj->value = i*i;
    }

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
    for(int i=0;i<512;i++)
        cout<<i<<":"<<data[i]<<endl;

    cout<<"here?"<<endl;
    return 1;
}

int main()
{
    test3();
    return 1;
}
