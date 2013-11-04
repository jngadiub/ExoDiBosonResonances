import os

#--------------------------------------------------
# EDIT HERE

masses =[800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400, 2500]
#masses =[1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400, 2500]
#masses =[1800, 1900, 2000, 2100, 2200, 2300, 2400, 2500]
#masses =[1000]

#NewDir = "mWWFit_DATA_06_06_2013_Scale1_From1000_v1"
#NewDir = "mWWFit_DATA_16_07_2013_Scale1_FitFrom700_v2"
NewDir = "mWWFit_DATA_04_11_2013_Scale1_FitFrom700_v1"
#NewDir = "mWWFit_DATASB_M700_Bin100_v1"
#NewDir = "mWWFit_DATASB_M700_Bin50_v1"
#NewDir = "mWWFit_MCSB_M700_Bin100_v1"
#NewDir = "mWWFit_MCSB_M700_Bin50_v1"
#NewDir = "mWWFit_DATA_SB_FitFrom700_09_08_2013"

#--------------------------------------------------

#1) create or cleaning
print "create datacards"
os.system("mkdir -p datacards/")
print "create workspaces"
os.system("mkdir -p workspaces/")
print "create plots"
os.system("mkdir -p plots/")
print "cleaning datacards"
os.system("rm -rf datacards/*")
print "cleaning workspaces"
os.system("rm -f workspaces/*")
print "cleaning plots"
os.system("rm -f plots/*")

#2) make workspaces

for mass in masses:
  os.system("mkdir -p datacards/"+str(mass))
  os.system("root -b -q 'R2JJFitter.cc("+str(mass)+")'\n")

#3) copy workspaces

os.system("mkdir -p "+str(NewDir))
print "copying datacards in "+str(NewDir)
os.system("cp -r datacards "+str(NewDir)+"/")
print "copying workspaces in "+str(NewDir)
os.system("cp -r workspaces "+str(NewDir)+"/")
print "copying plots in "+str(NewDir)
os.system("cp -r plots "+str(NewDir)+"/")
print "copying data in "+str(NewDir)
os.system("cp -r data "+str(NewDir)+"/")

#4) copy scripts for limits

print "copying scripts_combine in "+str(NewDir)
os.system("cp scripts_combine/* "+str(NewDir)+"/datacards/")
