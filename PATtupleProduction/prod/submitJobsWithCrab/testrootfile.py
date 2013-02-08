from ROOT import gSystem
from ROOT import TFile
import os, sys

#filename="root://eoscms//eos/cms/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/shuai/EDBR_PATtuple_edbr_zz_20130113_Summer12MC_WToLNuBinsPtW_MADGRAPH_20130122_141244/shuai/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/EDBR_PATtuple_edbr_zz_20130113/d5e30eea881ae7f2099773872ad1f309/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph__Summer12_DR53X-PU_S10_START53_V7A-v1__AODSIM_1096_4_mdK.root"
#filename="root://eoscms//eos/cms/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/shuai/EDBR_PATtuple_edbr_zz_20130113_SingleMu_Run2012B-13Jul2012-v1_20130130_170745/shuai/SingleMu/EDBR_PATtuple_edbr_zz_20130113_SingleMu_Run2012B-13Jul2012-v1/d2baed65106e8614f13549c75c711519/SingleMu__Run2012B-13Jul2012-v1__AOD_999_2_XxF.root"

filename=sys.argv[1]

file= TFile.Open(filename,"read")
if  not file or file.IsZombie():
    print "0"
else:
    print "1"
