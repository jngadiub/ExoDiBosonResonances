import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
		# 'file:./smallsample/RSWW_AODSIM_94_1_FZZ.root',
		# 'file:./smallsample/RSWW_AODSIM_95_1_Zeh.root',
		# 'file:./smallsample/RSWW_AODSIM_96_1_te7.root',
		# 'file:./smallsample/RSWW_AODSIM_97_1_lAn.root',
		# 'file:./smallsample/RSWW_AODSIM_98_1_R4N.root',
		# 'file:./smallsample/RSWW_AODSIM_99_1_OuN.root',
		# 'file:./smallsample/RSWW_AODSIM_9_1_8uc.root'
		'file:./smallsample_750/D6CF7864-8CF8-E111-8D55-02215E94EE47.root'
	)
)

process.demo = cms.EDAnalyzer('ZZ_ana')



OUTPUT_FILE_NAME = "Bulk_ZZ_genlevel.root"
process.TFileService = cms.Service(
    "TFileService", fileName = cms.string( OUTPUT_FILE_NAME ),
    closeFileFast = cms.untracked.bool(True)
)


process.p = cms.Path(process.demo)
