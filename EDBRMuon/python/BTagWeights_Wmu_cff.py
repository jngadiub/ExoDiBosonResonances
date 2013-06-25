import FWCore.ParameterSet.Config as cms



BTagWeightsKinFitMu = cms.EDProducer("BTagWeightProducerWmunuDiJet",
                                    src = cms.InputTag("HLTWeightsKinFitMu") ,
                                    )

    
    
BTagWeightsMu = BTagWeightsKinFitMu.clone()
BTagWeightsMu.src = "HLTWeightsMu"
    
    
BTagWeightsMergedMu =cms.EDProducer("BTagWeightProducerWmunuSingleJet",
                                   src = cms.InputTag( "HLTWeightsMergedMu") ,
                                   )


