import FWCore.ParameterSet.Config as cms


### values used for 2012 analysis, to be used with HLTWeightProducer2012
dummyHLTMuEffWeights =  cms.PSet( lumi = cms.double(1.),#dummy
                               lep1 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                               lep2 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                               hlt  = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,999.0,0.0,999. ,1.0))),
                               )

globMuIdSF_Run2012= cms.VPSet(
    cms.PSet(bin = cms.vdouble(0.0,0.9,20.,40. ,0.9907)),
    cms.PSet(bin = cms.vdouble(0.9,1.2,20.,40. ,0.9490)),
    cms.PSet(bin = cms.vdouble(1.2,2.1,20.,40. ,1.0094)),
    cms.PSet(bin = cms.vdouble(2.1,2.4,20.,40. ,0.9832)),
    cms.PSet(bin = cms.vdouble(0.0,0.9,40.,60. ,0.9748)),
    cms.PSet(bin = cms.vdouble(0.9,1.2,40.,60. ,0.9332)),
    cms.PSet(bin = cms.vdouble(1.2,2.1,40.,60. ,0.9888)),
    cms.PSet(bin = cms.vdouble(2.1,2.4,40.,60. ,0.9720)),
    cms.PSet(bin = cms.vdouble(0.0,0.9,60.,80. ,1.041)),
    cms.PSet(bin = cms.vdouble(0.9,1.2,60.,80. ,1.029)),
    cms.PSet(bin = cms.vdouble(1.2,2.1,60.,80. ,1.049)),
    cms.PSet(bin = cms.vdouble(2.1,2.4,60.,80. ,1.097)),
    cms.PSet(bin = cms.vdouble(0.0,0.9,80.,100. ,0.999)),
    cms.PSet(bin = cms.vdouble(0.9,1.2,80.,100. ,0.998)),
    cms.PSet(bin = cms.vdouble(1.2,2.1,80.,100. ,1.009)),
    cms.PSet(bin = cms.vdouble(2.1,2.4,80.,100. ,1.060)),
    cms.PSet(bin = cms.vdouble(0.0,0.9,100.,5000. ,0.985)),
    cms.PSet(bin = cms.vdouble(0.9,1.2,100.,5000. ,1.009)),
    cms.PSet(bin = cms.vdouble(1.2,2.1,100.,5000. ,0.997)),
    cms.PSet(bin = cms.vdouble(2.1,2.4,100.,5000. ,0.990))
    )

tkMuIdSF_Run2012= cms.VPSet(
    cms.PSet(bin = cms.vdouble(0.0,0.9,20.,40. ,0.9746)),
    cms.PSet(bin = cms.vdouble(0.9,1.2,20.,40. ,0.9231)),
    cms.PSet(bin = cms.vdouble(1.2,2.1,20.,40. ,0.9930)),
    cms.PSet(bin = cms.vdouble(2.1,2.4,20.,40. ,0.9890)),
    cms.PSet(bin = cms.vdouble(0.0,0.9,40.,60. ,0.9574)),
    cms.PSet(bin = cms.vdouble(0.9,1.2,40.,60. ,0.9369)),
    cms.PSet(bin = cms.vdouble(1.2,2.1,40.,60. ,0.9710)),
    cms.PSet(bin = cms.vdouble(2.1,2.4,40.,60. ,0.9789)),
    cms.PSet(bin = cms.vdouble(0.0,0.9,60.,80. ,1.035)),
    cms.PSet(bin = cms.vdouble(0.9,1.2,60.,80. ,1.026)),
    cms.PSet(bin = cms.vdouble(1.2,2.1,60.,80. ,1.041)),
    cms.PSet(bin = cms.vdouble(2.1,2.4,60.,80. ,1.070)),
    cms.PSet(bin = cms.vdouble(0.0,0.9,80.,100. ,1.003)),
    cms.PSet(bin = cms.vdouble(0.9,1.2,80.,100. ,1.001)),
    cms.PSet(bin = cms.vdouble(1.2,2.1,80.,100. ,1.011)),
    cms.PSet(bin = cms.vdouble(2.1,2.4,80.,100. ,1.020)),
    cms.PSet(bin = cms.vdouble(0.0,0.9,100.,5000. ,0.978)),
    cms.PSet(bin = cms.vdouble(0.9,1.2,100.,5000. ,1.007)),
    cms.PSet(bin = cms.vdouble(1.2,2.1,100.,5000. ,0.984)),
    cms.PSet(bin = cms.vdouble(2.1,2.4,100.,5000. ,0.970))

    )

