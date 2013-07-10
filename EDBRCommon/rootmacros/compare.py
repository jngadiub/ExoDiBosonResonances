
##SIDEBAND

#ele
#fileOne = "/afs/cern.ch/work/s/santanas/Workspace/EXOVV_2012/AN-13-045/DataVsMCPlots/plots_WW_ele_sideband_bvetoM_ALLP_sync_pickEventList_group1.txt"
#fileTwo = "/afs/cern.ch/work/s/santanas/Workspace/EXOVV_2012/AN-13-045/DataVsMCPlots/plots_WW_ele_sideband_bvetoM_ALLP_sync_pickEventList_group2.txt"

#mu
#fileOne = "/afs/cern.ch/work/s/santanas/Workspace/EXOVV_2012/AN-13-045/DataVsMCPlots/plots_WW_mu_sideband_bvetoM_ALLP_sync_pickEventList_group1.txt"
#fileTwo = "/afs/cern.ch/work/s/santanas/Workspace/EXOVV_2012/AN-13-045/DataVsMCPlots/plots_WW_mu_sideband_bvetoM_ALLP_sync_pickEventList_group2.txt"


##SIGNAL

#ele
#fileOne = "/afs/cern.ch/work/s/santanas/Workspace/EXOVV_2012/AN-13-045/DataVsMCPlots/plots_WW_ele_signal_bvetoM_ALLP_sync_pickEventList_group1.txt"
#fileTwo = "/afs/cern.ch/work/s/santanas/Workspace/EXOVV_2012/AN-13-045/DataVsMCPlots/plots_WW_ele_signal_bvetoM_ALLP_sync_pickEventList_group2.txt"

#mu
fileOne = "/afs/cern.ch/work/s/santanas/Workspace/EXOVV_2012/AN-13-045/DataVsMCPlots/plots_WW_mu_signal_bvetoM_ALLP_sync_pickEventList_group1.txt"
fileTwo = "/afs/cern.ch/work/s/santanas/Workspace/EXOVV_2012/AN-13-045/DataVsMCPlots/plots_WW_mu_signal_bvetoM_ALLP_sync_pickEventList_group2.txt"


print ""
print "File *One* is ", fileOne
print "File *Two* is ", fileTwo

ListOne = open(fileOne).readlines()
ListTwo = open(fileTwo).readlines()

print ""
print "Total number of entries in One: ", len(ListOne)
print "Total number of entries in Two: ", len(ListTwo)

ListOneSet = set(ListOne)
ListTwoSet = set(ListTwo)

#print ListOne
#print ListTwo

BothOneAndTwo = ListOneSet.intersection(ListTwoSet)
OneMinusTwo = ListOneSet.difference(ListTwoSet)
TwoMinusOne = ListTwoSet.difference(ListOneSet)
SymmetricDiff_OneTwo = ListOneSet.symmetric_difference(ListTwoSet)

print ""
print "BothOneAndTwo : " , len(BothOneAndTwo) , " events present in **both One and Two**"
print BothOneAndTwo

print ""
print "OneMinusTwo : " , len(OneMinusTwo) , " events present in **One but not in Two**"
print OneMinusTwo

print ""
print "TwoMinusOne : " , len(TwoMinusOne) , " events present in **Two but not in One**"
print TwoMinusOne
print ""

#print ""
#print "SymmetricDiff_OneTwo : Sum of the events not in common"
#print SymmetricDiff_OneTwo
#print ""


