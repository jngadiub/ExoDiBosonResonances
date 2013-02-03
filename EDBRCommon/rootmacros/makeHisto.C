#include "makeHisto.h"

EDBRHistoMaker::EDBRHistoMaker(TTree* tree, 
			       bool wantElectrons,
			       bool wantMuons,
			       bool wantSideband,
			       bool wantSignal,
			       int wantNXJets){
  fChain = 0;
  nVars = 77;
  
  // Definition of regions
  sidebandVHMassLow_  =  0.0;  // GeV
  sidebandVHMassHigh_ =  70.0; // GeV
  signalVHMassLow_    =  70.0; // GeV
  signalVHMassHigh_   = 105.0; // GeV

  // Which category do we want to analyze?
  wantElectrons_ = wantElectrons;
  wantMuons_ = wantMuons;
  wantSideband_ = wantSideband;
  wantSignal_ = wantSignal;
  wantNXJets_ = wantNXJets;

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
  char buffer2[256];

  for(int i = 0; i!= nVars; ++i) {
    sprintf(buffer,"h_%s",vars[i].c_str());
    sprintf(buffer2,"%s;%s;Number of events;",vars[i].c_str(),vars[i].c_str());
    TH1D* histogram = new TH1D(buffer,
			       buffer2,
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

bool EDBRHistoMaker::eventPassesFlavorCut(){
  bool passesFlavour = ((lep == 0 and wantElectrons_) or
			(lep == 1 and wantMuons_));
  
 return passesFlavour;
}

bool EDBRHistoMaker::eventInSidebandRegion(int i){

 bool isInSideband = (mJJ[i] > sidebandVHMassLow_ and 
		      mJJ[i] < sidebandVHMassHigh_); 
 
 return isInSideband;
}

bool EDBRHistoMaker::eventInSignalRegion(int i){
 bool isInSignal = (mJJ[i] > signalVHMassLow_ and 
		     mJJ[i] < signalVHMassHigh_);

 return isInSignal;
}

bool EDBRHistoMaker::eventPassesRegionCut(int i){
   bool isInSideband = eventInSidebandRegion(i);
   bool isInSignal =eventInSignalRegion(i);
   bool passesRegion = ((isInSideband and wantSideband_) or
		       (isInSignal and wantSignal_));

 return passesRegion;
}

bool  EDBRHistoMaker::eventPassesNXJetCut(int i){
  bool passesNXJ = (nXjets[i] == wantNXJets_);
  return passesNXJ;
  
}

bool EDBRHistoMaker::eventPassesCut(int i) {

  bool passesFlavour = eventPassesFlavorCut();
  bool passesRegion  = eventPassesRegionCut(i);
  bool passesNXJet   = eventPassesNXJetCut(i);
  bool result = passesFlavour && passesRegion && passesNXJet;

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
    if(jentry%10000==0)
      printf("Entry number %i...\n",(int)jentry);

    // We calculate a weight here.
    //double actualWeight = weight;//*HLTweight*PUweight*LumiWeight*GenWeight;
    double actualWeight = PUweight*LumiWeight*GenWeight;

    //printf("Actual weight is %g\n",actualWeight);
    // We get the histogram from the map by string and fill it.
    // We could wrap all the fills in the this->eventPassesCut()
    // to fill histograms only for the events which pass a given
    // cut (Sideband / SignalRegion, Muon / Electron, 
    // Single / Double jet ...) 

    (theHistograms["nCands"])->Fill(nCands,actualWeight);

    for(int ivec=0;ivec<nCands;ivec++){
      
      if(eventPassesCut(ivec)){
	//printf("Event passed\n");
	(theHistograms["ptlep1"])->Fill(ptlep1[ivec],actualWeight);
	(theHistograms["ptlep2"])->Fill(ptlep2[ivec],actualWeight);
	(theHistograms["ptjet1"])->Fill(ptjet1[ivec],actualWeight);
	(theHistograms["ptjet2"])->Fill(ptjet2[ivec],actualWeight);
	(theHistograms["mLL"])->Fill(mLL[ivec],actualWeight);
	(theHistograms["mJJ"])->Fill(mJJ[ivec],actualWeight);
	(theHistograms["mZZ"])->Fill(mZZ[ivec],actualWeight);
	(theHistograms["prunedmass"])->Fill(prunedmass[ivec],actualWeight);
	(theHistograms["mdrop"])->Fill(mdrop[ivec],actualWeight);
	(theHistograms["mJJNoKinFit"])->Fill(mJJNoKinFit[ivec],actualWeight);
	(theHistograms["nsubj12"])->Fill(nsubj12[ivec],actualWeight);
	(theHistograms["nVtx"])->Fill(nVtx,actualWeight);
	(theHistograms["betajet1"])->Fill(betajet1[ivec],actualWeight);
	(theHistograms["isolep1"])->Fill(isolep1[ivec],actualWeight);
	(theHistograms["isolep2"])->Fill(isolep2[ivec],actualWeight);
	(theHistograms["met"])->Fill(met,actualWeight);
	(theHistograms["metSign"])->Fill(metSign,actualWeight);
	(theHistograms["etalep1"])->Fill(etalep1[ivec],actualWeight);
	(theHistograms["etalep2"])->Fill(etalep2[ivec],actualWeight);
	(theHistograms["etajet1"])->Fill(etajet1[ivec],actualWeight);
	(theHistograms["etajet2"])->Fill(etajet2[ivec],actualWeight);
	//	(theHistograms[""])->Fill([ivec],actualWeight);
	
      }//end if eventPassesCut
    }//end loop over nCands
  }//end loop over entries
  
  this->saveAllHistos(outFileName);
}
