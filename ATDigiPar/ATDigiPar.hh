
#ifndef ATDIGIPAR_H
#define ATDIGIPAR_H

#include <iostream>
#include <fstream>

#include "ATGas.hh"

#include "TString.h"
#include "TSystem.h"

class ATDigiPar
{
  public:
    // Constructors and Destructors
    ATDigiPar();
    ~ATDigiPar();

    // Getters
    Int_t GetPadPlaneX();
    Int_t GetPadPlaneZ();
    Int_t GetPadSizeX();
    Int_t GetPadSizeZ();
    Int_t GetPadRows();
    Int_t GetPadLayers();
    Double_t GetAnodeWirePlaneY();
    Double_t GetGroundWirePlaneY();
    Double_t GetGatingWirePlaneY();
    ATGas *GetGas();
    Int_t GetNumTbs();
    Int_t GetTBTime();              ///< returns the time duration of a time bucket in given sampling time in ns.
    Double_t GetDriftVelocity();    ///< returns the drift velocity in cm/us.
    Double_t GetDriftLength();      ///< returns the drift length in mm
    Int_t GetYDivider();            ///< returns the slice divider

    TString GetFile(Int_t fileNum);

    Double_t GetBField();
    Double_t GetEField();
    Double_t GetTiltAngle();
    Int_t    GetTB0();
    Double_t GetThetaLorentz();
    Int_t    GetTBEntrance();

    // Setters
  //  virtual Bool_t getParams(FairParamList *paramList);
  //  virtual void putParams(FairParamList *paramList);

    // Main methods

  private:

    Bool_t getParams();

    ATGas *fGas;
    TString fGasFileName;

    Bool_t fInitialized;

    Int_t fPadPlaneX;
    Int_t fPadPlaneZ;
    Int_t fPadSizeX;
    Int_t fPadSizeZ;
    Int_t fPadRows;
    Int_t fPadLayers;
    Double_t fAnodeWirePlaneY;
    Double_t fGroundWirePlaneY;
    Double_t fGatingWirePlaneY;
    Double_t fEField;
    Int_t fNumTbs;
    Int_t fSamplingRate;
    Double_t fDriftVelocity;
    Double_t fDriftLength;
    Int_t fYDivider;
    Int_t fGasFile;
    Int_t fPadPlaneFile;
    Int_t fPadShapeFile;

    Double_t fBField;
    Double_t fTiltAng;
    Int_t fTB0;
    Double_t fThetaLorentz;
    Int_t fTBEntrance;

//    ClassDef(ATDigiPar, 1);
};

#endif
