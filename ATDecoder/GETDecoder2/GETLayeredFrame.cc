#include "GETLayeredFrame.hh"
#include "tbjcClonesArray.h"
#include "tbjcArray.h"

GETLayeredFrame::GETLayeredFrame() {
    fFrames = NULL;

    Clear();
}

Int_t  GETLayeredFrame::GetNumFrames()        { return GetNItems(); }
tbjcArray *GETLayeredFrame::GetFrames()           { return fFrames; }
GETBasicFrame *GETLayeredFrame::GetFrame(Int_t index) { return (GETBasicFrame * ) fFrames -> At(index); }

void GETLayeredFrame::Clear(Option_t *) {
    if (fFrames == NULL)
        fFrames = new tbjcClonesArray<GETBasicFrame>(48);

    GETLayerHeader::Clear();
    fFrames -> Clear("C");
}

void GETLayeredFrame::Read(ifstream &stream) {
    Clear();

    GETLayerHeader::Read(stream);

    for (Int_t iFrame = 0; iFrame < GetNItems(); iFrame++) {
        GETBasicFrame *frame = (GETBasicFrame *) fFrames -> ConstructedAt(iFrame);
        frame -> Read(stream);
    }
}
