import os

hinzpath ="/afs/cern.ch/work/m/mmozer/VV/CMGTools/CMSSW_5_3_3_patch3/src/ExoDiBosonResonances/EDBRCommon/test/fits/cards_hinz/"
zzpath   = "/afs/cern.ch/work/m/mmozer/VV/CMGTools/CMSSW_5_3_3_patch3/src/ExoDiBosonResonances/EDBRCommon/test/fits/DataCards_XZZ_EXPERIMENTAL/"


#read cross section file
xs=dict()
xsfile= open(zzpath+"../../../data/xsect_BulkG_ZZ_c0p5_xsect_in_pb.txt")
for line in iter(xsfile):
    tokens = line.split()
    xs[tokens[0]]=float(tokens[1])

f = open('massesComb.txt')


for line in iter(f):
    mass = line.rstrip()
    categories=["0"]#,"1","2"] #use only cat 0 until unblinding

    for cat in categories:
        cardname =  "Xvv.mX"+str(mass)+".0_ZZ_8TeV_channel"+ cat +".txt"
        cardnameWW =  "Xvv.mX"+str(mass)+".0_WW_8TeV_channel"+ cat +".txt"
        print cardname
        cardin   = open(hinzpath+"/datacards/"+cardname)
        cardinWW = open(hinzpath+"/datacards/"+cardnameWW)
        cardout  = open(zzpath+mass+"/"+cardname,"w")

        linesWW=cardinWW.readlines()
        linenr=-1
        #print "\n ++++++++++ \n\n" 
        section = 0 # sections of the datacard: 0=intro, 1=shapes, 2=obs, 3= proc, 4 = syst
        for cardline in iter(cardin):
            linenr+=1
            #print linesWW[linenr],
            
            if cardline.find("#")==0: # copy comments straight away
                cardout.write(cardline)
                continue
            if "----" in cardline: # next section
                cardout.write(cardline)
                section += 1
                continue
                
            if section == 0: # copy introduction and add one process (WW)
                cardout.write(cardline)
                continue
                    
            if section == 1: # section with shape defintitions. Copy root files, Add WW
                cardline = cardline.replace("MggSigWW","sigxww")
                cardline = cardline.replace("MggSigZZ","sigxzz")
                cardline = cardline.replace("MggSigWZ","sigxwz")
                substrings = cardline.split()
                newline = ""
                for substr in substrings:
                    if("root" in substr):
                        tokens = substr.split("/")
                        newline += " "+tokens[-1]                        
                        copytstring = "cp "+hinzpath+"/workspaces/"+tokens[-1]+" " +zzpath + mass + "/"+tokens[-1]
                        os.system(copytstring)                        
                    else:
                        newline +=" " + substr
                newline+="\n"
                cardout.write(newline)
                continue
                            

            if section == 2 : # section with number of events by cateogry
                cardout.write(cardline)
                continue
 
            if section == 3 : # section with expected events by process. Add WW column here
                cardline = cardline.replace("MggSigWW","sigxww")
                cardline = cardline.replace("MggSigZZ","sigxzz")
                cardline = cardline.replace("MggSigWZ","sigxwz")
                substrings = cardline.split()
                newline = ""
                            
                     
                if substrings[0]=="rate": # rescale signal rate to be in line with ZZ, add WW contrib with facotr 3
                    #print str(linenr)+ linesWW[linenr]
                    wwstrings = linesWW[linenr].split()
                    
                    newline = substrings[0] +"\t\t\t" + str(float(wwstrings[1])*2.*xs[mass]/0.007653)  +"\t\t"+ str(float(substrings[2])/0.007653*xs[mass])  +"\t\t0\t\t" + substrings[4]+"\n"
                else:
                    newline = cardline

                cardout.write(newline)
                continue

            if section == 4: # systematic errors
                cardout.write(cardline)
                continue

                
                                            
        cardout.close()
        cardin.close()
                                            
f.close()




