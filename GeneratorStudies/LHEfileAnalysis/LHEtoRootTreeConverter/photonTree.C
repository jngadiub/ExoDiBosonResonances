#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include "TFile.h"
#include "TList.h"
#include "TTree.h"
#include "TH1D.h"
#include "TLorentzVector.h"

using namespace std;

void photonTree(string filename){
  // DEFINITION OF INPUT FILE
  ifstream fin;
  string filenameT = filename + ".txt";
  cout<<"Processing "<<filenameT<<endl;
  fin.open(filenameT.c_str());

  // MAXIMUM NUMBER OF EVENTS PROCESSED
  int maxEvents = 3000000;

  //DEFINITION OF OUTPUT FILE
  char oname[192];
  sprintf(oname,"%s.root",filename.c_str());
  cout<<oname<<endl;
  TFile fout(oname, "RECREATE");
  TTree* tree = new TTree("variables","variables");
  
  //DEFINITION OF TREE VARIABLES
  Double_t m_costhetastar, m_ggmass, m_ggmass_gen, m_ggY, m_ggpt, m_ggY_gen, m_ggpt_gen,  
    m_etag1, m_etag2, m_etag1_gen, m_etag2_gen, m_ptg1, m_ptg2, m_ptg1_gen, m_ptg2_gen,;
    
  tree->Branch("ggmass", &m_ggmass, "ggmass/D");
  tree->Branch("etag1", &m_etag1, "etag1/D");
  tree->Branch("etag2", &m_etag2, "etag2/D");
  tree->Branch("ptg1", &m_ptg1, "ptg1/D");
  tree->Branch("ptg2", &m_ptg2, "ptg2/D");
  tree->Branch("ggY",&m_ggY,"ggY/D");
  tree->Branch("ggpt",&m_ggpt,"ggpt/D");
 
  // READING THE INPUT FILE
  int ctr=0;
  while (!fin.eof() && fin.good()){
    //cout<<"new event"<<endl;
    int idup[2],istup[2],mothup[2][2],icolup[2][2];
    double pup[2][5],vtimup[2],spinup[2];
    for (int a=0; a<2;a++){
      fin >> idup[a] >> istup[a] >> mothup[a][0] >> mothup[a][1] >> icolup[a][0] >> icolup[a][1];
      for (int i=0;i<5;i++){
	fin>>pup[a][i];
      }
      fin >> vtimup[a]>>spinup[a];
    }
 
    //STORAGE OF INFORMATION INTO THE TREE
    TLorentzVector photon1(pup[0][0],pup[0][1],pup[0][2],pup[0][3]);
    TLorentzVector photon2(pup[1][0],pup[1][1],pup[1][2],pup[1][3]);
     m_etag1= photon1.Eta();
     m_etag2= photon2.Eta();
     m_ptg1= photon1.Pt();
     m_ptg2= photon2.Pt();

    double costhetastar,phi1,phi2;

    TLorentzVector Graviton = photon1+photon2;
    m_ggY = Graviton.Rapidity();
    m_ggpt = Graviton.Pt();
    m_ggmass=Graviton.M();
    tree->Fill();

    ctr++;
    if (ctr%1000 == 0) std::cout << "event number: " << ctr << std::endl;
    if (ctr == maxEvents) break;
    }
  fout.ls();
  fout.cd();
  tree->Write();
  fout.Close();
}
