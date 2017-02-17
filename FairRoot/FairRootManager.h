
#ifndef FAIR_ROOT_MANAGER_H
#define FAIR_ROOT_MANAGER_H

#pragma link C++ class FairRootManager+;

#include "TObject.h"                    // for TObject

#include "Rtypes.h"                     // for Bool_t, Int_t, UInt_t, etc
#include "TChain.h"                     // for TChain
#include "TFile.h"                      // for TFile
#include "TString.h"                    // for TString, operator<
#include "TMCtls.h"                     // for multi-threading

#include <stddef.h>                     // for NULL
#include <list>                         // for list
#include <map>                          // for map, multimap, etc
#include <queue>                        // for queue
#include "TClonesArray.h"
#include "TMarker.h"
#include <vector>
#include "TTree.h"
#include <iostream>

class tbjcArray;

class FairRootManager : public TObject
{
    public:
        FairRootManager();
        virtual ~FairRootManager();

        tbjcArray* GetObject(const char* BrName);
        static FairRootManager* Instance();
        void   Register(const char*,const char*,tbjcArray*,Bool_t);
        Bool_t FinishRun() {return fFinishRun;}
        void SetFinishRun(Bool_t val = kTRUE){ fFinishRun = val;}
    private:

        TTree* fOutTree;

        Bool_t fFinishRun; //!
        TFile* fOutFile;
        std::map < TString , tbjcArray*>     fMap;
        static                FairRootManager*  fgInstance;

};

#endif
