#!/usr/bin/env python
import array, math
import os
import sys

### Make changes here if it is needed
Njob = int(sys.argv[1])
mass = int(sys.argv[2])
fromAsymptotic = float(sys.argv[3])
cardname = sys.argv[4]
chan = sys.argv[5]

### Total number of toys is numtoys*numiters
numtoys=50
numiters=600
### End changes

print "chan =",chan
print "mass =",mass
print "card =",cardname

midpoint = fromAsymptotic
points = []
for i in range(10):
    stepsize = 0.05
    factor = (i+1)*0.05
    thispoint = midpoint*factor;
    strpoint = "%.5f" % thispoint
    points+=[strpoint]
for i in range(30):
    stepsize = 0.025
    factor = 0.525+stepsize*i;
    thispoint = midpoint*factor
    strpoint = "%.5f" % thispoint
    points+=[strpoint]
for i in range(10):
    stepsize = 0.075;
    factor = 1.325+stepsize*i;
    thispoint = midpoint*factor
    strpoint = "%.5f" % thispoint
    points+=[strpoint]
for i in range(6):
    stepsize = 8.0/6.0;
    factor = 3.0+stepsize*i;
    thispoint = midpoint*factor
    strpoint = "%.5f" % thispoint
    points+=[strpoint]
    
print points
    
### This is the trick - THIS is the change from CRAB to grid.
### Get the RIGHT point

position = Njob
point = points[position]


submitname = "X"+chan+"."+str(mass)+"_"+chan+"_8TeV_channel"+chan+"_limit"+str(int(position))+"_submit.src"
submitlog = "Xvv.mX"+str(mass)+"_"+chan+"_8TeV_channel"+chan+"_limit"+str(int(position))+"_submit.out"
commandCombine = "combine ${CARD} -M HybridNew --frequentist --clsAcc 0 "+\
                 "-T "+str(numtoys)+" -i "+str(numiters)+" "+\
                 "--singlePoint "+point+" --rMin "+str(float(point)*0.33)+" --rMax "+str(float(point)*3.0)+\
                 " -s 100"+str(int(position))+" --saveHybridResult --saveToys -m "+\
                 str(mass) + " -n X"+str(chan)+"_CLs_"+str(mass)+"\n"        

outputfile = open(submitname,'w')
outputfile.write('#!/bin/bash\n')
outputfile.write("CARD="+cardname+"\n")
outputfile.write("JOBNUM="+str(Njob)+"\n")
outputfile.write("SEED=100"+str(Njob)+"\n")
outputfile.write("MASS="+str(mass)+"\n\n")
outputfile.write('echo "CARD is ${CARD}"\n')
outputfile.write('echo "NJob is ${JOBNUM}"\n\n')
outputfile.write("echo; echo \"Executing the following command\: "+commandCombine+"   \"\n")
outputfile.write('echo \"Path to combine program: $( which combine )\"; echo ;\n')
outputfile.write(commandCombine)
outputfile.write("ls -lhrt * ; echo ; echo -----; echo \n")                     
outputfile.write("mv higgsCombineXZZ_CLs_"+str(mass)+".HybridNew.mH"+str(mass)+".100"+str(Njob)+".root output.root")
outputfile.close()

command="source "+submitname
print(command)
#os.system(command)
