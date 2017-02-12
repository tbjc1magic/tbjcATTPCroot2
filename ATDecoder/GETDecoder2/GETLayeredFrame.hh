#ifndef GETLAYEREDFRAME
#define GETLAYEREDFRAME

#include "GETLayerHeader.hh"
#include "GETBasicFrame.hh"

class tbjcArray;
class GETLayeredFrame : public GETLayerHeader {
    public:
        GETLayeredFrame();

        Int_t  GetNumFrames();
        tbjcArray *GetFrames();
        GETBasicFrame *GetFrame(Int_t index);

        void  Clear(Option_t * = "");
        void  Read(ifstream &stream);

    private:
        tbjcArray *fFrames;

};

#endif
