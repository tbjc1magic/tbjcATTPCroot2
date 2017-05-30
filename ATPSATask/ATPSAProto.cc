#include "ATPSAProto.hh"
#include "TH1.h"
#include "ATRawEvent.hh"
#include "FairLogger.h"
#include "ATHit.hh"
#include "ATEvent.hh"
// STL
#include <cmath>
#include <map>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <iostream>

ATPSAProto::ATPSAProto()
{
    //fPeakFinder = new TSpectrum();
    //HPeak = new TH1F("HPeak","HPeak",512,0,511);
}

ATPSAProto::~ATPSAProto()
{
}

    void
ATPSAProto::Analyze(ATRawEvent *rawEvent, ATEvent *event)
{
    Int_t numPads = rawEvent -> GetNumPads();
    Int_t hitNum = 0;
    Double_t QEventTot=0.0;
    Double_t RhoVariance = 0.0;
    Double_t RhoMean = 0.0;
    Double_t Rho2 = 0.0;
    std::map<Int_t,Int_t> PadMultiplicity;
    Float_t mesh[512] = {0};

    Int_t iPad = 0;

#pragma omp parallel for ordered schedule(dynamic,1) private(iPad)
    for (iPad = 0; iPad < numPads; iPad++)
    {

        ATPad *pad = rawEvent -> GetPad(iPad);
        Int_t PadNum = pad->GetPadNum();
        Double_t QHitTot = 0.0;
        Int_t PadHitNum = 0;
        TVector3 HitPos;
        Bool_t fValidBuff = kTRUE;
        Bool_t fValidThreshold = kTRUE;

        Double_t xPos = pad -> GetPadXCoord();
        Double_t yPos = pad -> GetPadYCoord();
        Double_t zPos = 0;
        Double_t charge = 0;

        if (!(pad -> IsPedestalSubtracted())) {
            fLogger -> Error(MESSAGE_ORIGIN, "Pedestal should be subtracted to use this class!");

            //return;
        }

        Double_t *adc = pad -> GetADC();
        Double_t floatADC[512] = {0};
        Double_t dummy[512] = {0};
        Int_t zeroPeak = 0;

        for (Int_t iTb = 0; iTb < fNumTbs; iTb++){
            floatADC[iTb] = adc[iTb];
            QHitTot+=adc[iTb];

        }

        TSpectrum *PeakFinder = new TSpectrum;
        Int_t numPeaks=0;
        //#pragma omp ordered
        numPeaks = PeakFinder -> SearchHighRes(floatADC, dummy, fNumTbs, 4.7, 5, fBackGroundSuppression, 3, kTRUE, 3);

        if(numPeaks==0) zeroPeak=1;
        else zeroPeak=0;

        if(fValidBuff){
            for (Int_t iPeak = 0; iPeak < numPeaks+zeroPeak; iPeak++) {
                Int_t maxAdcIdx=0;
                Float_t max=0.0;
                Float_t min=0.0;
                Int_t maxTime = 0;

                if(numPeaks>0) maxAdcIdx = (Int_t)(ceil((PeakFinder -> GetPositionX())[iPeak]));
                else if(numPeaks==0){

                    for (Int_t ij = 20; ij < 500; ij++) //Excluding first and last 10 Time Buckets
                    {
                        if (floatADC[ij] > max)
                        {
                            max = floatADC[ij];
                            maxTime = ij;
                        }
                    }

                    maxAdcIdx = maxTime;

                }//if numPeaks

                zPos = CalculateZ(maxAdcIdx);
                charge = adc[maxAdcIdx];

                if (fThreshold > 0 && charge < fThreshold)// TODO: Does this work when the polarity is negative??
                    fValidThreshold = kFALSE;

                if(fValidThreshold){
                    if(iPeak==0) QEventTot+=QHitTot; //Sum only if Hit is valid - We only sum once (iPeak==0) to account for the whole spectrum.

                    ATHit *hit = new ATHit(PadNum,hitNum, xPos, yPos, zPos, charge);
                    PadHitNum++;
                    hit->SetQHit(QHitTot); // TODO: The charge of each hit is the total charge of the spectrum, so for double structures this is unrealistic.
                    hit->SetTimeStamp(maxAdcIdx);
                    HitPos =  hit->GetPosition();
                    Rho2+= HitPos.Mag2();
                    RhoMean+=HitPos.Mag();
                    if((xPos<-9000 || yPos<-9000) && pad->GetPadNum()!=-1 ) std::cout<<" ATPSAProto::Analysis Warning! Wrong Coordinates for Pad : "<<pad->GetPadNum()<<std::endl;
#pragma omp ordered
                    event -> AddHit(hit);
                    delete hit;
#pragma omp ordered
                    hitNum++;

                }//Valid Threshold

            }// Peak loop

            for (Int_t iTb = 0; iTb < fNumTbs; iTb++) mesh[iTb]+=floatADC[iTb];
#pragma omp ordered
            PadMultiplicity.insert(std::pair<Int_t,Int_t>(PadNum,PadHitNum));

        }//if Valid Num Peaks (ValidBuff)

        delete PeakFinder;

    }// Pad loop
    RhoVariance = Rho2 - ( pow(RhoMean,2)/(event->GetNumHits()) );
    RhoVariance = Rho2 - ( event->GetNumHits()*pow((RhoMean/event->GetNumHits()),2) ) ;
    for (Int_t iTb = 0; iTb < fNumTbs; iTb++) event -> SetMeshSignal(iTb,mesh[iTb]);
    event -> SetMultiplicityMap(PadMultiplicity);
    event -> SetRhoVariance(RhoVariance);
    event -> SetEventCharge(QEventTot);

}
