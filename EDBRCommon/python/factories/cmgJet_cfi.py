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
from HiggsAna.HLLJJCommon.selections.jetKinematics_cfi import jetKinematics
from HiggsAna.HLLJJCommon.selections.jetid_cfi import *

cmgJet = cms.EDFilter(
    "PFJetSmearPOProducer", 
    cfg = cms.PSet( PFJetFactory = pfJetFactory.clone(), 
                    applyResolution = cms.bool(False),
                    resolutionFile = cms.FileInPath("CondFormats/JetMETObjects/data/Spring10_PtResolution_AK5PF.txt"),
                    resolutionOverride = cms.double(-1.),# negative => use default
                    applyScale = cms.bool(False),
                    applyScaleFromDB= cms.bool(False),
                    scaleFile = cms.FileInPath("HiggsAna/HLLJJCommon/data/GR_R_42_V19_AK5PF_Uncertainty.txt"),
                    nSigmaScale = cms.double(0.0) # vary scale by n sigma
                    ),
    cuts = cms.PSet(
  #     btag = trackCountingHighEffBJetTags.clone(),
        TCHE = trackCountingHighEffBJetTags.clone(),
        JP = jetProbabilityBJetTags.clone(),
        CSV = combinedSecondaryVertexBJetTags.clone(),
       jetKinematics = jetKinematics.clone(),
       looseJetId = looseJetId.clone(),
       genLepton = cms.PSet( genLepton = cms.string("sourcePtr().get().hasOverlaps('genLeptons')")),
       recoLepton = cms.PSet( recoLepton = cms.string("sourcePtr().get().hasOverlaps('recoLeptons')")),
       ),    
    verbose = cms.untracked.bool( False )
)


