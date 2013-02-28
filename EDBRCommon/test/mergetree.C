#include "TTree.h"
#include "TFile.h"
#include "TChain.h"
#include "TString.h"
#include <iostream>


void mergetree(TString file1 = "/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv5/control/fullsig/treeEDBR_RSG_WW_lvjj_c0p2_M600_xww.root", TString file2 = "/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv5/control/fullsideband/treeEDBR_RSG_WW_lvjj_c0p2_M600_xww.root",TString outfile = "mergetreeTest.root")
{
	cout<<"file1 :"<<file1<<endl;
	cout<<"file2 :"<<file2<<endl;
	TChain * chain = new TChain("SelectedCandidates");
	chain->Add(file1);
	cout<<"file1 entries: "<<chain->GetEntries()<<endl;
	int nsig = chain->GetEntries();
	chain->Add(file2);
	cout<<"file1+file2 entries: "<<chain->GetEntries()<<endl;
	int ntotal = chain->GetEntries();
	
    TFile * outputfile = new TFile(outfile,"RECREATE");
	TTree * outTree = chain->CloneTree(0);

    int nCands;
    chain->SetBranchAddress("nCands",&nCands);

	//save events with signal region candidate for filesig
	for(int i =0; i<nsig; i++)
	{
		chain->GetEntry(i);
		if(nCands>0)outTree->Fill();//nCands could be 1 or 2 ( 1 signal region candidate with/without sideband candidate  )
	}

	//save events in sideband region, not overlaped with signal region
	for(int i =nsig; i<ntotal; i++)
	{
		chain->GetEntry(i);
		if(nCands==1)outTree->Fill();//have only one sideband candidate, to avoid overlap
	}

	cout<<"outTree entries: "<<outTree->GetEntries()<<endl;
	
	outputfile->cd();
	outTree->Write();
	outputfile->Close();
}
