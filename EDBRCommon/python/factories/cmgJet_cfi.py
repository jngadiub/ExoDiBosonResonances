import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgBaseJet_cfi import baseJetFactory

pfJetFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatJets"), #customPFJetsNoPUSub
       baseJetFactory = baseJetFactory.clone(),
       useConstituents=cms.bool(False),
       puVariables=cms.InputTag("puJetId"), #puJetIdAK5NoPUSub
       puMvas=cms.VInputTag(
                            "puJetMvaCA8CHS:fullDiscriminant", #puJetMvaAK5NoPUSub
                            "puJetMvaCA8CHS:simpleDiscriminant"
                            #"puJetMvaCA8CHS:cutbasedDiscriminant",
                            ),
       puIds=cms.VInputTag(
                           "puJetMvaCA8CHS:fullId",
                           "puJetMvaCA8CHS:simpleId"
                           #"puJetMvaCA8CHS:cutbasedId",
                           )
       )

#pfJetFactory.baseJetFactory.inputCollection  = cms.InputTag("customJets")
#baseJetFactory.inputCollection  = cms.InputTag("customJets")

from CMGTools.Common.selections.btaggedjet_cfi import * 
from ExoDiBosonResonances.EDBRCommon.selections.jetKinematics_cfi import jetKinematics
from ExoDiBosonResonances.EDBRCommon.selections.jetid_cfi import *

cmgJet = cms.EDFilter(
    "PFJetSmearPOProducer", 
    cfg =  cms.PSet( PFJetFactory = pfJetFactory.clone(), 
                    applyResolution = cms.bool(False),
                    resolutionFile = cms.FileInPath("CondFormats/JetMETObjects/data/Spring10_PtResolution_AK5PF.txt"),
                    resolutionOverride = cms.double(-1.),# negative => use default
                    applyScale = cms.bool(False),
                    applyScaleFromDB= cms.bool(False),
                    scaleFile = cms.FileInPath("ExoDiBosonResonances/EDBRCommon/data/DUMMY_GR_R_42_V19_AK5PF_DUMMY_Uncertainty_DUMMY.txt"),
                    nSigmaScale = cms.double(0.0) # vary scale by n sigma
                     ),  ### end cfg = cms.PSet 
    cuts = cms.PSet(
 #       btag = trackCountingHighEffBJetTags.clone(),
 #       TCHE = trackCountingHighEffBJetTags.clone(),
 #       JP = jetProbabilityBJetTags.clone(),
 #       CSV = combinedSecondaryVertexBJetTags.clone(),
       jetKinematics = jetKinematics.clone(),
       looseJetId = looseJetId.clone(),
       TOBTECjetsId = TOBTECjetsId.clone()
 #      genLepton = cms.PSet( genLepton = cms.string("sourcePtr().get().hasOverlaps('genLeptons')")),
 #      recoLepton = cms.PSet( recoLepton = cms.string("sourcePtr().get().hasOverlaps('recoLeptons')")),
       ),    
    verbose = cms.untracked.bool( False )
)


from ExoDiBosonResonances.EDBRCommon.selections.jetKinematics_cfi import mergedJetKinematics, mergedJetVTagging
from ExoDiBosonResonances.EDBRCommon.selections.jetKinematics_cfi import isMergedSideband, isMergedSignal
from ExoDiBosonResonances.EDBRCommon.selections.jetKinematics_cfi import isMergedWSideband, isMergedWSignal
from ExoDiBosonResonances.EDBRCommon.selections.jetKinematics_cfi import isMergedFullRange

structJetFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatJets"),
       prunedJetCollection=cms.InputTag("selectedPatJetsPruned"),
       baseJetFactory = baseJetFactory.clone(),
       pfJetFactory = pfJetFactory.clone(),
       useConstituents=cms.bool(False),
       puVariables=cms.InputTag("puJetId"), #puJetIdAK5NoPUSub
       puMvas=cms.VInputTag("puJetMvaCA8CHS:cutbasedDiscriminant",
                            "puJetMvaCA8CHS:simpleDiscriminant", #puJetMvaAK5NoPUSub
                            "puJetMvaCA8CHS:fullDiscriminant"
                            ),
       puIds=cms.VInputTag("puJetMvaCA8CHS:cutbasedId",
                           "puJetMvaCA8CHS:simpleId",
                           "puJetMvaCA8CHS:fullId",                           
                           ),
       verbose = cms.untracked.bool( True ),
###used for JES and JER systematics
       applyResolution = cms.bool(False),
       resolutionFile = cms.FileInPath("CondFormats/JetMETObjects/data/Spring10_PtResolution_AK5PF.txt"), 
       resolutionOverride = cms.double(-1.),# negative => use default
       applyScale = cms.bool(True),
       applyScaleFromDB= cms.bool(True),
       scaleFile = cms.FileInPath("ExoDiBosonResonances/EDBRCommon/data/DUMMY_GR_R_42_V19_AK5PF_DUMMY_Uncertainty_DUMMY.txt"), 
       nSigmaScale = cms.double(-2.0) # vary scale by n sigma
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
    TOBTECjetsId = TOBTECjetsId.clone(),
    mergedJetVTagging = mergedJetVTagging.clone(),
    isZSignal = isMergedSignal.clone(),
    isZSideband = isMergedSideband.clone(),
    isWSignal = isMergedWSignal.clone(),
    isWSideband = isMergedWSideband.clone(),
    isFullRange = isMergedFullRange.clone(),
    genP = cms.PSet( genLepton = cms.string("sourcePtr().get().hasOverlaps('genJets')"))
 #      recoLepton = cms.PSet( recoLepton = cms.string("sourcePtr().get().hasOverlaps('recoLeptons')")),
       )
)


