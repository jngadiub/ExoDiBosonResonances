import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing ('standard')

options.setDefault('output','cmgTuple.root')
options.setDefault('secondaryOutput','histograms.root')

options.register ('infile',
                  'ggH200',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  "Input config")


options.register ('selection',
                  'presel',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  "selection level full/presel/none")

options.register ('lepton',
                  'both',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  "selection which channel for output: ele/mu/both")

options.register ('mcordata',
                  'MC',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  "sample contains data or MC: MC/DATAELE/DATAMU/DATASE/DATASM")

options.register ('content',
                  '',
                  VarParsing.multiplicity.list,
                  VarParsing.varType.string,
                  "selection output event content: ele/mu/jets/gen/all/trigger")

options.register ('json',
                  '',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  "json file to apply")



options.setDefault('content',['ele','mu','jets','gen','trigger'])
