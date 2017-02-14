#ifndef ATPHIRECOTRIPLE_H
#define ATPHIRECOTRIPLE_H
#include "TH1.h"
#include "ATPhiReco.hh"
class ATProtoEvent;
class ATProtoQuadrant;

class ATPhiRecoTriple : public ATPhiReco
{
  public:
    ATPhiRecoTriple();
    ~ATPhiRecoTriple();

    void PhiAnalyze(ATEvent *event,ATProtoEvent *protoevent);
    //void PhiAnalyze(ATEvent *event, ATHoughSpace *HSpace);

    friend Bool_t operator== ( const ATHit &n1, const ATHit &n2);

   private:
   void PhiCalc(ATProtoQuadrant *quadrant,ATEvent *event);
   void PhiCalcMulti(std::vector<ATHit> *multihit_Array,ATProtoQuadrant *quadrant);
   TH1D* PhiDist;

};

#endif
