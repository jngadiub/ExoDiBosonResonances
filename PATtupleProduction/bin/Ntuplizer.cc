//SingleTopEdmNtupleAnalyzer inputFiles=../test/singleTopEdmNtuple_TTbar.root outputFile=test.root outputEvery=100 maxEvents=1000

#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TTree.h>

#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

//#include "DataFormats/MuonReco/interface/Muon.h"
//#include "DataFormats/PatCandidates/interface/Muon.h"
//#include "PhysicsTools/FWLite/interface/TFileService.h"
//#include "PhysicsTools/FWLite/interface/CommandLineParser.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "PhysicsTools/UtilAlgos/interface/FWLiteAnalyzerWrapper.h"

#include "MessageTools.h"
#include "NtupleBranches.h"
#include "GenParticlesNtuplizer.h"
#include "LeptonsNtuplizer.h"
#include "JetsNtuplizer.h"
#include "IvfNtuplizer.h"

using namespace NtupleBranches;

int main(int argc, char* argv[])
{

  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  std::stringstream ss;

  if ( argc < 2 ) {
    ss.str(""); ss << "Usage : " << argv[0] << " [parameters.py]";
    STDLINE(ss.str(),ACRed);
    return 0;
  }

  const edm::ParameterSet& cfg = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("process");
  
  int			   maxEvents_	= cfg.getParameter<int>("maxEvents"  );
  unsigned int  	   outputEvery_ = cfg.getParameter<unsigned int>("outputEvery");
  std::string		   outputFile_  = cfg.getParameter<std::string>("outputFile");
  std::vector<std::string> inputFiles_  = cfg.getParameter<std::vector<std::string> >("fileNames");
      
  TFile* outFile = new TFile( outputFile_.c_str(), "RECREATE" );
  TTree* tree = new TTree( "tree", "tree" );
  branch( tree );
    
  int ievt = 0;  
  for( unsigned int iFile = 0; iFile < inputFiles_.size(); ++iFile )
  {
    STDLINE("",ACWhite);
    STDLINE("Opening input file: " + inputFiles_[iFile], ACGreen);
    STDLINE("",ACWhite);
    
    TFile* inFile = TFile::Open( inputFiles_[iFile].c_str() );
    
    if( inFile ){
            
      fwlite::Event ev( inFile );
      for ( ev.toBegin(); !ev.atEnd(); ++ev, ++ievt){
      
        edm::EventBase const & event = ev;

	if ( maxEvents_ > 0 ? ievt+1 > maxEvents_ : false ) break;
	
	if ( outputEvery_ != 0 ? (ievt > 0 && ievt%outputEvery_ == 0 ) : false ){ 
	  ss.str(""); ss << "  processing event: " << ievt;
	  STDLINE(ss.str(),ACWhite);	    
	}

        NtupleBranches::reset();
        NtupleBranches::getEventByLabels( event );
	
	JetsNtuplizer::fillJetsBranches();				
					
	tree->Fill();
	
      }
      
      inFile->Close();
    }  
  }
  
  //tree->Print();
  STDLINE("",ACWhite);
  STDLINE("Closing output file: " + outputFile_, ACGreen);
  outFile->Write();
     
}
