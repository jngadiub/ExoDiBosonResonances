import FWCore.ParameterSet.Config as cms 

WelenuDiJetFactory = cms.PSet(
       leg1Collection = cms.InputTag("WelenuCand"),
       leg2Collection = cms.InputTag("ZjjCand"),
       metCollection = cms.InputTag("")
)
WelenuDiJetKinFitFactory = WelenuDiJetFactory.clone()
WelenuDiJetKinFitFactory.leg2Collection = cms.InputTag("cmgDiJetKinFit")

cmgWelenuDiJet = cms.EDFilter(
                   "WelenuDiJetPOProducer",
                   cfg = WelenuDiJetFactory.clone(),
                   cuts = cms.PSet( preSel = cms.PSet( noOverlap  = cms.string(" deltaR(leg1.leg1.eta,leg1.leg1.phi,leg2.leg1.sourcePtr.eta,leg2.leg1.sourcePtr.phi) > 0.8 &&"
                                                                   #         +"deltaR(leg1.leg2.eta,leg1.leg2.phi,leg2.leg1.sourcePtr.eta,leg2.leg1.sourcePtr.phi) > 0.8 &&"
                                                                            +"deltaR(leg1.leg1.eta,leg1.leg1.phi,leg2.leg2.sourcePtr.eta,leg2.leg2.sourcePtr.phi) > 0.8 "),
                                                                    #        +"deltaR(leg1.leg2.eta,leg1.leg2.phi,leg2.leg2.sourcePtr.eta,leg2.leg2.sourcePtr.phi) > 0.8 "),
                                                       kinematics = cms.PSet( mass = cms.string("mass > 180") ),#XXXX
													   mergedJetID = cms.string("leg2.leg1.getSelection(\"cuts_looseJetId\") && leg2.leg1.getSelection(\"cuts_TOBTECjetsId\") && leg2.leg2.getSelection(\"cuts_looseJetId\") && leg2.leg2.getSelection(\"cuts_TOBTECjetsId\")")
                                   )    
##                     btags      = cms.PSet( btag0= cms.string("leg2.getSelection(\"cuts_btags_btag0\") "),
##                                            btag1= cms.string("leg2.getSelection(\"cuts_btags_btag1\") "),
##                                            btag2= cms.string("leg2.getSelection(\"cuts_btags_btag2\") "),
##                                           ),
    
                    )   
)   
    
cmgWelenuDiJetKinFit=cmgWelenuDiJet.clone()
cmgWelenuDiJetKinFit.cfg=WelenuDiJetKinFitFactory.clone()


WelenuDiJetEDBRFactory = cms.PSet(
       inputs = cms.InputTag("cmgWelenuDiJet"),
       vbftag = cms.InputTag("VBFPairs"),
       overlapcut = cms.string(" deltaR(vbfptr.leg1.eta,vbfptr.leg1.phi,leg2.leg1.sourcePtr.eta,leg2.leg1.sourcePtr.phi) > 0.8 &&"
                               +"deltaR(vbfptr.leg2.eta,vbfptr.leg2.phi,leg2.leg1.sourcePtr.eta,leg2.leg1.sourcePtr.phi) > 0.8 &&"
                               +"deltaR(vbfptr.leg1.eta,vbfptr.leg1.phi,leg2.leg2.sourcePtr.eta,leg2.leg2.sourcePtr.phi) > 0.8 &&"
                               +"deltaR(vbfptr.leg2.eta,vbfptr.leg2.phi,leg2.leg2.sourcePtr.eta,leg2.leg2.sourcePtr.phi) > 0.8 &&"
                               +"deltaR(vbfptr.leg1.eta,vbfptr.leg1.phi,leg1.leg1.sourcePtr.eta,leg1.leg1.sourcePtr.phi) > 0.8 &&"
                               +"deltaR(vbfptr.leg2.eta,vbfptr.leg2.phi,leg1.leg1.sourcePtr.eta,leg1.leg1.sourcePtr.phi) > 0.8 ")
                             #  +"deltaR(vbfptr.leg1.eta,vbfptr.leg1.phi,leg1.leg2.sourcePtr.eta,leg1.leg2.sourcePtr.phi) > 0.8 &&"
                             #  +"deltaR(vbfptr.leg2.eta,vbfptr.leg2.phi,leg1.leg2.sourcePtr.eta,leg1.leg2.sourcePtr.phi) > 0.8 ")
)

cmgWelenuDiJetEDBR = cms.EDFilter(
    "WelenuDiJetEDBRPOProducer",
    cfg = WelenuDiJetEDBRFactory.clone(),
    cuts = cms.PSet(  genMatch = cms.string("leg1.getSelection(\"cuts_genP\") && leg2.getSelection(\"cuts_genP\")")
                     )   
    
    )   

cmgWelenuDiJetKinFitEDBR = cmgWelenuDiJetEDBR.clone()
cmgWelenuDiJetKinFitEDBR.cfg.inputs = cms.InputTag("cmgWelenuDiJetKinFit")
