import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgBaseJet_cfi import baseJetFactory

pfJetFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatJets"), #customPFJetsNoPUSub
       baseJetFactory = baseJetFactory.clone(),
       useConstituents=cms.bool(False),
       puVariables=cms.InputTag("puJetId"), #puJetIdAK5NoPUSub
       puMvas=cms.VInputTag("puJetMvaCustom:cutbasedDiscriminant",
                            "puJetMvaCustom:fullDiscriminant", #puJetMvaAK5NoPUSub
                            "puJetMvaCustom:simpleDiscriminant"
                            ),
       puIds=cms.VInputTag("puJetMvaCustom:cutbasedId",
                           "puJetMvaCustom:fullId",
                           "puJetMvaCustom:simpleId",
                           
                           )
       )

#pfJetFactory.baseJetFactory.inputCollection  = cms.InputTag("customJets")
#baseJetFactory.inputCollection  = cms.InputTag("customJets")

from CMGTools.Common.selections.btaggedjet_cfi import * 
from ExoDiBosonResonances.EDBRCommon.selections.jetKinematics_cfi import jetKinematics
from ExoDiBosonResonances.EDBRCommon.selections.jetid_cfi import *

cmgJet = cms.EDFilter(
    "PFJetPOProducer", 
    cfg = pfJetFactory.clone(), 
    cuts = cms.PSet(
 #       btag = trackCountingHighEffBJetTags.clone(),
 #       TCHE = trackCountingHighEffBJetTags.clone(),
 #       JP = jetProbabilityBJetTags.clone(),
 #       CSV = combinedSecondaryVertexBJetTags.clone(),
       jetKinematics = jetKinematics.clone(),
       looseJetId = looseJetId.clone(),
 #      genLepton = cms.PSet( genLepton = cms.string("sourcePtr().get().hasOverlaps('genLeptons')")),
 #      recoLepton = cms.PSet( recoLepton = cms.string("sourcePtr().get().hasOverlaps('recoLeptons')")),
       ),    
    verbose = cms.untracked.bool( False )
)


