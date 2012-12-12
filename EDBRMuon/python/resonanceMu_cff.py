import FWCore.ParameterSet.Config as cms

from ExoDiBosonResonances.EDBRMuon.factories.cmgdimuondijet_cfi import * 
from ExoDiBosonResonances.EDBRMuon.factories.cmgdimuondijetEDBR_cfi import *
from ExoDiBosonResonances.EDBRMuon.factories.cmgDiMuonSingleJet_cfi import * 
from ExoDiBosonResonances.EDBRMuon.factories.cmgDiMuonSingleJetEDBR_cfi import *
from ExoDiBosonResonances.EDBRMuon.skims.cmgEDBRSel_cff import *
from ExoDiBosonResonances.EDBRMuon.skims.selEventsEDBR_cfi import *
from ExoDiBosonResonances.EDBRMuon.HLTWeights_cff import *

cmgEDBRKinFitWeighted2012A = cms.EDProducer("DiMuonDiJetEDBRWeightAdder",
                                        src=cms.InputTag("HLTWeightsKinFit"),
                                        weight=cms.InputTag("PUWeights2012A"),
                                        )
cmgEDBRWeighted2012A = cms.EDProducer("DiMuonDiJetEDBRWeightAdder",
                                      src=cms.InputTag("HLTWeights"),
                                      weight=cms.InputTag("PUWeights2012A"),
                                      )

cmgEDBRMergedWeighted2012A = cms.EDProducer("DiMuonVJetEDBRWeightAdder",
                                            src=cms.InputTag("HLTWeightsMerged"),
                                            weight=cms.InputTag("PUWeights2012A")                                      
                                            )

########

cmgEDBRKinFitWeighted2012B = cms.EDProducer("DiMuonDiJetEDBRWeightAdder",
                                            src=cms.InputTag("cmgEDBRKinFitWeighted2012A"),
                                            weight=cms.InputTag("PUWeights2012B")
                                            )

cmgEDBRWeighted2012B = cms.EDProducer("DiMuonDiJetEDBRWeightAdder",
                                      src=cms.InputTag("cmgEDBRWeighted2012A"),
                                      weight=cms.InputTag("PUWeights2012B")
                                      )

cmgEDBRMergedWeighted2012B = cms.EDProducer("DiMuonVJetEDBRWeightAdder",
                                            src=cms.InputTag("cmgEDBRMergedWeighted2012A"),
                                            weight=cms.InputTag("PUWeights2012B"),
                                            )
#####
cmgEDBRKinFitWeighted = cms.EDProducer("DiMuonDiJetEDBRWeightAdder",
                                        src=cms.InputTag("cmgEDBRKinFitWeighted2012B"),
                                        weight=cms.InputTag("PUWeights"),
                                        )
cmgEDBRWeighted = cms.EDProducer("DiMuonDiJetEDBRWeightAdder",
                                  src=cms.InputTag("cmgEDBRWeighted2012B"),
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

    HLTWeights +
    HLTWeightsKinFit +

    cmgEDBRKinFitWeighted2012A +
    cmgEDBRWeighted2012A +
    cmgEDBRKinFitWeighted2012B +
    cmgEDBRWeighted2012B +
    cmgEDBRKinFitWeighted +
    cmgEDBRWeighted +

    cmgEDBRSel +
    cmgEDBRSelKinFit +

    selectedEDBRCandFilter
)

edbrSequenceMerged = cms.Sequence(
    cmgDiMuonVJet +
    cmgDiMuonVJetEDBR +

    HLTWeightsMerged +

    cmgEDBRMergedWeighted2012A +
    cmgEDBRMergedWeighted2012B +
    cmgEDBRMergedWeighted +

    cmgEDBRMergedSel +
    selectedEDBRMergedCandFilter
    )
