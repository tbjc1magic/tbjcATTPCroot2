struct bobo
{
    float bobo1;
    int bobo2;
    float bobo3;
};

struct pp
{
    float bobo1;
    int bobo2[4];
    float bobo3;
};

int TreeReader()
{

    TFile *f = new TFile("output.root");

    bobo a;
    pp b;

    TTree* tree = static_cast<TTree*>(f->Get("tree"));
    TBranch* tb1 = tree->GetBranch("a");
    TBranch* tb2 = tree->GetBranch("b");

    tb1->SetAddress(&a);
    tb2->SetAddress(&b);

    for(int i=0; i<tb1->GetEntries(); i++)
    {
        tb1->GetEntry(i);
        cout<<i<<":"<<a.bobo1<<":"<<a.bobo2<<":"<<a.bobo3<<endl;
    }

    for(int i=0; i<tb2->GetEntries(); i++)
    {
        tb2->GetEntry(i);
        cout<<i<<":"<<b.bobo1<<":";
        for(int j=0; j<4; j++)
        cout<<b.bobo2[j]<<":";
        cout<<b.bobo3<<endl;
    }

  std::vector<float> * pTracks = 0;
  TBranch *tb3 = tree->GetBranch("c");
    tb3->SetAddress(&pTracks);

      for (int i = 0; i < tb3->GetEntries(); ++i) {
        tb3->GetEntry(i);
        int ntrk = pTracks->size();
        for (int j = 0; j < ntrk; ++j) {
            cout<<(*pTracks)[j]<<":";
        }
        cout<<endl;
      }
   /*
      for(int i=0; i<tb3->GetEntries(); i++)
    {
        tb3->GetEntry(i);
        cout<<(*myp).size()<<endl;
   //     cout<<(*myp)[0]<<":";
   //     cout<<(*myp)[1]<<":";
   //     cout<<(*myp)[2]<<endl;
    }
*/
    return 1;

}
