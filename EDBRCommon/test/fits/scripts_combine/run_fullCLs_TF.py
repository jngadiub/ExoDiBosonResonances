#!/usr/bin/env python
import array, math
import os
import sys

channels=["ZZ"]

fullToys=True

# These values come from asymptotic limits
fromAsymptotic = dict()
fromAsymptotic[600]=0.3701
fromAsymptotic[650]=0.5254
fromAsymptotic[700]=0.7441
fromAsymptotic[750]=1.0039
fromAsymptotic[800]=1.3086
fromAsymptotic[850]=1.7422
fromAsymptotic[900]=2.2891
fromAsymptotic[950]=2.8828
fromAsymptotic[1000]=3.6406
fromAsymptotic[1050]=4.5781
fromAsymptotic[1100]=5.9219
fromAsymptotic[1150]=7.5312
fromAsymptotic[1200]=9.6562
fromAsymptotic[1250]=11.4688
fromAsymptotic[1300]=14.5625
fromAsymptotic[1350]=17.3125
fromAsymptotic[1400]=20.9062
fromAsymptotic[1450]=26.375
fromAsymptotic[1500]=34.375
fromAsymptotic[1550]=39.875
fromAsymptotic[1600]=49.125
fromAsymptotic[1650]=59.9375
fromAsymptotic[1700]=73.25
fromAsymptotic[1750]=88.625
fromAsymptotic[1800]=107.625
fromAsymptotic[1850]=132.969
fromAsymptotic[1900]=164.5
fromAsymptotic[1950]=201.25
fromAsymptotic[2000]=247.25
fromAsymptotic[2050]=300.25
fromAsymptotic[2100]=367.25
fromAsymptotic[2150]=446.5
fromAsymptotic[2200]=5.4830e+02
fromAsymptotic[2250]=6.7059e+02
fromAsymptotic[2300]=8.2016e+02
fromAsymptotic[2350]=1.0030e+03
fromAsymptotic[2400]=1.2268e+03
fromAsymptotic[2450]=1.5004e+03
fromAsymptotic[2500]=1.8351e+03

print ("VAI\n")
Njob = int(sys.argv[1])

for chan in channels:
    print "chan =",chan
    
    ### Check this sys.argv()
    masses = [sys.argv[2]]

    bins=[1]
    
    for bin in bins:
        
        for mass in masses:
            print "mass =",mass
            card = "comb_xzz.txt"
            
            midpoint = fromAsymptotic[int(mass)]
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

            ### Total number of toys is numtoys*numiters
            numtoys=20
            numiters=600

            submitname = "X"+chan+"."+str(mass)+"_"+chan+"_8TeV_channel"+str(bin)+"_limit"+str(int(position))+"_submit.src"
            submitlog = "Xvv.mX"+str(mass)+"_"+chan+"_8TeV_channel"+str(bin)+"_limit"+str(int(position))+"_submit.out"
            commandCombine = "combine ${CARD} -M HybridNew --frequentist --clsAcc 0 "+\
                             "-T "+str(numtoys)+" -i "+str(numiters)+" "+\
                             "--singlePoint "+point+" --rMin "+str(float(point)*0.33)+" --rMax "+str(float(point)*3.0)+\
                             " -s 100"+str(int(position))+" --saveHybridResult --saveToys -m "+\
                             str(mass) + " -n X"+str(chan)+"_CLs_"+str(mass)+"\n"        
            outputfile = open(submitname,'w')
            outputfile.write('#!/bin/bash\n')
            outputfile.write("CARD=comb_xzz.txt\n")
            outputfile.write("JOBNUM="+str(Njob)+"\n")
            outputfile.write("SEED=100"+str(Njob)+"\n")
            outputfile.write("MASS="+str(mass)+"\n\n")
            outputfile.write('echo "CARD is ${CARD}"\n')
            outputfile.write('echo "NJob is ${JOBNUM}"\n\n')
            outputfile.write("echo; echo "+commandCombine)
            outputfile.write(commandCombine)
            outputfile.write("ls -lhrt * ; echo ; echo -----; echo \n")                     
            outputfile.write("mv higgsCombineXZZ_CLs_"+str(mass)+".HybridNew.mH"+str(mass)+".100"+str(Njob)+".root output.root")
            outputfile.close()
            
            command="source "+submitname
            print(command)
            #os.system(command)
