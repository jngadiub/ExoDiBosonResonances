import FWCore.ParameterSet.Config as cms

#PUWeights = cms.EDProducer("PUWeightProducer",
#                           filenameData = cms.FileInPath("ExoDiBosonResonances/EDBRCommon/data/PUDist_Run2012AB_Truth.root"),
#                           hnameData    = cms.string("pileup"),
#                           filenameMC   = cms.FileInPath("ExoDiBosonResonances/EDBRCommon/data/PUDist_Summer12MC_Extended.root"),
#                           hnameMC      = cms.string("PUS7_Distr"))

PUWeights = cms.EDProducer("PUWeightProducer",
                           filenameData = cms.FileInPath("ExoDiBosonResonances/EDBRCommon/data/PUDist_Run2012Full_Truth_69p4mb.root"),
                           hnameData    = cms.string("pileup"),
                           filenameMC   = cms.FileInPath("ExoDiBosonResonances/EDBRCommon/data/PUDist_Summer12MC_S10.root"),
                           hnameMC      = cms.string("PUS10_Distr"))

PUWeights2012A = cms.EDProducer("PUWeightProducer",
                                filenameData = cms.FileInPath("ExoDiBosonResonances/EDBRCommon/data/PUDist_Run2012A_Truth.root"),
                                hnameData    = cms.string("pileup"),
                                filenameMC   = cms.FileInPath("ExoDiBosonResonances/EDBRCommon/data/PUDist_Summer12MC_Extended.root"),
                                hnameMC      = cms.string("PUS7_Distr"))


PUWeights2012B = cms.EDProducer("PUWeightProducer",
                         filenameData = cms.FileInPath("ExoDiBosonResonances/EDBRCommon/data/PUDist_Run2012AB_Truth.root"),   ### B only???
                         hnameData    = cms.string("pileup"),
                         filenameMC   = cms.FileInPath("ExoDiBosonResonances/EDBRCommon/data/PUDist_Summer12MC_Extended.root"),
                         hnameMC      = cms.string("PUS7_Distr"))


PUseq = cms.Sequence(PUWeights + PUWeights2012A + PUWeights2012B)

