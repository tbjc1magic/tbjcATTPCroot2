#include "TRandom.h"
#include "TStopwatch.h"
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TMath.h"

#include <iostream>

// CINT does not understand some files included by LorentzVector
#include "Math/Vector3D.h"
#include "Math/Vector4D.h"

using namespace ROOT::Math;

double write() {

    int n=100;
  TRandom R;
  TStopwatch timer;

  TFile f1("mathcoreLV.root","RECREATE");

  // create tree
  TTree t1("t1","Tree with new LorentzVector");

  std::vector<float>  tracks;
  std::vector<float> * pTracks = &tracks;
  t1.Branch("tracks","std::vector<float>",&pTracks);

  double M = 0.13957;  // set pi+ mass

  timer.Start();
  double sum = 0;
  for (int i = 0; i < n; ++i) {
    pTracks->push_back(0*i);
    pTracks->push_back(1*i);

    t1->Fill();
  }

  return sum;
}
