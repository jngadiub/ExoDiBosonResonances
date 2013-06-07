import os

#--------------------------------------------------
# EDIT HERE

masses=[700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1650,1700,1750,1800,1850,1900,1950,2000]
#masses =[700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000]
#masses =[1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400, 2500]
#masses =[1800, 1900, 2000, 2100, 2200, 2300, 2400, 2500]
#masses =[1000]

NewDir = "mZZFit_data_02_06_2013_V3";

#--------------------------------------------------

#1) cleaning 

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
