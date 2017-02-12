
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

      //  TClonesArray* GetObject(const char* BrName);
        tbjcArray* GetObject(const char* BrName);
        static FairRootManager* Instance();
       // void   Register(const char*,const char*,TClonesArray*,Bool_t);
        void   Register(const char*,const char*,tbjcArray*,Bool_t);
        void SetFinishRun(Bool_t val=true);
        TFile* OpenOutFile(const char*);
        void  CloseOutFile() {

            std::cout<<"aaa1"<<std::endl;
            if(fOutFile) {
                std::cout<<"aaa"<<std::endl;
                fOutFile->Close(); }}
    private:

        TTree* fOutTree;

        TFile* fOutFile;
       // std::map < TString , TClonesArray*>     fMap;
        std::map < TString , tbjcArray*>     fMap;
        static                FairRootManager*  fgInstance;

};

#endif
