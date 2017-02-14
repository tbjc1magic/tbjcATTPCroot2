#include "ATHoughSpaceLine.hh"
#include "ATHoughSpace.hh"

// FairRoot classes
#include "FairRuntimeDb.h"
#include "FairRun.h"

ATHoughSpace::ATHoughSpace()
{
  fThreshold = 0.0;
}

ATHoughSpace::~ATHoughSpace()
{
}

void ATHoughSpace::SetThreshold(Double_t value) {fThreshold = value;}
