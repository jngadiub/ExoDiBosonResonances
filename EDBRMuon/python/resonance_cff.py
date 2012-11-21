import FWCore.ParameterSet.Config as cms

from ExoDiBosonResonances.EDBRMuon.factories.cmgdimuondijet_cfi import * 
from ExoDiBosonResonances.EDBRMuon.factories.cmgdimuondijethiggs_cfi import *
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
cmgEDBRKinFitWeighted2012B = cms.EDProducer("DiMuonDiJetEDBRWeightAdder",
                                        src=cms.InputTag("cmgEDBRKinFitWeighted2012A"),
                                        weight=cms.InputTag("PUWeights2012B"),
                                        )
cmgEDBRWeighted2012B = cms.EDProducer("DiMuonDiJetEDBRWeightAdder",
                                  src=cms.InputTag("cmgEDBRWeighted2012A"),
                                  weight=cms.InputTag("PUWeights2012B"),
                                  )
cmgEDBRKinFitWeighted = cms.EDProducer("DiMuonDiJetEDBRWeightAdder",
                                        src=cms.InputTag("cmgEDBRKinFitWeighted2012B"),
                                        weight=cms.InputTag("PUWeights"),
                                        )
cmgEDBRWeighted = cms.EDProducer("DiMuonDiJetEDBRWeightAdder",
                                  src=cms.InputTag("cmgEDBRWeighted2012B"),
                                  weight=cms.InputTag("PUWeights"),
                                  )

edbrSequence = cms.Sequence(
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
