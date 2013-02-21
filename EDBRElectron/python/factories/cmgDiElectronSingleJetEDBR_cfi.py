import FWCore.ParameterSet.Config as cms
#from CMGTools.Common.factories.cmgMuon_cfi import cmgMuon

diElectronVJetEDBRFactory = cms.PSet(
       inputs = cms.InputTag("cmgDiElectronVJet"),
       vbftag = cms.InputTag("VBFPairs"),
       overlapcut = cms.string(" deltaR(vbfptr.leg1.eta,vbfptr.leg1.phi,leg2.eta,leg2.phi) > 0.5 &&"
                               +"deltaR(vbfptr.leg2.eta,vbfptr.leg2.phi,leg2.eta,leg2.phi) > 0.5 &&"
                               +"deltaR(vbfptr.leg1.eta,vbfptr.leg1.phi,leg1.leg1.sourcePtr.eta,leg1.leg1.sourcePtr.phi) > 0.5 &&"
                               +"deltaR(vbfptr.leg2.eta,vbfptr.leg2.phi,leg1.leg1.sourcePtr.eta,leg1.leg1.sourcePtr.phi) > 0.5 &&"
                               +"deltaR(vbfptr.leg1.eta,vbfptr.leg1.phi,leg1.leg2.sourcePtr.eta,leg1.leg2.sourcePtr.phi) > 0.5 &&"
                               +"deltaR(vbfptr.leg2.eta,vbfptr.leg2.phi,leg1.leg2.sourcePtr.eta,leg1.leg2.sourcePtr.phi) > 0.5 ")
)

#from CMGTools.Common.selections.zmumu_cfi import zmumu
cmgDiElectronVJetEDBR = cms.EDFilter(
    "DiElectronSingleJetEDBRPOProducer",
    cfg = diElectronVJetEDBRFactory.clone(),
    cuts = cms.PSet( genMatch = cms.PSet(genMatch = cms.string("leg1.getSelection(\"cuts_genP\") && leg2.getSelection(\"cuts_genP\")"))
                     )
    
    )
