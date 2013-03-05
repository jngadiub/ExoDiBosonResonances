import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgBaseJet_cfi import baseJetFactory

pfJetFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatJets"), #customPFJetsNoPUSub
       baseJetFactory = baseJetFactory.clone(),
       useConstituents=cms.bool(False),
       puVariables=cms.InputTag("puJetId"), #puJetIdAK5NoPUSub
       puMvas=cms.VInputTag(
                            "puJetMvaCustom:fullDiscriminant", #puJetMvaAK5NoPUSub
                            "puJetMvaCustom:simpleDiscriminant"
                            #"puJetMvaCustom:cutbasedDiscriminant",
                            ),
       puIds=cms.VInputTag(
                           "puJetMvaCustom:fullId",
                           "puJetMvaCustom:simpleId"
                           #"puJetMvaCustom:cutbasedId",
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


from ExoDiBosonResonances.EDBRCommon.selections.jetKinematics_cfi import mergedJetKinematics, mergedJetVTagging
from ExoDiBosonResonances.EDBRCommon.selections.jetKinematics_cfi import isMergedSideband, isMergedSignal
from ExoDiBosonResonances.EDBRCommon.selections.jetKinematics_cfi import isMergedWSideband, isMergedWSignal

structJetFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatJets"),
       prunedJetCollection=cms.InputTag("selectedPatJetsPruned"),
       baseJetFactory = baseJetFactory.clone(),
       pfJetFactory = pfJetFactory.clone(),
       useConstituents=cms.bool(False),
       puVariables=cms.InputTag("puJetId"), #puJetIdAK5NoPUSub
       puMvas=cms.VInputTag("puJetMvaCustom:cutbasedDiscriminant",
                            "puJetMvaCustom:simpleDiscriminant", #puJetMvaAK5NoPUSub
                            "puJetMvaCustom:fullDiscriminant"
                            ),
       puIds=cms.VInputTag("puJetMvaCustom:cutbasedId",
                           "puJetMvaCustom:simpleId",
                           "puJetMvaCustom:fullId",                           
                           ),
       verbose = cms.untracked.bool( True )
       )

cmgStructuredJet = cms.EDFilter(
    "VJetPOProducer", 
    cfg = structJetFactory.clone(),
 #   cfg.prunedJetCollection=cms.InputTag("selectedPatJetsPruned"),
    cuts = cms.PSet(
 #       btag = trackCountingHighEffBJetTags.clone(),
 #       TCHE = trackCountingHighEffBJetTags.clone(),
 #       JP = jetProbabilityBJetTags.clone(),
 #       CSV = combinedSecondaryVertexBJetTags.clone(),
    jetKinematics = jetKinematics.clone(),
    mergedJetKinematics = mergedJetKinematics.clone(),
    looseJetId = looseJetId.clone(),
    mergedJetVTagging = mergedJetVTagging.clone(),
    isZSignal = isMergedSignal.clone(),
    isZSideband = isMergedSideband.clone(),
    isWSignal = isMergedWSignal.clone(),
    isWSideband = isMergedWSideband.clone(),
    genP = cms.PSet( genLepton = cms.string("sourcePtr().get().hasOverlaps('genJets')"))
 #      recoLepton = cms.PSet( recoLepton = cms.string("sourcePtr().get().hasOverlaps('recoLeptons')")),
       )
)


