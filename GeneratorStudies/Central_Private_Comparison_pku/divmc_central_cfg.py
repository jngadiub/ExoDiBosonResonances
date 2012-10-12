import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
'file:/eos/uscms/store/user/qliphy/div/44CD52D2-C0FC-E111-81E6-00215E21DF18.root',
'file:/eos/uscms/store/user/qliphy/div/A6F4065C-E0FC-E111-A313-00215E93ED9C.root',
'file:/eos/uscms/store/user/qliphy/div/B4771A84-E1FC-E111-887D-00215E222760.root',
'file:/eos/uscms/store/user/qliphy/div/D2462A6C-E3FC-E111-958F-E41F131815B8.root',
'file:/eos/uscms/store/user/qliphy/div/F68EBDB9-EBFC-E111-A5BC-00215E2287E4.root',
    )
)

process.demo = cms.EDAnalyzer('DiVMC')


#process.TFileService.fileName = cms.string('new.root')

OUTPUT_FILE_NAME = "central.root"
process.TFileService = cms.Service(
    "TFileService", fileName = cms.string( OUTPUT_FILE_NAME ),
    closeFileFast = cms.untracked.bool(True)
)

process.options = cms.untracked.PSet(
SkipEvent = cms.untracked.vstring('ProductNotFound')
)

process.p = cms.Path(process.demo)
