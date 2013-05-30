import FWCore.ParameterSet.Config as cms




### values used for 2012 analysis, to be used with HLTWeightProducer2012
dummyHLTEleEffWeights =  cms.PSet( lumi = cms.double(1.),#dummy
                               lep1 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                               lep2 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                               hlt  = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,999.0,0.0,999. ,1.0))),
                               )

eleIdSF_Run2012= cms.VPSet( ### eta bin of lepton, pt bin of lepton, eff
    cms.PSet(bin = cms.vdouble(0.0,   1.4442,   0.0,   9999.,   0.98)),
    cms.PSet(bin = cms.vdouble(1.566,    2.5,   0.0,   9999.,   0.98)),
    )

singleEleTrigSF_Run2012= cms.VPSet(  ### eta bin of lepton, eta bin of neutrino, eff  ## bin neutrino is used for zz
    cms.PSet(bin = cms.vdouble(0.0,   1.4442,   0.0,   9999.,     0.991)),
    cms.PSet(bin = cms.vdouble(1.566,    2.5,   0.0,   9999.,     0.976)),
    
    )



#### to be expanded according to the modifcations of the HLT triggers that we use
#### (a set of efficieincies for each of them, function of etan and pt, calculated with T&P)
HLTWeightsKinFitEle = cms.EDProducer("HLTWeightProducer2012WelenuDiJet",
                                    src = cms.InputTag("cmgWelenuDiJetKinFitEDBR") ,
                                    isMuChannel = cms.bool(False),
                                    ranges = cms.VPSet(
                                                       cms.PSet( lumi = cms.double(19532.0),#Run2012
                                                                 lep1 = eleIdSF_Run2012,
                                                                 lep2 = eleIdSF_Run2012,
                                                                 hlt = singleEleTrigSF_Run2012 
                                                                 )   
                                                       #  dummyHLTEleEffWeights
                                                       )# end ranges
                                    )

    
    
HLTWeightsEle = HLTWeightsKinFitEle.clone()
HLTWeightsEle.src = "cmgWelenuDiJetEDBR"
    
    
HLTWeightsMergedEle =cms.EDProducer("HLTWeightProducer2012WelenuSingleJet",
                                   src = cms.InputTag( "cmgWelenuSingleJetEDBR") ,
                                   isMuChannel = cms.bool(False),
                                   ranges = cms.VPSet(
                                                       cms.PSet( lumi = cms.double(19532.0),#Run2012
                                                                 lep1 = eleIdSF_Run2012,
                                                                 lep2 = eleIdSF_Run2012,
                                                                 hlt = singleEleTrigSF_Run2012 
                                                                 )   
                                                       #  dummyHLTEleEffWeights
                                                       )# end ranges
                                   )


