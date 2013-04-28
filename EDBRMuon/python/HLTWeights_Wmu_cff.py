import FWCore.ParameterSet.Config as cms



### values used for 2012 analysis, to be used with HLTWeightProducer2012
dummyHLTMuEffWeights =  cms.PSet( lumi = cms.double(1.),#dummy
                               lep1 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                               lep2 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                               hlt  = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,999.0,0.0,999. ,1.0))),
                               )

globMuIdSF_Run2012= cms.VPSet( ### dummy at one for now
    cms.PSet(bin = cms.vdouble(0.0,0.9,20.,40. ,1.000)),
    cms.PSet(bin = cms.vdouble(0.9,1.2,20.,40. ,1.000)),
    cms.PSet(bin = cms.vdouble(1.2,2.1,20.,40. ,1.000)),
    cms.PSet(bin = cms.vdouble(2.1,2.4,20.,40. ,1.000)),
    cms.PSet(bin = cms.vdouble(0.0,0.9,40.,999. ,1.000)),
    cms.PSet(bin = cms.vdouble(0.9,1.2,40.,999. ,1.000)),
    cms.PSet(bin = cms.vdouble(1.2,2.1,40.,999. ,1.000)),
    cms.PSet(bin = cms.vdouble(2.1,2.4,40.,999. ,1.000)),
    )

singleMuTrigSF_Run2012= cms.VPSet(
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
HLTWeightsKinFitMu = cms.EDProducer("HLTWeightProducer2012WmunuDiJet",
                                    src = cms.InputTag("cmgWmunuDiJetKinFitEDBR") ,
                                    isMuChannel = cms.bool(True),
                                    ranges = cms.VPSet(
                                                       cms.PSet( lumi = cms.double(19532.0),#Run2012
                                                                 lep1 = globMuIdSF_Run2012,
                                                                 lep2 = globMuIdSF_Run2012,
                                                                 hlt = singleMuTrigSF_Run2012 
                                                                 )   
                                                       #  dummyHLTMuEffWeights
                                                       )# end ranges
                                    )

    
    
HLTWeightsMu = HLTWeightsKinFitMu.clone()
HLTWeightsMu.src = "cmgWmunuDiJetEDBR"
    
    
HLTWeightsMergedMu =cms.EDProducer("HLTWeightProducer2012WmunuSingleJet",
                                   src = cms.InputTag( "cmgWmunuSingleJetEDBR") ,
                                   isMuChannel = cms.bool(True),
                                   ranges = cms.VPSet(
                                                       cms.PSet( lumi = cms.double(19532.0),#Run2012
                                                                 lep1 = globMuIdSF_Run2012,
                                                                 lep2 = globMuIdSF_Run2012,
                                                                 hlt = singleMuTrigSF_Run2012 
                                                                 )   
                                                       #  dummyHLTMuEffWeights
                                                       )# end ranges
                                   )


