import FWCore.ParameterSet.Config as cms

from ExoDiBosonResonances.EDBRMuon.factories.cmgdimuondijet_cfi import * 
from ExoDiBosonResonances.EDBRMuon.factories.cmgdimuondijetEDBR_cfi import *
from ExoDiBosonResonances.EDBRMuon.factories.cmgDiMuonSingleJet_cfi import * 
from ExoDiBosonResonances.EDBRMuon.factories.cmgDiMuonSingleJetEDBR_cfi import *
from ExoDiBosonResonances.EDBRMuon.skims.cmgEDBRSel_cff import *
from ExoDiBosonResonances.EDBRMuon.skims.selEventsEDBR_cfi import *
from ExoDiBosonResonances.EDBRMuon.HLTWeights_cff import *

cmgEDBRKinFitMuWeighted2012A = cms.EDProducer("DiMuonDiJetEDBRWeightAdder",
                                        src=cms.InputTag("HLTWeightsKinFitMu"),
                                        weight=cms.InputTag("PUWeights2012A"),
                                        )
cmgEDBRMuWeighted2012A = cms.EDProducer("DiMuonDiJetEDBRWeightAdder",
                                      src=cms.InputTag("HLTWeightsMu"),
                                      weight=cms.InputTag("PUWeights2012A"),
                                      )

cmgEDBRMergedWeighted2012A = cms.EDProducer("DiMuonVJetEDBRWeightAdder",
                                            src=cms.InputTag("HLTWeightsMergedMu"),
                                            weight=cms.InputTag("PUWeights2012A")                                      
                                            )

########

cmgEDBRKinFitMuWeighted2012B = cms.EDProducer("DiMuonDiJetEDBRWeightAdder",
                                            src=cms.InputTag("cmgEDBRKinFitMuWeighted2012A"),
                                            weight=cms.InputTag("PUWeights2012B")
                                            )

cmgEDBRMuWeighted2012B = cms.EDProducer("DiMuonDiJetEDBRWeightAdder",
                                      src=cms.InputTag("cmgEDBRMuWeighted2012A"),
                                      weight=cms.InputTag("PUWeights2012B")
                                      )

cmgEDBRMergedWeighted2012B = cms.EDProducer("DiMuonVJetEDBRWeightAdder",
                                            src=cms.InputTag("cmgEDBRMergedWeighted2012A"),
                                            weight=cms.InputTag("PUWeights2012B"),
                                            )
#####
cmgEDBRKinFitMuWeighted = cms.EDProducer("DiMuonDiJetEDBRWeightAdder",
                                        src=cms.InputTag("cmgEDBRKinFitMuWeighted2012B"),
                                        weight=cms.InputTag("PUWeights"),
                                        )
cmgEDBRMuWeighted = cms.EDProducer("DiMuonDiJetEDBRWeightAdder",
                                  src=cms.InputTag("cmgEDBRMuWeighted2012B"),
                                  weight=cms.InputTag("PUWeights"),
                                  )
cmgEDBRMergedWeighted = cms.EDProducer("DiMuonVJetEDBRWeightAdder",
                                  src=cms.InputTag("cmgEDBRMergedWeighted2012B"),
                                  weight=cms.InputTag("PUWeights"),
                                  )

edbrSequenceMMJJ = cms.Sequence(
    cmgDiMuonDiJet +
    cmgDiMuonDiJetKinFit +
    
    cmgDiMuonDiJetEDBR +
    cmgDiMuonDiJetKinFitEDBR +

    HLTWeightsMu +
    HLTWeightsKinFitMu +

    cmgEDBRKinFitMuWeighted2012A +
    cmgEDBRMuWeighted2012A +
    cmgEDBRKinFitMuWeighted2012B +
    cmgEDBRMuWeighted2012B +
    cmgEDBRKinFitMuWeighted +
    cmgEDBRMuWeighted +

    cmgEDBRSelMu +
    cmgEDBRSelKinFitMu +

    selectedEDBRMuCandFilter
)

edbrSequenceMerged = cms.Sequence(
    cmgDiMuonVJet +
    cmgDiMuonVJetEDBR +

    HLTWeightsMergedMu +

    cmgEDBRMergedWeighted2012A +
    cmgEDBRMergedWeighted2012B +
    cmgEDBRMergedWeighted +

    cmgEDBRMergedSel +
    selectedEDBRMergedCandFilter
    )
