import FWCore.ParameterSet.Config as cms

from ExoDiBosonResonances.EDBRElectron.factories.cmgdielectrondijet_cfi import * 
from ExoDiBosonResonances.EDBRElectron.factories.cmgdielectrondijetEDBR_cfi import *
from ExoDiBosonResonances.EDBRElectron.skims.cmgEDBRSel_cff import *
from ExoDiBosonResonances.EDBRElectron.skims.selEventsEDBR_cfi import *
from ExoDiBosonResonances.EDBRElectron.HLTWeights_cff import *


cmgEDBRKinFitWeighted2012A = cms.EDProducer("DiElectronDiJetEDBRWeightAdder",
                                        src=cms.InputTag("HLTWeightsKinFit"),
                                        weight=cms.InputTag("PUWeights2012A"),
                                        )
cmgEDBRWeighted2012A = cms.EDProducer("DiElectronDiJetEDBRWeightAdder",
                                  src=cms.InputTag("HLTWeights"),
                                  weight=cms.InputTag("PUWeights2012A"),
                                  )
cmgEDBRKinFitWeighted2012B = cms.EDProducer("DiElectronDiJetEDBRWeightAdder",
                                        src=cms.InputTag("cmgEDBRKinFitWeighted2012A"),
                                        weight=cms.InputTag("PUWeights2012B"),
                                        )
cmgEDBRWeighted2012B = cms.EDProducer("DiElectronDiJetEDBRWeightAdder",
                                  src=cms.InputTag("cmgEDBRWeighted2012A"),
                                  weight=cms.InputTag("PUWeights2012B"),
                                  )
cmgEDBRKinFitWeighted = cms.EDProducer("DiElectronDiJetEDBRWeightAdder",
                                        src=cms.InputTag("cmgEDBRKinFitWeighted2012B"),
                                        weight=cms.InputTag("PUWeights"),
                                        )
cmgEDBRWeighted = cms.EDProducer("DiElectronDiJetEDBRWeightAdder",
                                  src=cms.InputTag("cmgEDBRWeighted2012B"),
                                  weight=cms.InputTag("PUWeights"),
                                  )

                                                         
edbrSequenceEE = cms.Sequence(
    cmgDiElectronDiJet +
    cmgDiElectronDiJetKinFit +
    
    cmgDiElectronDiJetEDBR +
    cmgDiElectronDiJetKinFitEDBR +

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
