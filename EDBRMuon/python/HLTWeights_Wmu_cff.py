import FWCore.ParameterSet.Config as cms

### these VPset are dummy for now, to be updated for 2012
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


myHLTMuEffWeights =  cms.PSet( lumi = cms.double(1.),#dummy
                               double1 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                               double2 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                               single  = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,3.0,0.0,7000. ,1.0))),
                               )
    
#### to be expanded according to the modifcations of the HLT triggers that we use
#### (a set of efficieincies for each of them, function of etan and pt, calculated with T&P)
HLTWeightsKinFitMu = cms.EDProducer("HLTWeightProducerWmunuDiJet",
                                  src = cms.InputTag("cmgWmunuDiJetKinFitEDBR") ,
                                  ranges = cms.VPSet( cms.PSet( lumi = cms.double(217.),#Run2011A1
                                                                double1 = doubleMuLeg1Eff_Run2011A,
                                                                double2 = doubleMuLeg2Eff_Run2011A,
                                                                single = singleMuEff_Run2011A
                                                                ),
                                                      cms.PSet( lumi = cms.double(920.),#Run2011A2
                                                                double1 = doubleMuLeg1Eff_Run2011A,
                                                                double2 = doubleMuLeg2Eff_Run2011A,
                                                                single = singleMuEff_Run2011A
                                                                ),
                                                       myHLTMuEffWeights
                                                        
                                  
                                                      )# end ranges
                                                      )

    
    
HLTWeightsMu = HLTWeightsKinFitMu.clone()
HLTWeightsMu.src = "cmgWmunuDiJetEDBR"
    
    
HLTWeightsMergedMu =cms.EDProducer("HLTWeightProducerWmunuSingleJet",
                                  src = cms.InputTag( "cmgWmunuSingleJetEDBR") ,
                                  ranges = cms.VPSet(
                                                      myHLTMuEffWeights
                                                  )
                                  )


