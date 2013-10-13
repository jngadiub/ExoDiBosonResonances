
//merge SingEle and SingMu, remove overlap

void mergeDATA(TString Tree="/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv7_newMJ/fullallrange")
{

	TFile * outputfile = new TFile(Tree+"/treeEDBR_data_xww.root","RECREATE");

	TString treename;
	treename = "SelectedCandidates";

	TChain * chain_SingleEle = new TChain(treename);
	TChain * chain_SingleMu = new TChain(treename);
/*
	chain_SingleEle->Add(Tree+"/"+"treeEDBR_SingleElectron_Run2012A_13Jul2012_xww.root");
	chain_SingleEle->Add(Tree+"/"+"treeEDBR_SingleElectron_Run2012A_recover_xww.root");
	chain_SingleEle->Add(Tree+"/"+"treeEDBR_SingleElectron_Run2012B_13Jul2012_xww.root");
	chain_SingleEle->Add(Tree+"/"+"treeEDBR_SingleElectron_Run2012C_24Aug2012_xww.root");
	chain_SingleEle->Add(Tree+"/"+"treeEDBR_SingleElectron_Run2012C_EcalRecove_xww.root");
	chain_SingleEle->Add(Tree+"/"+"treeEDBR_SingleElectron_Run2012C_PromptReco_xww.root");
	chain_SingleEle->Add(Tree+"/"+"treeEDBR_SingleElectron_Run2012D_PromptReco_xww.root");

	chain_SingleMu->Add(Tree+"/"+"treeEDBR_SingleMu_Run2012A_13Jul2012_xww.root");
	chain_SingleMu->Add(Tree+"/"+"treeEDBR_SingleMu_Run2012A_recover_xww.root");
	chain_SingleMu->Add(Tree+"/"+"treeEDBR_SingleMu_Run2012B_13Jul2012_xww.root");
	chain_SingleMu->Add(Tree+"/"+"treeEDBR_SingleMu_Run2012C_24Aug2012_xww.root");
	chain_SingleMu->Add(Tree+"/"+"treeEDBR_SingleMu_Run2012C_EcalRecove_xww.root");	
	chain_SingleMu->Add(Tree+"/"+"treeEDBR_SingleMu_Run2012C_PromptReco_xww.root");	
	chain_SingleMu->Add(Tree+"/"+"treeEDBR_SingleMu_Run2012D_PromptReco_xww.root");
*/

	chain_SingleMu->Add(Tree+"/"+"treeEDBR_SingleMu_Run2012A-22Jan2013_xww.root");
	chain_SingleMu->Add(Tree+"/"+"treeEDBR_SingleMu_Run2012B-22Jan2013_xww.root");
	chain_SingleMu->Add(Tree+"/"+"treeEDBR_SingleMu_Run2012C-22Jan2013_xww.root");
	chain_SingleMu->Add(Tree+"/"+"treeEDBR_SingleMu_Run2012D-22Jan2013_xww.root");	

	chain_SingleEle->Add(Tree+"/"+"treeEDBR_SingleElectron_Run2012A-22Jan2013_xww.root");
	chain_SingleEle->Add(Tree+"/"+"treeEDBR_SingleElectron_Run2012B-22Jan2013_xww.root");
	chain_SingleEle->Add(Tree+"/"+"treeEDBR_SingleElectron_Run2012C-22Jan2013_xww.root");
	chain_SingleEle->Add(Tree+"/"+"treeEDBR_SingleElectron_Run2012D-22Jan2013_xww.root");



	TChain * chain = new TChain(treename);
	chain->Add(chain_SingleEle);
	int nele = chain->GetEntries();
	chain->Add(chain_SingleMu);
	int ntotal = chain->GetEntries();
	int nmu  = ntotal - nele;
	cout<<"ele entries: "<<nele<<endl;
	cout<<"mu entries: "<<nmu<<endl;
	cout<<"ele+mu entries: "<<ntotal<<endl;

	TTree * outTree = chain->CloneTree(0);

	double lep[99];
	int nLooseEle;
	int nLooseMu;
	chain->SetBranchAddress("lep", lep);
	chain->SetBranchAddress("nLooseMu", &nLooseMu);
	chain->SetBranchAddress("nLooseEle",&nLooseEle);

	//fill only lep==0 for SingEle
	for(int i=0; i<nele; i++)
	{
		chain->GetEntry(i);
		if(nLooseEle+nLooseMu!=1)continue;
		if(lep[0]==0)outTree->Fill();
	}
	//fill only lep==1 for SingMu
	
	for(int i=nele; i<ntotal; i++)
	{
		chain->GetEntry(i);
		if(nLooseEle+nLooseMu!=1)continue;
		if(lep[0]==1)outTree->Fill();
	}
	

	cout<<"output entries:  "<<outTree->GetEntries()<<endl;

	outputfile->cd();
	outTree->Write();
	outputfile->Close();

}
