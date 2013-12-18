import FWCore.ParameterSet.Config as cms

process = cms.PSet(
    fileNames	= cms.vstring("../prod/submitJobsWithCrab/patTuple.root"),                    
    maxEvents	= cms.int32(-1),
    outputEvery = cms.uint32(10),
    outputFile  = cms.string('test.root'),
    )

