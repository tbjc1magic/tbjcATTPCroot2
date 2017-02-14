#ifndef ATPHIRECOSIMPLE_H
#define ATPHIRECOSIMPLE_H
#include "TH1.h"
#include "ATPhiReco.hh"
class ATProtoEvent;
class ATProtoQuadrant;

class ATPhiRecoSimple : public ATPhiReco
{
  public:
    ATPhiRecoSimple();
    ~ATPhiRecoSimple();

    void PhiAnalyze(ATEvent *event,ATProtoEvent *protoevent);
    //void PhiAnalyze(ATEvent *event, ATHoughSpace *HSpace);

   private:
   void PhiCalc(ATProtoQuadrant *quadrant,ATEvent *event);
   void PhiCalcMulti(std::vector<ATHit> *multihit_Array,ATProtoQuadrant *quadrant);
   TH1D* PhiDist;

};

#endif
