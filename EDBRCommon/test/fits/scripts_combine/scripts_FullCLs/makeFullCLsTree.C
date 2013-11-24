{
  double limit;
  double mh;
  float quantileExpected;
  ifstream massesfile ("masses.txt");
  ifstream limitOfile ("observedLimit.txt");
  ifstream limitE0file ("expectedLimit.txt");
  ifstream limitM2file ("limitM2.txt");
  ifstream limitM1file ("limitM1.txt");
  ifstream limitP1file ("limitP1.txt");
  ifstream limitP2file ("limitP2.txt");

  TFile* f = TFile::Open("higgsCombineEXOZZ.HybridNew.TOTAL.root","RECREATE");
  TTree* t = new TTree("limit","limit");
  t->Branch("limit",&limit,"limit/D");
  t->Branch("mh",&mh,"mh/D");
  t->Branch("quantileExpected",&quantileExpected,"quantileExpected/F");


  //  for(int i=600; i!=2550; i=(i+50)) {
  int mass;
  while (massesfile.good()) {
    
    //    int mass = i;
    massesfile>>mass;
    mh = mass;
    
    limitM2file >> limit;
    quantileExpected = 0.025;
    printf("%g\t%g\t%g\n",mass,limit,quantileExpected);
    t->Fill();
    limitM1file >> limit;
    quantileExpected = 0.16;
    printf("%g\t%g\t%g\n",mass,limit,quantileExpected);
    t->Fill();
    limitE0file >> limit;
    quantileExpected = 0.5;
    printf("%g\t%g\t%g\n",mass,limit,quantileExpected);
    t->Fill();
    limitP1file >> limit;
    quantileExpected = 0.84;
    printf("%g\t%g\t%g\n",mass,limit,quantileExpected);
    t->Fill();
    limitP2file >> limit;
    quantileExpected = 0.975;
    printf("%g\t%g\t%g\n",mass,limit,quantileExpected);
    t->Fill();
    limitOfile >> limit;
    quantileExpected = -1;
    printf("%g\t%g\t%g\n",mass,limit,quantileExpected);
    t->Fill();
   
   }

  t->Write();
  f->Close();
}
