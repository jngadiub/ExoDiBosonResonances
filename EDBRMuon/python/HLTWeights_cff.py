import FWCore.ParameterSet.Config as cms

HLTWeightsKinFit = cms.EDProducer("HLTWeightProducerMu",
                                  src = cms.InputTag("cmgDiMuonDiJetKinFitHiggs") ,
                                  ranges = cms.VPSet( cms.PSet( lumi = cms.double(217.),#Run2011A1
                                                                double1 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,0.8,10.,20. ,0.975)),
                                                                                     cms.PSet(bin = cms.vdouble(0.0,0.8,20.,40. ,0.975)),
                                                                                     cms.PSet(bin = cms.vdouble(0.0,0.8,40.,150.,0.973)),
                                                                                     
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,10.,20. ,0.955)),
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,20.,40. ,0.950)),
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,40.,150.,0.946)),
                                                                                     
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,10.,20. ,0.958)),
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,20.,40. ,0.922)),
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,40.,150.,0.908)),
                                                                                     ),
                                                                double2 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,0.8,10.,20. ,0.975)),
                                                                                     cms.PSet(bin = cms.vdouble(0.0,0.8,20.,40. ,0.975)),
                                                                                     cms.PSet(bin = cms.vdouble(0.0,0.8,40.,150.,0.973)),
                                                                                     
                                                                                     cms.PSet(bin = cms.vdouble(0.0,2.1,10.,20. ,0.955)),
                                                                                     cms.PSet(bin = cms.vdouble(0.0,2.1,20.,40. ,0.950)),
                                                                                     cms.PSet(bin = cms.vdouble(0.0,2.1,40.,150.,0.946)),
                                                                                     
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,10.,20. ,0.958)),
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,20.,40. ,0.922)),
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,40.,150.,0.908)),
                                                                                     ),
                                                                single = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,0.8,25.,1000. ,0.896)),
                                                                                    cms.PSet(bin = cms.vdouble(0.8,2.1,25.,1000. ,0.810)),
                                                                                    cms.PSet(bin = cms.vdouble(2.1,2.4,25.,1000. ,0.610)),
                                                                                    )
                                                                ),
                                                      cms.PSet( lumi = cms.double(920.),#Run2011A2
                                                                double1 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,0.8,14.,20. ,0.974)),
                                                                                     cms.PSet(bin = cms.vdouble(0.0,0.8,20.,40. ,0.977)),
                                                                                     cms.PSet(bin = cms.vdouble(0.0,0.8,40.,150.,0.974)),
                                                                                     
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,14.,20. ,0.955)),
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,20.,40. ,0.952)),
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,40.,150.,0.954)),
                                                                                                 
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,14.,20. ,0.893)),
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,20.,40. ,0.881)),
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,40.,150.,0.893)),
                                                                                     ),
                                                                double2 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,0.8,10.,20. ,0.977)),
                                                                                     cms.PSet(bin = cms.vdouble(0.0,0.8,20.,40. ,0.978)),
                                                                                     cms.PSet(bin = cms.vdouble(0.0,0.8,40.,150.,0.975)),
                                                                                     
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,10.,20. ,0.964)),
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,20.,40. ,0.957)),
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,40.,150.,0.957)),
                                                                                     
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,10.,20. ,0.924)),
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,20.,40. ,0.909)),
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,40.,150.,0.913)),
                                                                                     ),
                                                                single = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,0.8,25.,1000. ,0.895)),
                                                                                    cms.PSet(bin = cms.vdouble(0.8,2.1,25.,1000. ,0.838)),
                                                                                    cms.PSet(bin = cms.vdouble(2.1,2.4,25.,1000. ,0.738)),
                                                                                    )
                                                                ),
                                                      cms.PSet( lumi = cms.double(995.),#Run2011A3
                                                                double1 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,0.8,14.,20. ,0.974)),
                                                                                     cms.PSet(bin = cms.vdouble(0.0,0.8,20.,40. ,0.977)),
                                                                                     cms.PSet(bin = cms.vdouble(0.0,0.8,40.,150.,0.974)),
                                                                                     
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,14.,20. ,0.955)),
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,20.,40. ,0.952)),
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,40.,150.,0.954)),
                                                                                 
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,14.,20. ,0.893)),
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,20.,40. ,0.881)),
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,40.,150.,0.893)),
                                                                                     ),
                                                                double2 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,0.8,10.,20. ,0.977)),
                                                                                     cms.PSet(bin = cms.vdouble(0.0,0.8,20.,40. ,0.978)),
                                                                                     cms.PSet(bin = cms.vdouble(0.0,0.8,40.,150.,0.975)),
                                                                                     
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,10.,20. ,0.964)),
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,20.,40. ,0.957)),
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,40.,150.,0.957)),
                                                                                     
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,10.,20. ,0.924)),
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,20.,40. ,0.909)),
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,40.,150.,0.913)),
                                                                                     ),
                                                                single = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,0.8,25.,1000. ,0.893)),
                                                                                    cms.PSet(bin = cms.vdouble(0.8,2.1,25.,1000. ,0.822)),
                                                                                    cms.PSet(bin = cms.vdouble(2.1,2.4,25.,1000. ,0.610)),
                                                                                    )
                                                                ),
                                                      cms.PSet( lumi = cms.double(1900.),#Run2011B1
                                                                double1 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,0.8,18.,20. ,0.977)),
                                                                                     cms.PSet(bin = cms.vdouble(0.0,0.8,20.,40. ,0.971)),
                                                                                     cms.PSet(bin = cms.vdouble(0.0,0.8,40.,150.,0.971)),
                                                                                     
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,18.,20. ,0.940)),
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,20.,40. ,0.934)),
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,40.,150.,0.941)),
                                                                                 
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,18.,20. ,0.738)),
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,20.,40. ,0.843)),
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,40.,150.,0.865)),
                                                                                     ),
                                                                double2 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,0.8, 9.,20. ,0.986)),
                                                                                     cms.PSet(bin = cms.vdouble(0.0,0.8,20.,40. ,0.973)),
                                                                                     cms.PSet(bin = cms.vdouble(0.0,0.8,40.,150.,0.972)),
                                                                                     
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1, 9.,20. ,0.948)),
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,20.,40. ,0.943)),
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,40.,150.,0.945)),
                                                                                     
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4, 9.,20. ,0.892)),
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,20.,40. ,0.866)),
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,40.,150.,0.883)),
                                                                                     ),
                                                                single = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,0.8,25.,1000. ,0.893)),
                                                                                    cms.PSet(bin = cms.vdouble(0.8,2.1,25.,1000. ,0.822)),
                                                                                    cms.PSet(bin = cms.vdouble(2.1,2.4,25.,1000. ,0.610)),
                                                                                    )
                                                                ),
                                                      cms.PSet( lumi = cms.double(600.),#Run2011B2
                                                                double1 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,0.8,18.,20. ,0.977)),
                                                                                     cms.PSet(bin = cms.vdouble(0.0,0.8,20.,40. ,0.971)),
                                                                                     cms.PSet(bin = cms.vdouble(0.0,0.8,40.,150.,0.971)),
                                                                                     
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,18.,20. ,0.940)),
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,20.,40. ,0.934)),
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,40.,150.,0.941)),
                                                                                 
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,18.,20. ,0.738)),
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,20.,40. ,0.843)),
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,40.,150.,0.865)),
                                                                                     ),
                                                                double2 = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,0.8, 9.,20. ,0.986)),
                                                                                     cms.PSet(bin = cms.vdouble(0.0,0.8,20.,40. ,0.973)),
                                                                                     cms.PSet(bin = cms.vdouble(0.0,0.8,40.,150.,0.972)),
                                                                                     
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1, 9.,20. ,0.948)),
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,20.,40. ,0.943)),
                                                                                     cms.PSet(bin = cms.vdouble(0.8,2.1,40.,150.,0.945)),
                                                                                     
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4, 9.,20. ,0.892)),
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,20.,40. ,0.866)),
                                                                                     cms.PSet(bin = cms.vdouble(2.1,2.4,40.,150.,0.883)),
                                                                                     ),
                                                                single = cms.VPSet( cms.PSet(bin = cms.vdouble(0.0,0.8,25.,30. ,0.911)),
                                                                                    cms.PSet(bin = cms.vdouble(0.0,0.8,30.,40. ,0.876)),
                                                                                    cms.PSet(bin = cms.vdouble(0.0,0.8,40.,150.,0.866)),

                                                                                    cms.PSet(bin = cms.vdouble(0.8,2.1,25.,30. ,0.794)),
                                                                                    cms.PSet(bin = cms.vdouble(0.8,2.1,30.,40. ,0.787)),
                                                                                    cms.PSet(bin = cms.vdouble(0.8,2.1,40.,150.,0.791)),
                                                                                    )
                                                                ),
                                                      )
                                  
                              )

 
HLTWeights = HLTWeightsKinFit.clone()
HLTWeights.src = "cmgDiMuonDiJetHiggs"
