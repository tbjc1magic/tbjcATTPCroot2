struct bobo
{
    float bobo1;
    int bobo2;
    float bobo3;
  //  map<string,float> pos;
};

struct pp
{
    float bobo1;
    int bobo2[4];
    float bobo3;
};

int TreeCreator()
{

    TFile *f = new TFile("output.root","RECREATE");

    bobo a;
    pp b;

    TTree* tree = new TTree("tree","tree");
    TBranch*b1 = tree->Branch("a", &a, "bobo1/f:bobo2/i:bobo3/f");//:pos/map<string,float>");
    TBranch*b2 = tree->Branch("b", &b, "bobo1/f:bobo2[4]/i:bobo3/f");

    for(int i=0;i<32;i++)
    {
        a.bobo1 =sqrt(i*i);
        a.bobo2 = i*i;
        a.bobo3 = sin(i*i/180.0*M_PI);
        /*
        map<string,float> pos;
        pos["x"] = i;
        pos["y"] = i*2;
        pos["z"] = i*3;

        a.pos = pos;
        */
        b1->Fill();
    }

    for(int i=0;i<64;i++)
    {
        b.bobo1 =sqrt(i*i);
        for(int j=0;j<4;j++) b.bobo2[j] = j*i;
        b.bobo3 = sin(i*i/180.0*M_PI);

        b2->Fill();
    }
    vector<float> *myp = new vector<float>;
    TBranch*b3 = tree->Branch("c","vector<float>",&myp);
    for(int i=0;i<10;i++)
    {
        (*myp).push_back(i);
        (*myp).push_back(i*2);
        (*myp).push_back(i*3);

        b3->Fill();
    }

    tree->Write();

    return 1;

}
