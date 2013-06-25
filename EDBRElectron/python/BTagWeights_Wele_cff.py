import FWCore.ParameterSet.Config as cms

BTagWeightsKinFitEle = cms.EDProducer("BTagWeightProducerWelenuDiJet",
                                    src = cms.InputTag("HLTWeightsKinFitEle") ,
                                    )

    
    
BTagWeightsEle = BTagWeightsKinFitEle.clone()
BTagWeightsEle.src = "HLTWeightsEle"
    
    
BTagWeightsMergedEle =cms.EDProducer("BTagWeightProducerWelenuSingleJet",
                                   src = cms.InputTag("HLTWeightsMergedEle") ,
                                   )


