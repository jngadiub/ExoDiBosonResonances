#include "makeHisto.h"

EDBRHistoMaker::EDBRHistoMaker(TTree* tree, 
			       bool wantElectrons,
			       bool wantMuons,
			       bool wantSideband,
			       bool wantSignal){
  fChain = 0;
  nVars = 77;
  
  // Definition of regions
  sidebandVHMassLow_  =  50.0; // GeV
  sidebandVHMassHigh_ =  70.0; // GeV
  signalVHMassLow_    =  70.0; // GeV
  signalVHMassHigh_   = 100.0; // GeV

  // Which category do we want to analyze?
  wantElectrons_ = wantElectrons;
  wantMuons_ = wantMuons;
  wantSideband_ = wantSideband;
  wantSignal_ = wantSignal;
  
  Init(tree);
  createAllHistos();
  printAllHistos();
}

EDBRHistoMaker::~EDBRHistoMaker() {
  if (!fChain) return;                                                                                                                                     
  delete fChain->GetCurrentFile();
}                  

Int_t EDBRHistoMaker::GetEntry(Long64_t entry) {
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}

Long64_t EDBRHistoMaker::LoadTree(Long64_t entry) {
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
  }
  return centry;
}

//-------------------------
// Infrastructure functions
//-------------------------

void EDBRHistoMaker::createAllHistos() {
  char buffer[256];

  for(int i = 0; i!= nVars; ++i) {
    sprintf(buffer,"h_%s",vars[i].c_str());
    TH1D* histogram = new TH1D(buffer,
			       vars[i].c_str(),
			       nBins[i],
			       minBin[i],
			       maxBin[i]);
    histogram->SetDirectory(0);
    histogram->Sumw2();
    theHistograms[vars[i]] = histogram;
  }
  
}

void EDBRHistoMaker::printAllHistos() {
  printf("We have %i histograms\n",int(theHistograms.size()));
}

void EDBRHistoMaker::saveAllHistos(std::string outFileName) {
  TFile* outFile = TFile::Open(outFileName.c_str(),"RECREATE");

  for(int i = 0; i!=nVars; ++i) {
    std::string name = vars[i];
    const TH1D* thisHisto = this->theHistograms[name];
    thisHisto->Write();
  }

  outFile->Close();
}

//------------------
// Physics functions
//------------------

bool EDBRHistoMaker::eventPassesCut() {
  bool passesFlavour = ((lep == 0 and wantElectrons_) or
			(lep == 1 and wantMuons_));

  // TODO: check the numbers here
  bool isInSideband = (mJJ[0] > 50 and mJJ[0] < 70); 
  bool isInSignal = (mJJ[0] > 70 and mJJ[0] < 110);
  bool passesRegion = ((isInSideband and wantSideband_) or
		       (isInSignal and wantSignal_));

  bool result = passesFlavour and passesRegion;
  
  return result;
}

///----------------------------------------------------------------
/// This is the important function, the loop over all events.
/// Here we fill the histograms according to cuts, weights,
/// and can also filter out events on an individual basis.
///----------------------------------------------------------------
void EDBRHistoMaker::Loop(std::string outFileName){
  
  if (fChain == 0) return;
  
  Long64_t nentries = fChain->GetEntriesFast();
  
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    
    // We calculate a weight here.
    // TODO: check if this is really the weight we want to use!
    double actualWeight = weight*HLTweight*PUweight*LumiWeight*GenWeight;
    
    // We get the histogram from the map by string and fill it.
    // We could wrap all the fills in the this->eventPassesCut() function
    // 
    (theHistograms["nCands"])->Fill(nCands,actualWeight);
    (theHistograms["ptlep1"])->Fill(ptlep1[0],actualWeight);
    (theHistograms["ptlep2"])->Fill(ptlep2[0],actualWeight);
    (theHistograms["ptjet1"])->Fill(ptjet1[0],actualWeight);
    (theHistograms["ptjet2"])->Fill(ptjet2[0],actualWeight);
    
    (theHistograms["mLL"])->Fill(mLL[0],actualWeight);
    (theHistograms["mJJ"])->Fill(mJJ[0],actualWeight);
    (theHistograms["mZZ"])->Fill(mZZ[0],actualWeight);
    (theHistograms["prunedmass"])->Fill(prunedmass[0],actualWeight);
    (theHistograms["mdrop"])->Fill(mdrop[0],actualWeight);
  }

  this->saveAllHistos(outFileName);
}
