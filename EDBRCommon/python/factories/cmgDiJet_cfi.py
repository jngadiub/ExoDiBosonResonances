import FWCore.ParameterSet.Config as cms

diJetFactory = cms.PSet(
    leg1Collection = cms.InputTag("jetIDJet"),
    leg2Collection = cms.InputTag("jetIDJet"),
    metCollection = cms.InputTag(""),
)


#from CMGTools.Common.selections.kinematics_cfi import dijetKinematics
from ExoDiBosonResonances.EDBRCommon.selections.jetKinematics_cfi import zjj,isSignal,isSideband
cmgDiJet = cms.EDFilter(
    "DiPFJetPOProducer",
    cfg = diJetFactory.clone(),
    cuts = cms.PSet(
       zjj  = zjj.clone(),
       isSignal = isSignal.clone(),
       isSideband = isSideband.clone(),
       genP = cms.PSet(genP = cms.string("leg1.getSelection(\"cuts_genParton\") && leg2.getSelection(\"cuts_genParton\")")),
       btags= cms.PSet(btag0= cms.string("!leg1.getSelection(\"cuts_JP_loose\") "
                                         +"&& !leg2.getSelection(\"cuts_JP_loose\") "
                                         ),
                       btag1= cms.string("!( !leg1.getSelection(\"cuts_JP_loose\")"
                                         +  "&& !leg2.getSelection(\"cuts_JP_loose\") )" # not btag0
                                         +" && !(leg1.getSelection(\"cuts_JP_medium\") " 
                                         +"   && leg2.getSelection(\"cuts_JP_loose\") )" # not btag2a
                                         +" && !(leg1.getSelection(\"cuts_JP_loose\")"
                                         +"   && leg2.getSelection(\"cuts_JP_medium\") )"# not btag2b
                                         ),
                       btag2= cms.string("  (leg1.getSelection(\"cuts_JP_medium\")  "
                                         +"&&leg2.getSelection(\"cuts_JP_loose\") )"
                                         +"||(leg1.getSelection(\"cuts_JP_loose\") "
                                         +"&& leg2.getSelection(\"cuts_JP_medium\") )"
                                         ),
                       ),
       ),
    verbose = cms.untracked.bool( False )
)

VBFPairsAll = cms.EDFilter("DiPFJetPOProducer",
                        cfg = cms.PSet(leg1Collection = cms.InputTag("vbfJet"),
                                       leg2Collection = cms.InputTag("vbfJet"),
                                       metCollection = cms.InputTag(""),
                                       ),
                        cuts = cms.PSet( vbf   = cms.PSet(deta = cms.string("abs(leg1.eta - leg2.eta) > 4 "),
                                                          mass = cms.string("mass > 0") #XXXX for optimzation studies
                                                          ),
                                         ),
                        verbose = cms.untracked.bool( False )
                        )

VBFPairs = cms.EDFilter("DiJetSelector",  #created
                          src = cms.InputTag("VBFPairsAll"),
                          cut = cms.string( "getSelection(\"cuts_vbf\")" ) #created
                          )


diJetSequence = cms.Sequence(cmgDiJet + VBFPairsAll + VBFPairs)

