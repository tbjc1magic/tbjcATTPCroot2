#include "FairRunAna.h"
#include "FairLogger.h"
#include "RVersion.h"                   // for ROOT_VERSION, etc
#include "Riosfwd.h"                    // for ostream
#include "TChain.h"                     // for TChain
#include "TCollection.h"                // for TIter
#include "TDirectory.h"                 // for TDirectory, gDirectory
#include "TFile.h"                      // for TFile, gFile
#include "TGeoManager.h"                // for gGeoManager, TGeoManager
#include "TKey.h"                       // for TKey
#include "TList.h"                      // for TList
#include "TNamed.h"                     // for TNamed
#include "TObjArray.h"                  // for TObjArray
#include "TObject.h"                    // for TObject
#include "TROOT.h"                      // for TROOT, gROOT
#include "TSeqCollection.h"             // for TSeqCollection
#include "TSystem.h"                    // for TSystem, gSystem
#include "TTree.h"                      // for TTree
#include "FairTask.h"
#include <stdlib.h>                     // for NULL, exit
#include "signal.h"
#include <string.h>                     // for strcmp
#include <iostream>                     // for operator<<, basic_ostream, etc
#include <list>                         // for list

using std::cout;
using std::endl;
using std::list;

FairRunAna* FairRunAna::fgRinstance= 0;
FairRunAna* FairRunAna::Instance(){return fgRinstance;}

FairRunAna::FairRunAna()
    :FairRun(),
    fIsInitialized(kFALSE),
    fInputGeoFile(0),
    fLoadGeo( kFALSE),
    fStatic(kFALSE),
    fTimeStamps(kFALSE),
    fInFileIsOpen(kFALSE),
    fEventTimeMin(0),
    fEventTimeMax(0),
    fEventTime(0)
{

    fgRinstance=this;
}

FairRunAna::~FairRunAna()
{

    cout<<"FairRunAna end here"<<endl;
}

void  FairRunAna::SetGeomFile(const char* GeoFileName)
{
    if (fIsInitialized) {
        LOG(FATAL) << "Geometry file has to be set before Run::Init !"
            << FairLogger::endl;
        exit(-1);
    } else {

        TFile* CurrentFile=gFile;
        fInputGeoFile= new TFile(GeoFileName);
        if (fInputGeoFile->IsZombie()) {
            LOG(ERROR) << "Error opening Geometry Input file"
                << FairLogger::endl;
            fInputGeoFile=0;
        }
        LOG(INFO) << "Opening Geometry input file: " << GeoFileName
            << FairLogger::endl;
        fLoadGeo=kTRUE;
        gFile=CurrentFile;
    }
}

void FairRunAna::Init()
{

  fTask->SetParTask();
  fTask->InitTask();

}

void FairRunAna::Run(Int_t Ev_start, Int_t Ev_end)
{

    for (int i=Ev_start; i< Ev_end ; i++) {

        cout<<"hahaha"<<endl;
        cout<<"hahaha"<<fTask->GetName()<<endl;
        fTask->ExecuteTask("");
     //   fTask->FinishEvent();

    }
}

void FairRunAna::RunOnTBData() {
      std::cout << "FairRunAna::RunOnTBData " << std::endl;
        while (fRootManager->FinishRun() != kTRUE) {
		fTask->ExecuteTask("");
          //  Fill();
            fTask->FinishEvent();
        }

        fTask->FinishTask();
      //  fRootManager->LastFill();
      //  fRootManager->Write();
}
