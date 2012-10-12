# Auto generated configuration file
# using: 
# Revision: 1.303.2.7 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: -s GEN,SIM --conditions auto:mc --eventcontent RAWSIM --datatier GEN-SIM -n 10 --no_exec --mc --fileout zprime_GENSIM.root --filein run_01_unweighted_events.lhe --filetype LHE
import FWCore.ParameterSet.Config as cms

process = cms.Process('SIM')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic8TeVCollision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
#process.source = cms.Source("LHESource",
#    fileNames = cms.untracked.vstring(
#	'file:gww021000.lhe'
#	)
#)
process.source = cms.Source("EmptySource")



process.options = cms.untracked.PSet(
)

from Configuration.Generator.PythiaUEZ2Settings_cfi import *

process.generator = cms.EDFilter("Pythia6GeneratorFilter",
	pythiaHepMCVerbosity = cms.untracked.bool(False),
	maxEventsToPrint = cms.untracked.int32(0),
	pythiaPylistVerbosity = cms.untracked.int32(0),
	filterEfficiency = cms.untracked.double(1),
	comEnergy = cms.double(8000.0),
	crossSection = cms.untracked.double(1.671),
        PythiaParameters = cms.PSet(
	        pythiaUESettingsBlock,
                processParameters = cms.vstring(
		        'MSEL = 0',
		        'MSUB(391) = 1',
		        'MSUB(392) = 1',
		        'PMAS(347,1) = 1000',
		        'PARP(50) = 1.08', 
		        '5000039:ALLOFF',
		        '5000039:ONIFANY 24',
        ),
		parameterSets = cms.vstring(
		        'pythiaUESettings',
			'processParameters')
	)
)

#process.generator = cms.EDFilter("Pythia6HadronizerFilter",
#    ExternalDecays = cms.PSet(
#        Tauola = cms.untracked.PSet(
#            UseTauolaPolarization = cms.bool(True),
#            InputCards = cms.PSet(
#                mdtau = cms.int32(0),
#                pjak2 = cms.int32(0),
#                pjak1 = cms.int32(0)
#            )
#        ),
#        parameterSets = cms.vstring('Tauola')
#    ),
#    pythiaPylistVerbosity = cms.untracked.int32(1),
#    filterEfficiency = cms.untracked.double(1.0),
#    pythiaHepMCVerbosity = cms.untracked.bool(False),
#    comEnergy = cms.double(8000.0),
#    maxEventsToPrint = cms.untracked.int32(0),
#    PythiaParameters = cms.PSet(
#        pythiaUESettings = cms.vstring('MSTU(21)=1     ! Check on possible errors during program execution', 
#            'MSTJ(22)=2     ! Decay those unstable particles', 
#            'PARJ(71)=10 .  ! for which ctau  10 mm', 
#            'MSTP(33)=0     ! no K factors in hard cross sections', 
#            'MSTP(2)=1      ! which order running alphaS', 
#            'MSTP(51)=10042 ! structure function chosen (external PDF CTEQ6L1)', 
#            'MSTP(52)=2     ! work with LHAPDF', 
#            'PARP(82)=1.921 ! pt cutoff for multiparton interactions', 
#            'PARP(89)=1800. ! sqrts for which PARP82 is set', 
#            'PARP(90)=0.227 ! Multiple interactions: rescaling power', 
#            'MSTP(95)=6     ! CR (color reconnection parameters)', 
#            'PARP(77)=1.016 ! CR', 
#            'PARP(78)=0.538 ! CR', 
#            'PARP(80)=0.1   ! Prob. colored parton from BBR', 
#            'PARP(83)=0.356 ! Multiple interactions: matter distribution parameter', 
#            'PARP(84)=0.651 ! Multiple interactions: matter distribution parameter', 
#            'PARP(62)=1.025 ! ISR cutoff', 
#            'MSTP(91)=1     ! Gaussian primordial kT', 
#            'PARP(93)=10.0  ! primordial kT-max', 
#            'MSTP(81)=21    ! multiple parton interactions 1 is Pythia default', 
#            'MSTP(82)=4     ! Defines the multi-parton model'),
#        processParameters = cms.vstring('MSEL=0           ! User defined processes', 
#            'PMAS(5,1)=4.75   ! b quark mass', 
#            'PMAS(6,1)=172.5  ! t quark mass', 
##            'MDME(210,1)=0    ! Higgs decay into dd', 
##            'MDME(211,1)=0    ! Higgs decay into uu', 
##            'MDME(212,1)=0    ! Higgs decay into ss', 
##            'MDME(213,1)=0    ! Higgs decay into cc', 
##            'MDME(214,1)=0    ! Higgs decay into bb', 
##            'MDME(215,1)=0    ! Higgs decay into tt', 
##            'MDME(216,1)=0    ! Higgs decay into bbbar prime', 
##            'MDME(217,1)=0    ! Higgs decay into ttbar prime', 
##            'MDME(218,1)=0    ! Higgs decay into e e', 
##            'MDME(219,1)=0    ! Higgs decay into mu mu', 
##            'MDME(220,1)=0    ! Higgs decay into tau tau', 
##            'MDME(221,1)=0    ! Higgs decay into tau tau prime', 
##            'MDME(222,1)=0    ! Higgs decay into g g', 
##            'MDME(223,1)=0    ! Higgs decay into gam gam', 
##            'MDME(224,1)=0    ! Higgs decay into gam Z', 
##            'MDME(225,1)=0    ! Higgs decay into Z Z', 
##            'MDME(226,1)=1    ! Higgs decay into W W', 
##            'MDME(190,1)=4    ! W decay into dbar u', 
##            'MDME(191,1)=4    ! W decay into dbar c', 
##            'MDME(192,1)=4    ! W decay into dbar t', 
##            'MDME(194,1)=4    ! W decay into sbar u', 
##            'MDME(195,1)=4    ! W decay into sbar c', 
##            'MDME(196,1)=4    ! W decay into sbar t', 
##            'MDME(198,1)=4    ! W decay into bbar u', 
##            'MDME(199,1)=4    ! W decay into bbar c', 
##            'MDME(200,1)=4    ! W decay into bbar t', 
##            'MDME(206,1)=5    ! W decay into e+ nu_e', 
##            'MDME(207,1)=5    ! W decay into mu+ nu_mu', 
##            'MDME(208,1)=5    ! W decay into tau+ nu_tau'
#              ),
#        parameterSets = cms.vstring('pythiaUESettings', 
#            'processParameters')
#    )
#)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    annotation = cms.untracked.string('-s nevts:10'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RAWSIMEventContent.outputCommands,
    fileName = cms.untracked.string('RSWW_GENSIM.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

# Additional output definition

# Other statements
#process.GlobalTag.globaltag = 'MC_42_V12::All'
process.GlobalTag.globaltag = 'START53_V7A::All'
process.ProductionFilterSequence = cms.Sequence(process.generator)

# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.endjob_step,process.RAWSIMoutput_step)

# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.ProductionFilterSequence * getattr(process,path)._seq 
