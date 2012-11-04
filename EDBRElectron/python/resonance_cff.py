import FWCore.ParameterSet.Config as cms

from ExoDiBosonResonances.EDBRElectron.factories.dielectrondijet_cfi import * 
#from ExoDiBosonResonances.EDBRElectron.factories.diElectronDiJetHiggs_cfi import *
#from ExoDiBosonResonances.EDBRElectron.skims.cmgHiggsSel_cff import *
#from ExoDiBosonResonances.EDBRElectron.skims.selEventsHiggs_cfi import *
#from ExoDiBosonResonances.EDBRElectron.HLTWeights_cff import *


## cmgHiggsKinFitWeighted2012A = cms.EDProducer("DiElectronDiJetHiggsWeightAdder",
##                                         src=cms.InputTag("HLTWeightsKinFit"),
##                                         weight=cms.InputTag("PUWeights2012A"),
##                                         )
## cmgHiggsWeighted2012A = cms.EDProducer("DiElectronDiJetHiggsWeightAdder",
##                                   src=cms.InputTag("HLTWeights"),
##                                   weight=cms.InputTag("PUWeights2012A"),
##                                   )
## cmgHiggsKinFitWeighted2012B = cms.EDProducer("DiElectronDiJetHiggsWeightAdder",
##                                         src=cms.InputTag("cmgHiggsKinFitWeighted2012A"),
##                                         weight=cms.InputTag("PUWeights2012B"),
##                                         )
## cmgHiggsWeighted2012B = cms.EDProducer("DiElectronDiJetHiggsWeightAdder",
##                                   src=cms.InputTag("cmgHiggsWeighted2012A"),
##                                   weight=cms.InputTag("PUWeights2012B"),
##                                   )
## cmgHiggsKinFitWeighted = cms.EDProducer("DiElectronDiJetHiggsWeightAdder",
##                                         src=cms.InputTag("cmgHiggsKinFitWeighted2012B"),
##                                         weight=cms.InputTag("PUWeights"),
##                                         )
## cmgHiggsWeighted = cms.EDProducer("DiElectronDiJetHiggsWeightAdder",
##                                   src=cms.InputTag("cmgHiggsWeighted2012B"),
##                                   weight=cms.InputTag("PUWeights"),
##                                   )

                                                         
resonanceSequence = cms.Sequence(
    cmgDiElectronDiJet +
    cmgDiElectronDiJetKinFit 
    
##     cmgDiElectronDiJetHiggs +
##     cmgDiElectronDiJetKinFitHiggs +

##     HLTWeights +
##     HLTWeightsKinFit +
    
##     cmgHiggsKinFitWeighted2012A +
##     cmgHiggsWeighted2012A +
##     cmgHiggsKinFitWeighted2012B +
##     cmgHiggsWeighted2012B +
##     cmgHiggsKinFitWeighted +
##     cmgHiggsWeighted +

##     cmgHiggsSel +
##     cmgHiggsSelKinFit +    
    
##     selectedHiggsCandFilter
)
