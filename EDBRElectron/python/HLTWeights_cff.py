import FWCore.ParameterSet.Config as cms

### values used for 2012 analysis, to be used with HLTWeightProducer2012
dummyHLTEleEffWeights =  cms.PSet( lumi = cms.double(1.),#dummy
                               lep1 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                               lep2 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                               hlt  = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,999.0,0.0,999. ,1.0))),
                               )

#barrel sf from T. Williams
eleIdSF_Run2012= cms.VPSet(
    cms.PSet(bin = cms.vdouble(0.0,1.44 ,20.,999. ,1.001)),
    cms.PSet(bin = cms.vdouble(1.44,99.0,20.,999. ,1.00))
    )

doubleEleTrigEff_Run2012= cms.VPSet(
    cms.PSet(bin = cms.vdouble(0.0,999.0 ,0.,999. ,1.00))
    )

HLTWeightsKinFit = cms.EDProducer("HLTWeightProducer2012Electron",
                                  src = cms.InputTag("cmgDiElectronDiJetKinFitEDBR") ,
                                  isMuChannel = cms.bool(False),
                                  ranges = cms.VPSet(
                                                    cms.PSet( lumi = cms.double(19532.0),#Run2012
                                                              lep1 = eleIdSF_Run2012,
                                                              lep2 = eleIdSF_Run2012,
                                                              hlt = doubleEleTrigEff_Run2012 
                                                              )
                                                 #     myHLTEleEffWeights
                                                    )# end ranges       
                                  )

 
HLTWeights = HLTWeightsKinFit.clone()
HLTWeights.src = "cmgDiElectronDiJetEDBR"
HLTWeightsMerged = cms.EDProducer("HLTWeightProducer2012EleVJet",
                                  src = cms.InputTag("cmgDiElectronVJetEDBR") ,
                                  isMuChannel = cms.bool(False),
                                  ranges = cms.VPSet(
                                                    cms.PSet( lumi = cms.double(19532.0),#Run2012
                                                              lep1 = eleIdSF_Run2012,
                                                              lep2 = eleIdSF_Run2012,
                                                              hlt = doubleEleTrigEff_Run2012 
                                                              )
                                                 #     myHLTEleEffWeights
                                                    )# end ranges       
                                  )



####################
####################
####### for 2011 (EXO-11-102) we used weights set to 1 for electrons
myHLTEleEffWeights2011 =  cms.PSet( lumi = cms.double(1.),#dummy
                                double1 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                                double2 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                                single  = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                                )

