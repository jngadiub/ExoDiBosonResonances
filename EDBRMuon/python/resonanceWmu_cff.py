import FWCore.ParameterSet.Config as cms

from ExoDiBosonResonances.EDBRMuon.factories.cmgWmunuDiJet_cfi import * 
from ExoDiBosonResonances.EDBRMuon.factories.cmgWmunuSingleJet_cfi import * 

from ExoDiBosonResonances.EDBRMuon.skims.cmgEDBRSel_Wmu_cff import *
from ExoDiBosonResonances.EDBRMuon.skims.selEventsEDBR_cfi import *
from ExoDiBosonResonances.EDBRMuon.HLTWeights_Wmu_cff import *

cmgEDBRKinFitMuWeighted2012A = cms.EDProducer("WmunuDiJetEDBRWeightAdder",
                                        src=cms.InputTag("HLTWeightsKinFitMu"),
                                        weight=cms.InputTag("PUWeights2012A"),
                                        )
cmgEDBRMuWeighted2012A = cms.EDProducer("WmunuDiJetEDBRWeightAdder",
                                      src=cms.InputTag("HLTWeightsMu"),
                                      weight=cms.InputTag("PUWeights2012A"),
                                      )

cmgEDBRMergedWeighted2012A = cms.EDProducer("WmunuSingleJetEDBRWeightAdder",
                                            src=cms.InputTag("HLTWeightsMergedMu"),
                                            weight=cms.InputTag("PUWeights2012A")                                      
                                            )

########

cmgEDBRKinFitMuWeighted2012B = cms.EDProducer("WmunuDiJetEDBRWeightAdder",
                                            src=cms.InputTag("cmgEDBRKinFitMuWeighted2012A"),
                                            weight=cms.InputTag("PUWeights2012B")
                                            )

cmgEDBRMuWeighted2012B = cms.EDProducer("WmunuDiJetEDBRWeightAdder",
                                      src=cms.InputTag("cmgEDBRMuWeighted2012A"),
                                      weight=cms.InputTag("PUWeights2012B")
                                      )

cmgEDBRMergedWeighted2012B = cms.EDProducer("WmunuSingleJetEDBRWeightAdder",
                                            src=cms.InputTag("cmgEDBRMergedWeighted2012A"),
                                            weight=cms.InputTag("PUWeights2012B"),
                                            )
#####
cmgEDBRKinFitMuWeighted = cms.EDProducer("WmunuDiJetEDBRWeightAdder",
                                        src=cms.InputTag("cmgEDBRKinFitMuWeighted2012B"),
                                        weight=cms.InputTag("PUWeights"),
                                        )
cmgEDBRMuWeighted = cms.EDProducer("WmunuDiJetEDBRWeightAdder",
                                  src=cms.InputTag("cmgEDBRMuWeighted2012B"),
                                  weight=cms.InputTag("PUWeights"),
                                  )
cmgEDBRMergedWeighted = cms.EDProducer("WmunuSingleJetEDBRWeightAdder",
                                  src=cms.InputTag("cmgEDBRMergedWeighted2012B"),
                                  weight=cms.InputTag("PUWeights"),
                                  )

edbrSequenceMVJJ = cms.Sequence(
    cmgWmunuDiJet +
    cmgWmunuDiJetKinFit +
    
    cmgWmunuDiJetEDBR +
    cmgWmunuDiJetKinFitEDBR +

    HLTWeightsMu +
    HLTWeightsKinFitMu +

    cmgEDBRKinFitMuWeighted2012A +
    cmgEDBRMuWeighted2012A +
    cmgEDBRKinFitMuWeighted2012B +
    cmgEDBRMuWeighted2012B +
    cmgEDBRKinFitMuWeighted +
    cmgEDBRMuWeighted +

    cmgEDBRSelMu +
    cmgEDBRSelKinFitMu 
    
#    selectedEDBRKinFitCandFilterMu
#    selectedEDBRMuCandFilter
)

edbrSequenceMergedMVJ = cms.Sequence(
    cmgWmunuSingleJet +
    cmgWmunuSingleJetEDBR +

    HLTWeightsMergedMu +

    cmgEDBRMergedWeighted2012A +
    cmgEDBRMergedWeighted2012B +
    cmgEDBRMergedWeighted  +
    cmgEDBRMergedSel 
#    selectedEDBRMergedCandFilter
    )