doubleMuTrigSF_Run2012= cms.VPSet(
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

HLTWeightsKinFitMu = cms.EDProducer("HLTWeightProducer2012Mu",
                                    src = cms.InputTag("cmgDiMuonDiJetKinFitEDBR") ,
                                    isMuChannel = cms.bool(True),
                                    ranges = cms.VPSet(
                                          cms.PSet( lumi = cms.double(19532.0),#Run2012
                                                    lep1 = globMuIdSF_Run2012,
                                                    lep2 = tkMuIdSF_Run2012,
                                                    hlt = doubleMuTrigSF_Run2012 
                                                    )
                                          #  dummyHLTMuEffWeights
                                                                
                                  
                                          )# end ranges
                                    )       
HLTWeightsMu = HLTWeightsKinFitMu.clone()
HLTWeightsMu.src = "cmgDiMuonDiJetEDBR"


HLTWeightsMergedMu =cms.EDProducer("HLTWeightProducer2012MuVJet",
                                   src = cms.InputTag( "cmgDiMuonVJetEDBR") ,
                                   isMuChannel = cms.bool(True),
                                   ranges = cms.VPSet(
                                            cms.PSet(lumi = cms.double(19532.0),
                                            lep1 = globMuIdSF_Run2012,
                                            lep2 = tkMuIdSF_Run2012,
                                            hlt = doubleMuTrigSF_Run2012
                                                     )
                                            ) # end ranges
                                   ### dummyHLTMuEffWeights
                                   )



###
#######################################################
###
### values used for 2011 analysis EXO-11-102, to be used with HLTWeightProducerMu
doubleMuLeg1Eff_Run2011A= cms.VPSet(
    cms.PSet(bin = cms.vdouble(0.0,0.8,10.,20. ,0.975)),
    cms.PSet(bin = cms.vdouble(0.0,0.8,20.,40. ,0.975)),
    cms.PSet(bin = cms.vdouble(0.0,0.8,40.,150.,0.973)),
    cms.PSet(bin = cms.vdouble(0.8,2.1,10.,20. ,0.955)),
    cms.PSet(bin = cms.vdouble(0.8,2.1,20.,40. ,0.950)),
    cms.PSet(bin = cms.vdouble(0.8,2.1,40.,150.,0.946)),
    cms.PSet(bin = cms.vdouble(2.1,2.4,10.,20. ,0.958)),
    cms.PSet(bin = cms.vdouble(2.1,2.4,20.,40. ,0.922)),
    cms.PSet(bin = cms.vdouble(2.1,2.4,40.,150.,0.908))
    )                                                                                

doubleMuLeg2Eff_Run2011A= cms.VPSet(
    cms.PSet(bin = cms.vdouble(0.0,0.8,10.,20. ,0.975)),
    cms.PSet(bin = cms.vdouble(0.0,0.8,20.,40. ,0.975)),
    cms.PSet(bin = cms.vdouble(0.0,0.8,40.,150.,0.973)),
    cms.PSet(bin = cms.vdouble(0.8,2.1,10.,20. ,0.955)),
    cms.PSet(bin = cms.vdouble(0.8,2.1,20.,40. ,0.950)),
    cms.PSet(bin = cms.vdouble(0.8,2.1,40.,150.,0.946)),
    cms.PSet(bin = cms.vdouble(2.1,2.4,10.,20. ,0.958)),
    cms.PSet(bin = cms.vdouble(2.1,2.4,20.,40. ,0.922)),
    cms.PSet(bin = cms.vdouble(2.1,2.4,40.,150.,0.908))
    )                                                                                
singleMuEff_Run2011A= cms.VPSet(
    cms.PSet(bin = cms.vdouble(0.0,0.8,10.,20. ,0.975)),
    cms.PSet(bin = cms.vdouble(0.0,0.8,20.,40. ,0.975)),
    cms.PSet(bin = cms.vdouble(0.0,0.8,40.,150.,0.973)),
    cms.PSet(bin = cms.vdouble(0.8,2.1,10.,20. ,0.955)),
    cms.PSet(bin = cms.vdouble(0.8,2.1,20.,40. ,0.950)),
    cms.PSet(bin = cms.vdouble(0.8,2.1,40.,150.,0.946)),
    cms.PSet(bin = cms.vdouble(2.1,2.4,10.,20. ,0.958)),
    cms.PSet(bin = cms.vdouble(2.1,2.4,20.,40. ,0.922)),
    cms.PSet(bin = cms.vdouble(2.1,2.4,40.,150.,0.908))
    )    
myOLDDummyHLTMuEffWeights =  cms.PSet( lumi = cms.double(1.),#dummy
                               double1 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                               double2 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                               single  = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                               )
HLTWeightsKinFitMuOLD = cms.EDProducer("HLTWeightProducerMu",
                                  src = cms.InputTag("cmgDiMuonDiJetKinFitEDBR") ,
                                  ranges = cms.VPSet(
    #  cms.PSet( lumi = cms.double(217.),#Run2011A1
    #                                                                 double1 = doubleMuLeg1Eff_Run2011A,
    #                                                                 double2 = doubleMuLeg2Eff_Run2011A,
    #                                                                 single = singleMuEff_Run2011A
    #                                                                 ),
    #                                                       cms.PSet( lumi = cms.double(920.),#Run2011A2
    #                                                                 double1 = doubleMuLeg1Eff_Run2011A,
    #                                                                 double2 = doubleMuLeg2Eff_Run2011A,
    #                                                                 single = singleMuEff_Run2011A
    #                                                                 ),
                                                      myOLDDummyHLTMuEffWeights
                                                                
                                  
                                                      )# end ranges
                                                      )          


 
