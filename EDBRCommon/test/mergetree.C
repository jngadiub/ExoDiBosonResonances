#include "TTree.h"
#include "TFile.h"
#include "TChain.h"
#include "TString.h"
#include <iostream>


void mergetree(TString file1 = "/afs/cern.ch/work/s/shuai/public/diboson/trees/test/testnewsh/fullsig/treeEDBR_TTBAR_xww.root", TString file2 = "/afs/cern.ch/work/s/shuai/public/diboson/trees/test/testnewsh/fullsideband/treeEDBR_TTBAR_xww.root",TString outfile = "mergetreeTest.root")
{
	cout<<"file1 :"<<file1<<endl;
	cout<<"file2 :"<<file2<<endl;
	TChain * chain = new TChain("SelectedCandidates");
	chain->Add(file1);
	int nsig = chain->GetEntries();
	chain->Add(file2);
	int ntotal = chain->GetEntries();
	int nside = ntotal-nsig;
	cout<<"file1 entries: "<<nsig<<endl;
	cout<<"file2 entries: "<<nside<<endl;
	cout<<"file1+file2 entries: "<<ntotal<<endl;	
	
    TFile * outputfile = new TFile(outfile,"RECREATE");
	TTree * outTree = chain->CloneTree(0);

    int nCands;
	//int nEvt;
    chain->SetBranchAddress("nCands",&nCands);
    //chain->SetBranchAddress("nEvt",&nEvt);

	//save events with signal region candidate for filesig
	for(int i =0; i<nsig; i++)
	{
		if(i%100==0)cout<<i<<endl;
		//if(i>1000)break;
		chain->GetEntry(i);
		//cout<<nEvt<<endl;
		//chain->GetEntry(nsig+i);
		//cout<<nEvt<<endl;
		
		if(nCands>0)outTree->Fill();//this event have signal region candidate ( 2 means there are Wmunu and Welenu. This will be filtered by loose lepton veto  )
		else
		{
			int j = nsig+i;//corresponding sideband event
			chain->GetEntry(j);
			if(nCands>0)outTree->Fill();
		}
		
	}


	cout<<"outTree entries: "<<outTree->GetEntries()<<endl;
	
	outputfile->cd();
	outTree->Write();
	outputfile->Close();
}
