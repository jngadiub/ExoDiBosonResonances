import FWCore.ParameterSet.Config as cms

HLTWeightsKinFit = cms.EDProducer("HLTWeightProducerElectron",
                                  src = cms.InputTag("cmgDiElectronDiJetKinFitEDBR") ,
                                  ranges = cms.VPSet( cms.PSet( lumi = cms.double(1.),#dummy
                                                            double1 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                                                            double2 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                                                            single  = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),                                                            
                                                            ),
                                                  )       
                                  )

 
HLTWeights = HLTWeightsKinFit.clone()
HLTWeights.src = "cmgDiElectronDiJetEDBR"
