#ifndef GETLAYEREDFRAME
#define GETLAYEREDFRAME

#include "GETLayerHeader.hh"
#include "GETBasicFrame.hh"

#include "TClonesArray.h"
class tbjcArray;
class GETLayeredFrame : public GETLayerHeader {
  public:
    GETLayeredFrame();

            Int_t  GetNumFrames();
   //  TClonesArray *GetFrames();
    tbjcArray *GetFrames();
    GETBasicFrame *GetFrame(Int_t index);

             void  Clear(Option_t * = "");
             void  Read(ifstream &stream);

  private:
    // TClonesArray *fFrames;
     tbjcArray *fFrames;

};

#endif
