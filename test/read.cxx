#include "Math/Vector3D.h"
#include "Math/Vector4D.h"
double read() {

  TRandom R;
  TStopwatch timer;

  TH1D * h1 = new TH1D("h1","total event  energy ",100,0,1000.);
  TH1D * h2 = new TH1D("h2","Number of track per event",21,-0.5,20.5);
  TH1D * h3 = new TH1D("h3","Track Energy",100,0,200);
  TH1D * h4 = new TH1D("h4","Track Pt",100,0,100);
  TH1D * h5 = new TH1D("h5","Track Eta",100,-5,5);
  TH1D * h6 = new TH1D("h6","Track Cos(theta)",100,-1,1);

  TFile f1("mathcoreLV.root");

  // create tree
  TTree *t1 = (TTree*)f1.Get("t1");

  std::vector<float> * pTracks = 0;
  TBranch *tb1 = t1->GetBranch("tracks");
    tb1->SetAddress(&pTracks);
  // t1->SetBranchAddress("tracks",&pTracks);

  timer.Start();
  int n = (int) tb1->GetEntries();
  std::cout << " Tree Entries " << n << std::endl;
  double sum=0;
  for (int i = 0; i < n; ++i) {
    tb1->GetEntry(i);
    int ntrk = pTracks->size();
    for (int j = 0; j < ntrk; ++j) {
        cout<<(*pTracks)[j]<<":";
    }
    cout<<endl;
  }
  return sum;
}
