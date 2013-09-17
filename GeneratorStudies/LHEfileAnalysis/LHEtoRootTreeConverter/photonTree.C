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
  Double_t m_costhetastar, m_compHmass, m_compHmass_gen, m_compHY, m_compHpt, m_compHY_gen, m_compHpt_gen,  
    m_etaZ, m_etaH, m_rapidityZ, m_rapidityH, m_etaZ_gen, m_etaH_gen, m_ptZ, m_ptH, m_ptZ_gen, m_ptH_gen,
    m_etaZ1, m_etaZ2, m_etaZ1_gen, m_etaZ2_gen, m_ptZ1, m_ptZ2, m_ptZ1_gen, m_ptZ2_gen,
    m_etaH1, m_etaH2, m_etaH1_gen, m_etaH2_gen, m_ptH1, m_ptH2, m_ptH1_gen, m_ptH2_gen,
    m_phiH, m_phiZ, m_mZ, m_mH, m_deltaR_H1H2, m_deltaR_Z1Z2;
  //Double_t m_pxZ1, m_pxZ2,m_pyZ1, m_pyZ2, m_pzZ1, m_pzZ2, m_EZ1, m_EZ2, m_pxH1, m_pxH2,m_pyH1, m_pyH2, m_pzH1, m_pzH2, m_EH1, m_EH2, m_pxZ, m_pxH, m_pyZ, m_pyH, m_pzZ, m_pzH, m_EZ, m_EH, m_pxcompH, m_pycompH, m_pzcompH, m_EcompH;

  int m_pdgId_Zdaughters, m_pdgId_Hdaughters,;

  tree->Branch("compHmass", &m_compHmass, "compHmass/D");
  tree->Branch("compHrapidity",&m_compHY,"compHY/D");
  tree->Branch("compHpt",&m_compHpt,"compHpt/D");
  tree->Branch("mZ", &m_mZ, "mZ/D");
  tree->Branch("mH", &m_mH, "mH/D");
  tree->Branch("ptZ", &m_ptZ, "ptZ/D");
  /*  tree->Branch("pxZ", &m_pxZ, "pxZ/D");
  tree->Branch("pyZ", &m_pyZ, "pyZ/D");
  tree->Branch("pzZ", &m_pzZ, "pzZ/D");
  tree->Branch("EZ", &m_EZ, "EZ/D");*/
  tree->Branch("ptH", &m_ptH, "ptH/D");
  /*  tree->Branch("pxH", &m_pxH, "pxH/D");
  tree->Branch("pyH", &m_pyH, "pyH/D");
  tree->Branch("pzH", &m_pzH, "pzH/D");
  tree->Branch("EH", &m_EH, "EH/D");*/
  tree->Branch("etaZ", &m_etaZ, "etaZ/D");
  tree->Branch("etaH", &m_etaH, "etaH/D");
  tree->Branch("rapidityZ", &m_rapidityZ, "rapidityZ/D");
  tree->Branch("rapidityH", &m_rapidityH, "rapidityH/D");
  tree->Branch("phiZ", &m_phiZ, "phiZ/D");
  tree->Branch("phiH", &m_phiH, "phiH/D");
  tree->Branch("ptZ1", &m_ptZ1, "ptZ1/D");
  /*  tree->Branch("pxZ1", &m_pxZ1, "pxZ1/D");
  tree->Branch("pyZ1", &m_pyZ1, "pyZ1/D");
  tree->Branch("pzZ1", &m_pzZ1, "pzZ1/D");
  tree->Branch("EZ1", &m_EZ1, "EZ1/D");*/
  tree->Branch("ptZ2", &m_ptZ2, "ptZ2/D");
  /*  tree->Branch("pxZ2", &m_pxZ2, "pxZ2/D");
  tree->Branch("pyZ2", &m_pyZ2, "pyZ2/D");
  tree->Branch("pzZ2", &m_pzZ2, "pzZ2/D");
  tree->Branch("EZ2", &m_EZ2, "EZ2/D");*/
  tree->Branch("etaZ1", &m_etaZ1, "etaZ1/D");
  tree->Branch("etaZ2", &m_etaZ2, "etaZ2/D");
  tree->Branch("ptH1", &m_ptH1, "ptH1/D");
  /*  tree->Branch("pxH1", &m_pxH1, "pxH1/D");
  tree->Branch("pyH1", &m_pyH1, "pyH1/D");
  tree->Branch("pzH1", &m_pzH1, "pzH1/D");
  tree->Branch("EH1", &m_EH1, "EH1/D");
  tree->Branch("ptH2", &m_ptH2, "ptH2/D");
  tree->Branch("pxH2", &m_pxH2, "pxH2/D");
  tree->Branch("pyH2", &m_pyH2, "pyH2/D");
  tree->Branch("pzH2", &m_pzH2, "pzH2/D");
  tree->Branch("EH2", &m_EH2, "EH2/D");*/
  tree->Branch("etaH1", &m_etaH1, "etaH1/D");
  tree->Branch("etaH2", &m_etaH2, "etaH2/D");
  tree->Branch("deltaR_H1H2", &m_deltaR_H1H2, "deltaR_H1H2/D");
  tree->Branch("deltaR_Z1Z2", &m_deltaR_Z1Z2, "deltaR_Z1Z2/D");
  tree->Branch("PdgId_Zdaughters", &m_pdgId_Zdaughters, "PdgId_Zdaughters/I");
  tree->Branch("PdgId_Hdaughters", &m_pdgId_Hdaughters, "PdgId_Hdaughters/I");

  // READING THE INPUT FILE                          
  int ctr=0;

  while (!fin.eof() && fin.good()){
    //cout<<"new event"<<endl;
    int idup[7],istup[7],mothup[7][2],icolup[7][2];
    double pup[7][5],vtimup[7],spinup[7];

    for (int a=0; a<7;a++){
      fin >> idup[a] >> istup[a] >> mothup[a][0] >> mothup[a][1] >> icolup[a][0] >> icolup[a][1];
      for (int i=0;i<5;i++){
	fin>>pup[a][i];
      }
      fin >> vtimup[a]>>spinup[a];
    }
   

    //STORAGE OF INFORMATION INTO THE TREE
    TLorentzVector compH(pup[0][0],pup[0][1],pup[0][2],pup[0][3]);
    TLorentzVector H(pup[1][0],pup[1][1],pup[1][2],pup[1][3]);
    TLorentzVector Z(pup[2][0],pup[2][1],pup[2][2],pup[2][3]);
    
    TLorentzVector H1(pup[3][0],pup[3][1],pup[3][2],pup[3][3]);	
    TLorentzVector H2(pup[4][0],pup[4][1],pup[4][2],pup[4][3]);   
    TLorentzVector Z1(pup[5][0],pup[5][1],pup[5][2],pup[5][3]);
    TLorentzVector Z2(pup[6][0],pup[6][1],pup[6][2],pup[6][3]);

    //TLorentzVector Z = Z1+Z2;
    //TLorentzVector H = H1+H2;
    //TLorentzVector compH = Z+H;
     m_etaZ= Z.Eta();
     m_etaH= H.Eta();
     m_rapidityZ= Z.Rapidity();
     m_rapidityH= H.Rapidity();
     m_ptZ= Z.Pt();
     m_ptH= H.Pt();
     m_mH=H.M();
     m_mZ=Z.M();
     m_phiH=H.Phi();
     m_phiZ=Z.Phi();
     m_ptZ1= Z1.Pt();
     m_ptZ2= Z2.Pt();
     m_etaZ1= Z1.Eta();
     m_etaZ2= Z2.Eta();
     m_ptH1= H1.Pt();
     m_ptH2= H2.Pt();
     m_etaH1= H1.Eta();
     m_etaH2= H2.Eta();

     /*m_pxZ1=Z1.Px();
     m_pxZ2=Z2.Px();
     m_pxH1=H1.Px();
     m_pxH2=H2.Px();
     m_pxZ=Z.Px();
     m_pxH=H.Px();
     m_pyZ1=Z1.Py();
     m_pyZ2=Z2.Py();
     m_pyH1=H1.Py();
     m_pyH2=H2.Py();
     m_pyZ=Z.Py();
     m_pyH=H.Py();
     m_pzZ1=Z1.Pz();
     m_pzZ2=Z2.Pz();
     m_pzH1=H1.Pz();
     m_pzH2=H2.Pz();
     m_pzZ=Z.Pz();
     m_pzH=H.Pz();
     m_EZ1=Z1.E();
     m_EZ2=Z2.E();
     m_EH1=H1.E();
     m_EH2=H2.E();
     m_EZ=Z.E();
     m_EH=H.E();*/

     m_deltaR_Z1Z2=Z1.DeltaR(Z2);
     m_deltaR_H1H2=H1.DeltaR(H2);
     m_pdgId_Zdaughters=abs(idup[5]);
     m_pdgId_Hdaughters=abs(idup[3]);

    double costhetastar,phi1,phi2;

    m_compHY = compH.Rapidity();
    m_compHpt = compH.Pt();
    m_compHmass=compH.M();
    tree->Fill();

    ctr++;
    //cout <<  "event number: " << ctr <<endl;
    if (ctr%1000 == 0) std::cout << "event number: " << ctr << std::endl;
    if (ctr == maxEvents) break;
    }
  fout.ls();
  fout.cd();
  tree->Write();
  fout.Close();
}
