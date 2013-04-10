import FWCore.ParameterSet.Config as cms




### values used for 2012 analysis, to be used with HLTWeightProducer2012
dummyHLTEleEffWeights =  cms.PSet( lumi = cms.double(1.),#dummy
                               lep1 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                               lep2 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                               hlt  = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,999.0,0.0,999. ,1.0))),
                               )

eleIdSF_Run2012= cms.VPSet( ### dummy at one for now
    cms.PSet(bin = cms.vdouble(0.0,0.9,20.,40. ,1.000)),
    cms.PSet(bin = cms.vdouble(0.9,1.2,20.,40. ,1.000)),
    cms.PSet(bin = cms.vdouble(1.2,2.1,20.,40. ,1.000)),
    cms.PSet(bin = cms.vdouble(2.1,2.4,20.,40. ,1.000)),
    cms.PSet(bin = cms.vdouble(0.0,0.9,40.,999. ,1.000)),
    cms.PSet(bin = cms.vdouble(0.9,1.2,40.,999. ,1.000)),
    cms.PSet(bin = cms.vdouble(1.2,2.1,40.,999. ,1.000)),
    cms.PSet(bin = cms.vdouble(2.1,2.4,40.,999. ,1.000)),
    )

singleEleTrigSF_Run2012= cms.VPSet(
    cms.PSet(bin = cms.vdouble(0.0,0.9, 0.0,0.9 ,1.000)),
    cms.PSet(bin = cms.vdouble(0.0,0.9, 0.9,1.2 ,1.000)),
    cms.PSet(bin = cms.vdouble(0.0,0.9, 1.2,2.1 ,1.000)),
    cms.PSet(bin = cms.vdouble(0.0,0.9, 2.1,2.4 ,1.000)),

    cms.PSet(bin = cms.vdouble(0.9, 1.2, 0.0,0.9 ,1.000)),
    cms.PSet(bin = cms.vdouble(0.9, 1.2, 0.9,1.2 ,1.000)),
    cms.PSet(bin = cms.vdouble(0.9, 1.2, 1.2,2.1 ,1.000)),
    cms.PSet(bin = cms.vdouble(0.9, 1.2, 2.1,2.4 ,1.000)),

    cms.PSet(bin = cms.vdouble(1.2, 2.1, 0.0,0.9 ,1.000)),
    cms.PSet(bin = cms.vdouble(1.2, 2.1, 0.9,1.2 ,1.000)),
    cms.PSet(bin = cms.vdouble(1.2, 2.1, 1.2,2.1 ,1.000)),
    cms.PSet(bin = cms.vdouble(1.2, 2.1, 2.1,2.4 ,1.000)),

    cms.PSet(bin = cms.vdouble(2.1, 2.4, 0.0,0.9 ,1.000)),
    cms.PSet(bin = cms.vdouble(2.1, 2.4, 0.9,1.2 ,1.000)),
    cms.PSet(bin = cms.vdouble(2.1, 2.4, 1.2,2.1 ,1.000)),
    cms.PSet(bin = cms.vdouble(2.1, 2.4, 2.1,2.4 ,1.000)),
    
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


