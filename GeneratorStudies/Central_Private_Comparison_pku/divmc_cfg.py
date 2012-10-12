import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        #'file:/eos/uscms/store/user/qliphy/div/44CD52D2-C0FC-E111-81E6-00215E21DF18.root',
		'/store/user/qliphy/RSWW_1000_02_SIM/RSWW_1000_02_AODSIM/c8f8ed334db8a7d6f56c62266b1dfa5b/RSWW_AODSIM_189_1_ry0.root'
    )
)

process.demo = cms.EDAnalyzer('DiVMC')


#process.TFileService.fileName = cms.string('new.root')

OUTPUT_FILE_NAME = "private_test.root"
process.TFileService = cms.Service(
    "TFileService", fileName = cms.string( OUTPUT_FILE_NAME ),
    closeFileFast = cms.untracked.bool(True)
)

process.options = cms.untracked.PSet(
SkipEvent = cms.untracked.vstring('ProductNotFound')
)

process.p = cms.Path(process.demo)
