import FWCore.ParameterSet.Config as cms

BTagWeightsKinFitEle = cms.EDProducer("BTagWeightProducerWelenuDiJet",
                                    src = cms.InputTag("HLTWeightsKinFitEle") ,
									BTagJets=cms.InputTag("jetAK5"),
									EffmapFilename = cms.FileInPath("ExoDiBosonResonances/EDBRCommon/data/BtaggingEffs/WW_TuneZ2star_8TeV_pythia6_tauola_AK5PF_CSVM_bTaggingEfficiencyMap.root"),
									scale = cms.double(0),#0 noscale, 1 scale up, -1 scale down
                                    )

    
    
BTagWeightsEle = BTagWeightsKinFitEle.clone()
BTagWeightsEle.src = "HLTWeightsEle"
    
    
BTagWeightsMergedEle =cms.EDProducer("BTagWeightProducerWelenuSingleJet",
                                   src = cms.InputTag("HLTWeightsMergedEle"),
								   BTagJets=cms.InputTag("jetAK5"),
								   EffmapFilename = cms.FileInPath("ExoDiBosonResonances/EDBRCommon/data/BtaggingEffs/WW_TuneZ2star_8TeV_pythia6_tauola_AK5PF_CSVM_bTaggingEfficiencyMap.root"),
								   scale = cms.double(0),#0 noscale, 1 scale up, -1 scale down
                                   )


