# Auto generated configuration file
# using: 
# Revision: 1.381.2.2 
# Source: /local/reps/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: REDIGI --step DIGI,L1,DIGI2RAW,HLT:7E33v2 --conditions START53_V7A::All --pileup 2012_Summer_50ns_PoissonOOTPU --datamix NODATAMIXER --eventcontent RAWSIM --datatier GEN-SIM-RAW --no_exec --mc --filein tc_GENSIM.root --fileout tc_RAWSIM.root --python_filename ggH250_RAWSIM_cfg.py
import FWCore.ParameterSet.Config as cms

process = cms.Process('HLT')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mix_2012_Summer_50ns_PoissonOOTPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('HLTrigger.Configuration.HLT_7E33v2_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('RSWW_GENSIM.root')
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.381.2.2 $'),
    annotation = cms.untracked.string('REDIGI nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)

#pile-up samples
process.mix.input.fileNames = cms.untracked.vstring(
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/00DDA010-A769-E111-8A12-0030487D5EAF.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/02124719-B769-E111-8971-003048C66BBE.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/0261C34F-C969-E111-B0E6-003048C692E2.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/0413ADCF-DD69-E111-85EC-003048C69414.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/044C8BDF-516A-E111-AFFF-003048D436D2.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/06932BAC-BF69-E111-AE28-002481E14F2A.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/06CC68A1-1C6A-E111-A0A1-003048C69024.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/06E209A1-066A-E111-82E3-00266CF32FA0.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/06EA4A98-C169-E111-9C57-003048D436CA.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/0817CFCB-486A-E111-8DE4-002481E0DC66.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/08539AB8-0E6A-E111-8480-002481E0DC7C.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/08D878A3-F269-E111-BDBB-0030487F1A67.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/106F28F9-396A-E111-8BC8-0030487D8563.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/10A50890-F669-E111-B13D-00266CF32F90.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/1422D03E-EB69-E111-B20C-0030487D5DC3.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/1443AEC9-BB69-E111-946A-003048C6763A.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/1489B856-016A-E111-8B79-003048D4364C.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/14FC05E7-506A-E111-AEB5-003048C6617E.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/1603FBA2-236A-E111-BF33-003048D436CA.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/1823D4C7-F969-E111-B9EB-003048C662BC.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/1C15ECE1-176A-E111-9B10-0030487D5DA5.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/1E5A64FA-AE69-E111-8CA2-0030487D5D95.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/1E795D79-496A-E111-9B43-002481E0DC66.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/1E86426A-116A-E111-8B5C-003048F0E1E8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/20C853D3-436A-E111-A2E9-003048C662C8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/22B4B009-B469-E111-9E46-003048C69414.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/264C679B-066A-E111-A5C5-003048D3CA06.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/26A38F4B-D769-E111-AEF2-003048C69288.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/26B55055-A069-E111-A4AE-003048C692FE.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/2C5E6254-F469-E111-8689-0030487DE7C1.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/2CF15368-BB69-E111-97B9-003048C692B8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/302CDFB9-B869-E111-81CE-003048C693C8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/30C62000-056A-E111-B0D5-0030487F1C55.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/342AB83A-FD69-E111-A451-002481E1026A.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/34507336-E469-E111-BC8D-003048C65E48.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/387D67F1-216A-E111-87C6-003048D43986.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/3A62F816-0C6A-E111-81C4-0030487F16FB.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/3ADC386D-056A-E111-9D43-002481E107A8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/3C1C16AD-AB69-E111-A3FB-0030487D70FD.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/3CE2C301-F269-E111-8493-0030487F92A7.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/44382240-C569-E111-995E-0030487F1C4F.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/44B342A8-B269-E111-96B8-0030487E5179.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/462EC9C6-036A-E111-8EA8-00237DE0BED6.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/4A6D786C-166A-E111-9330-0030487F1BD3.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/4CA4C402-AB69-E111-A26D-0030487F132D.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/4E4BF5DE-2B6A-E111-AB17-003048D3C7DC.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/4EFBF2B4-4D6A-E111-9EF3-00266CFFA344.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/542B6297-346A-E111-BD12-0030487F1A57.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/56EBFDDD-446A-E111-82C3-003048C662C8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/56F6E04B-026A-E111-9E5C-0030487F1667.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/5C414259-B169-E111-BDEE-0030487D5E99.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/5E0418A7-FF69-E111-9647-0030487FA623.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/5E27471F-CC69-E111-8814-0030487D5EBB.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/5EE71620-406A-E111-BD2D-0030487FA607.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/6266263F-D069-E111-A604-003048C68A7C.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/628040C2-B969-E111-90D2-002481E0D12C.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/64BB6D26-096A-E111-9519-002481E94BFE.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/66372FCB-4C6A-E111-98B8-00266CFFA2D0.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/66BB8168-BB69-E111-B83F-003048C693BA.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/685046EB-E169-E111-8B2B-003048C662C8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/6C46744E-296A-E111-BB95-0030487F91D9.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/6CEC0A04-1E6A-E111-82A1-002481E0E450.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/6E9E85E2-4C6A-E111-83A4-00266CFFA344.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/74E3E534-BE69-E111-A08A-002481E94B4E.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/76875297-DA69-E111-AC9E-003048F0E812.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/7AD94AFD-B769-E111-A6AD-0030487F1735.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/7C7ACFE8-216A-E111-89D3-003048C692E4.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/7E53EEF5-0B6A-E111-8AC9-0030487F9307.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/7EE63124-A869-E111-A887-0030487D70FF.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/80529658-C969-E111-ADDD-003048C68A98.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/805712C4-FA69-E111-93AE-0030487FA627.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/8072929D-536A-E111-ABCE-003048D436D2.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/825F2F9D-0E6A-E111-8725-0030487F933D.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/82D93C41-BB69-E111-8F83-003048C662C8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/8A4A8EC9-AE69-E111-89A1-0030487E5399.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/8A5BDCE6-AF69-E111-92C3-0030487D710F.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/8C4A2FC4-C569-E111-AB81-003048D4DF80.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/8C6330C8-D469-E111-8FC0-0030487F9297.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/8C6C17CE-D969-E111-B3B0-0030487E54B5.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/8C869A1F-DC69-E111-84ED-003048D47746.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/90C8E789-246A-E111-9C6F-003048C6929C.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/92871992-2E6A-E111-B053-003048C68A80.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/92CA4E91-2A6A-E111-B7CD-0030487D86CB.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/92DAB54C-026A-E111-8EBF-003048C68A90.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/9431CE8C-0A6A-E111-BCFB-003048D3CD6C.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/948E8795-D269-E111-970B-0030487D5E9D.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/94969F28-B669-E111-A688-003048C66BBE.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/94C731D7-206A-E111-A6E3-003048D437EC.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/9657F470-466A-E111-802E-0030487F1651.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/96899128-B669-E111-A924-003048D439A8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/985D2B51-276A-E111-9CB9-0030487F1BD3.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/9A65E3AF-CF69-E111-83C5-003048D3C880.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/9C9395C0-B469-E111-B808-003048C69414.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/A069D2EE-146A-E111-94A7-0030487D5E81.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/A08583F7-B769-E111-B996-003048D4385C.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/A2337DEA-F769-E111-B3BC-00266CF32684.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/A24C6EF7-2B6A-E111-81C6-002481E1070E.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/A257077F-056A-E111-A60B-0030487E9881.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/A294B039-A669-E111-A7BC-0030487F132D.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/A4A8BFD3-4A6A-E111-8386-0030487FA627.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/A4FEB048-B269-E111-BE46-0030487D5E99.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/A6C8D0E6-DC69-E111-A395-003048C69414.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/A6EA9C81-D669-E111-B550-0030487EBB21.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/A845DFFA-F469-E111-9377-0030487D7105.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/AC30C382-126A-E111-8135-003048C693E6.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/AC7CD86E-1A6A-E111-A23B-0030487F92FF.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/B03DF83D-396A-E111-A6DB-0030487D8563.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/B08882CC-526A-E111-A4B0-003048D436D2.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/B0F3A4DA-A869-E111-9AB8-0030487F132D.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/B2B1207E-236A-E111-A9EA-0030487D5D8D.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/B4D29FDD-CC69-E111-8D55-003048C690A0.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/B4DCCC0E-056A-E111-B752-0030487FA4CD.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/B653D913-0C6A-E111-8E9A-0030487F91D7.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/B662044A-D869-E111-AD2A-003048C693F2.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/B8674D68-326A-E111-9801-003048F0E3B4.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/B8A7DDBC-036A-E111-9EED-003048F0E1AE.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/BC7E4506-B469-E111-AD61-003048C66180.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/BCB4FEF2-AF69-E111-A632-003048C692C0.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/BE0FD1DA-2E6A-E111-88F6-0030487D5E73.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/C0B2C629-096A-E111-9DE0-003048F02CB8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/C2E141A6-DB69-E111-9853-003048C6928C.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/C629CDD9-2F6A-E111-8178-0030487F1A47.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/C8BA49A5-276A-E111-82A9-003048F0E522.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/CA49807D-0A6A-E111-8288-0030487FA629.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/CCCA159E-A969-E111-BB0C-00266CF3336C.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/CCF0BC24-3C6A-E111-9012-002481E0D69C.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/D005B363-B269-E111-8D58-0030487D5DA9.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/D0E013FF-A369-E111-AA6C-0030487F132D.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/D2597F1C-AF69-E111-8334-0030487F1653.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/D2A57B10-C369-E111-B6C2-003048F0E3B8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/D2D51F24-3A6A-E111-86A3-0030487E4EB5.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/D424AFFE-C069-E111-A1A9-002481E0DDBE.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/D631E25C-AD69-E111-B211-0030487E54B7.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/D84954B6-C769-E111-B8DD-00266CF33100.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/DAE82F86-116A-E111-B35D-003048C693E6.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/DE7581FC-306A-E111-8F09-0030487F1657.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/DEFE63C2-B969-E111-97F2-003048C662B8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/E080FA03-3F6A-E111-997D-0030487FA607.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/E0845BAB-416A-E111-A621-0030487D5D4D.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/E2B2A325-BE69-E111-BD3D-002481E0D678.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/E842B1B3-FF69-E111-BD2E-0030487D8541.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/EC32F478-EF69-E111-A723-0030487F1A55.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/EE4A44F6-396A-E111-AC13-0030487F1651.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/EE7588ED-FB69-E111-8792-003048C692D4.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/EED45FB5-DF69-E111-B434-003048C66BBE.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/EEECCB2E-2D6A-E111-8F34-0030487F92B1.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/F0DB05D7-C369-E111-99E9-003048F0E3B8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/F261132F-F969-E111-BE95-0030487F1797.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/F2C72257-476A-E111-8049-0030487F1651.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/F86C6515-1E6A-E111-8F9B-00266CF2AACC.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/FA2633DF-AC69-E111-B760-0030487D5DA9.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0002/FEF2F4CC-0E6A-E111-96F6-0030487F1C57.root',
)

# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RAWSIMEventContent.outputCommands,
    fileName = cms.untracked.string('RSWW_RAWSIM.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-RAW')
    )
)

# Additional output definition

# Other statements
# customise the HLT menu for running on MC
from HLTrigger.Configuration.customizeHLTforMC import customizeHLTforMC
process = customizeHLTforMC(process)

process.GlobalTag.globaltag = 'START53_V7A::All'

# Path and EndPath definitions
process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.digitisation_step,process.L1simulation_step,process.digi2raw_step)
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.endjob_step,process.RAWSIMoutput_step])

