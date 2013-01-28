import FWCore.ParameterSet.Config as cms

myHLTEleEffWeights =  cms.PSet( lumi = cms.double(1.),#dummy
                                double1 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                                double2 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                                single  = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                                )

HLTWeightsKinFit = cms.EDProducer("HLTWeightProducerWelenuDiJet",
                                  src = cms.InputTag("cmgWelenuDiJetKinFitEDBR") ,
                                  ranges = cms.VPSet(
                                                      myHLTEleEffWeights
                                                      )       
                                  )

 
HLTWeights = HLTWeightsKinFit.clone()
HLTWeights.src = "cmgWelenuDiJetEDBR"

HLTWeightsMerged =cms.EDProducer("HLTWeightProducerWelenuSingleJet",
                                  src = cms.InputTag( "cmgWelenuSingleJetEDBR") ,
                                  ranges = cms.VPSet(
                                                      myHLTEleEffWeights
                                                  )       
                                  )

