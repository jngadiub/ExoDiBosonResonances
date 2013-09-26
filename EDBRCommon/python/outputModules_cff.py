### read steering options from cmd file:
from PhysicsTools.PatAlgos.tools.helpers import *

from ExoDiBosonResonances.EDBRCommon.cmdLine import options
####options.parseArguments() # to be decommented only if you copy all this module in the main

out = cms.OutputModule("PoolOutputModule",
                              fileName = cms.untracked.string("cmgTuple.root"),
                              outputCommands = cms.untracked.vstring('drop *',
                                                                     'keep *_*_*_CMG',
                                                                     'keep *_patMetShiftCorrected_*_*'
                                                                     )
                              )


#### filter contents according to option in steering file 
for content in options.content:
    
    if content == "all":
        out.outputCommands.append( 'keep *_*_*_*' )

    if content == "ele":
        out.outputCommands.append( 'keep *_*PatElectrons*_*_*' ) # we keep the pat::Muon

    if content == "mu":
        out.outputCommands.append( 'keep *_triggeredPatMuons_*_*' ) 
        out.outputCommands.append( 'keep *_selectedPatMuons_*_*' ) 

    if content == "gen":
        out.outputCommands.append( 'keep *_genParticles_*_*' ) 
        out.outputCommands.append( 'keep *_ak5GenJets_*_*' )
        out.outputCommands.append( 'keep GenEventInfoProduct*_*_*_*' )


    if content == "jets":
#        out.outputCommands.append( 'keep patJets_customPFJets*_*_*' ) 
#        out.outputCommands.append( 'keep *_customPFJetsPUFastjet_*_*' ) 
        out.outputCommands.append( 'keep *_custom*Jets*_*_*' )
        out.outputCommands.append( 'keep patJets_selected*_*_*' ) 
        out.outputCommands.append( 'keep *_*_rho_*' )
        out.outputCommands.append( 'keep *_patMETs*_*_*' )
        out.outputCommands.append( 'keep *_addPileupInfo_*_*')

    if content == "trigger":
        out.outputCommands.append( 'keep *_TriggerResults_*_*' )
        out.outputCommands.append( 'keep *_*PrimaryVertices_*_*' )


#### fileter events according to option in steering file 
if options.selection != "none":
    out.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring() )

if options.selection == "full":
    if options.lepton == "both" or options.lepton == "ele":
        out.SelectEvents.SelectEvents.append("cmgEDBRZZEle")
#        out.SelectEvents.SelectEvents.append("EDBRZZEESideband")
    if options.lepton == "both" or options.lepton == "mu":
        out.SelectEvents.SelectEvents.append("cmgEDBRZZMu")
#        out.SelectEvents.SelectEvents.append("EDBRZZMMSideband")
if options.selection == "presel":
    if options.lepton == "both" or options.lepton == "ele":
        out.SelectEvents.SelectEvents.append("preselElePath")
        out.SelectEvents.SelectEvents.append("preselEleMergedPath")
    if options.lepton == "both" or options.lepton == "mu":
        out.SelectEvents.SelectEvents.append("preselMuPath")
        out.SelectEvents.SelectEvents.append("preselMuMergedPath")
