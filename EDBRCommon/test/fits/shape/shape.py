#!/usr/bin/env python


import ROOT as root
import argparse
import os,sys
import math

#delta-R computation copied from CMSSW

def deltaPhi(phi1, phi2):

    M_PI = 3.1415926535897
    
    result = phi1 - phi2
    while (result > M_PI):
        result -= 2*M_PI
    while (result <= -M_PI):
        result += 2*M_PI

    return result;


def deltaR2(eta1,phi1,eta2,phi2):
    deta = eta1 - eta2
    dphi = deltaPhi(phi1, phi2)
    return deta*deta + dphi*dphi

def deltaR(eta1, phi1, eta2, phi2):
    return math.sqrt(deltaR2(eta1, phi1, eta2, phi2))



# some utility functions to deduce signal types from filenames
def desc(filepath):
    descriptor =  os.path.basename(filepath).replace("treeEDBR_","").replace(".root","")
    return descriptor

def checkfile(descriptor):
    retval = True
    if descriptor.find("RSG")==-1 and descriptor.find("BulkG")==-1 :
        retval=False
        print "Warning: The file: "+  descriptor + " doesn't look like a signal sample" 
        print "it should contain RSG or BulkG in the filename."    
    return retval
        
def deduceBosonType(filepath):
    descriptor = desc(filepath)
    boson = "X"
    if descriptor.find("ZZ")!= -1:
        boson = "Z"
    if descriptor.find("WW")!= -1:
        boson = "W"
    if boson=="X":
        print "ERROR: Cannot deduce boson type for file: "+ filepath
        print "it should contain ZZ or WW in the filename."
    exit

    return boson


# make the PDF, currently uses one functional form for all channels, may need extension  
def ConstructPdf(workspace):
    #MatchedFuncBase   = root.RooVoigtianShape("MatchedFunc","MatchedFunc",workspace.var("mZZ"),workspace.var("mean_match"),workspace.var("sigma_match"),workspace.var("alpha_match"),workspace.var("n_match"),workspace.var("width_match"),True)
    MatchedFuncBase   = root.RooDoubleCB("MatchedFunc","MatchedFunc",workspace.var("mZZ"),workspace.var("mean_match"),workspace.var("sigma_match"),workspace.var("alpha1_match"),workspace.var("n1_match"),workspace.var("alpha2_match"),workspace.var("n2_match")) 
    #MatchedFuncBase   = root.RooVoigtian("MatchedFunc","MatchedFunc",workspace.var("mZZ"),workspace.var("mean_match"),workspace.var("sigma_match"),workspace.var("width_match"))
    #MatchedFuncBase   = root.RooCBShape("MatchedFunc","MatchedFunc",workspace.var("mZZ"),workspace.var("mean_match"),workspace.var("sigma_match"),workspace.var("alpha_match"),workspace.var("n_match") )
    totalnorm = root.RooRealVar("totalnorm","totalnorm",100,1,100000)
    matchnorm = root.RooProduct("matchnorm","matchnorm",root.RooArgSet(totalnorm,workspace.var("machfrac")))
    MatchedFunc = root.RooExtendPdf("ExtMatchedFunc","ExtMatchedFunc",MatchedFuncBase,matchnorm)
    
    UnMatchedFuncBase = root.RooCBShape("UnMatchedFunc","UnMatchedFunc",workspace.var("mZZ"),workspace.var("mean_unmatch"),workspace.var("sigma_unmatch"),workspace.var("alpha_unmatch"),workspace.var("n_unmatch") )
    unmatchnorm = root.RooFormulaVar("unmatchnorm","unmatchnorm","@0*(1-@1)",root.RooArgList(totalnorm,workspace.var("machfrac")))
    UnMatchedFunc = root.RooExtendPdf("ExtUnMatchedFunc","ExtUnMatchedFunc",UnMatchedFuncBase,unmatchnorm)

    FitFunc = root.RooSimultaneous("FitFunc","FitFunc", root.RooArgList(UnMatchedFunc,MatchedFunc), workspace.cat("match"))

    getattr(workspace,'import')(FitFunc)

# set up the variables to be used in the fit. Will need ot be extended if we use different funcional forms for different channels
def defineVars(descriptor,njets,workspace,plotonly):
    # matched parameters
    mean_match = root.RooRealVar("mean_match","mean_match",0)
    sigma_match = root.RooRealVar("sigma_match","sigma_match",0)
    width_match = root.RooRealVar("width_match","width_match",0)
    alpha1_match = root.RooRealVar("alpha1_match","alpha1_match",0)
    n1_match = root.RooRealVar("n1_match","n1_match",0)
    alpha2_match = root.RooRealVar("alpha2_match","alpha2_match",0)
    n2_match = root.RooRealVar("n2_match","n2_match",0)
    
    # unmatched parameters
    mean_unmatch = root.RooRealVar("mean_unmatch","mean_unmatch",0)
    sigma_unmatch = root.RooRealVar("sigma_unmatch","sigma_unmatch",0)
    alpha_unmatch = root.RooRealVar("alpha_unmatch","alpha_unmatch",0)
    n_unmatch = root.RooRealVar("n_unmatch","n_unmatch",0)

    # realtive fractions
    machfrac = root.RooRealVar("machfrac","machfrac",0)

    fitpars   = root.RooArgSet(mean_match,sigma_match,width_match,mean_unmatch,sigma_unmatch,alpha_unmatch,n_unmatch,machfrac)
    
    fitpars.add(alpha1_match)
    fitpars.add(n1_match)
    fitpars.add(alpha2_match)
    fitpars.add(n2_match)

  
    workspace.defineSet("pars",fitpars,True)

    filename = "pars/"
    if plotonly:
        filename +="outpars_"
    else:
        filename +="inpars_"
    filename += descriptor  +"_" +  str( njets ) +  ".config"
    print 'Initializing par values from ',filename
    print 'mean-match before reading from file: ',workspace.var("mean_match").getVal()
    workspace.set("pars").readFromFile(filename)
    print 'mean-match after reading from file: ',workspace.var("mean_match").getVal()


    windowSF=4.0
    if workspace.var("mean_match").getVal()>1500 :
        windowSF=6.0
    mzzLow=workspace.var("mean_match").getVal()- workspace.var("sigma_match").getVal()*windowSF
    if mzzLow < 500.0 :
        mzzLow=500.0
    mzzHigh=workspace.var("mean_match").getVal()+ workspace.var("sigma_match").getVal()*windowSF
    if mzzHigh > 3000.0 :
        mzzHigh=3000.0
        
    binWidth=20.0; ### 20 gev bins
    mzzNBins=round( (mzzHigh-mzzLow)/binWidth );
    
    mzz = root.RooRealVar("mZZ","mZZ",mzzNBins,mzzLow,mzzHigh) ## IMPORTANT: Master fit range must be the same as for the datacards
    mzz.setBins(int(mzzNBins))
    print 'mzz created in range ',mzzLow,' ', mzzHigh,' with ',mzzNBins,' bins'
    weight = root.RooRealVar("weight","weight",0,100000)
    match  = root.RooCategory("match","match")
    match.defineType("unmatched",0)
    match.defineType("matched",1)
     
    getattr(workspace,'import')(mzz)
    getattr(workspace,'import')(weight)
    getattr(workspace,'import')(match)
    print 'TEMP NBINS: ', workspace.var("mZZ").getBins()

def readTree(filename, njet,pur,lep, workspace):
    # set up dataset, filtering for the proper jet category
    print 'Reading tree with arguments: ',njet,' ',pur,' ' ,lep

    varlist = root.RooArgSet(workspace.var("mZZ"),workspace.var("weight"),workspace.cat("match"))
    dataset = root.RooDataSet("dataset","dataset",varlist)

    mzz = workspace.var("mZZ")
    weight = workspace.var("weight")
    match = workspace.cat("match")


    #read the tree
    infile = root.TFile.Open(filename)
    tree = infile.Get("SelectedCandidates")
 ###   print 'Input tree contains: ',tree.GetEntries(),' entries'
    for event in tree:
    ###    print 'nCands=',event.nCands
        for i in range(event.nCands):
       ###     print 'nXj=',event.nXjets[i],'  Region=',event.region[i],'  MZZ=',event.mZZ[i]
            if event.nXjets[i]==njet and event.region[i]==1 \
            and event.mZZ[i]> mzz.getMin() and event.mZZ[i]< mzz.getMax(): # select events in signal region with corect jet number
                if (event.lep[i]!=lep and lep!=2): #if lep==2, no check on lep flavor, i.e. sum ele+mu
                    continue
                
                if (pur<0 or event.vTagPurity[i]==pur):
             ###       print 'vTag=',event.vTagPurity[i],'  ---> event selected'
                    mzz.setVal(event.mZZ[i])
                    weight.setVal(event.weight)
                    if njet==2 : # mc matching active only for 2-jets right now
                        if event.MCmatch[i]!=0:
                            match.setIndex(1)
                        else:
                            match.setIndex(0)
                    else:
                    ###print deduceBosonType(filename)
                        if  deduceBosonType(filename)=="Z":
                            if event.nsubj21[i] > 999.0: #nsubjettiness cut for ZZ
                                continue
                        else: #WW
                            dummy =  event.nsubj21[i]
                            #print "fitting WW sample"
                            #deltaR_LJ = deltaR(event.etalep1[i],event.philep1[i],event.etajet1[i],event.phijet1[i]);
                            #deltaPhi_JMET = deltaPhi(event.phijet1[i],event.philep2[i]);
                            #deltaPhi_JWL  = deltaPhi(event.phijet1[i],event.phiZll[i]);
                            
                            # these cuts are already applied in the lates trees
                            #if(event.lep[i]!=1.):
                            #    continue # 1 is for mu,0 for ele
                            #if((event.nLooseEle+event.nLooseMu)!=1):
                            #    continue # second lepton veto
                            #if(event.ptZll[i]<200):
                            #    continue #pt of Wmunu                                                
                            #if(event.nbtagsM[i]!=0):
                            #    continue #b-tag veto
                            #if(deltaR_LJ<1.57 or deltaPhi_JMET<2. or deltaPhi_JWL<2.):
                            #    continue
                           ## if(event.nsubj21[i]>0.45):
                           ##     continue
                        
                        match.setIndex(1) #assume all 1-jet events to be matched for now
                    
                    dataset.add(root.RooArgSet(mzz,match,weight))
              ###  else :
              ###      print 'vTag=',event.vTagPurity[i],'  ---> event rejected'
    
    weightedSet = root.RooDataSet("weightedSet","weightedSet",dataset,varlist,"","weight");
    weightedSet.Print("v")

    getattr(workspace,'import')(weightedSet)


# make pretty plots of the different categories
def plot( category , workspace, descriptor):

    NBinsMZZ=workspace.var("mZZ").getBins();
    mZZLow2=workspace.var("mZZ").getMin();
    mZZHi2=workspace.var("mZZ").getMax();
    plot = workspace.var("mZZ").frame(NBinsMZZ)
    print 'Bins: ',NBinsMZZ,' Range: ',mZZLow2,' - ',mZZHi2

    if category == 0: # no match
        ###,root.RooFit.Binning(root.RooBinning(NBinsMZZ,mZZLow2,mZZHi2))
        workspace.data("weightedSet").plotOn(plot,root.RooFit.Cut("match==match::unmatched"))
        workspace.pdf("FitFunc").plotOn(plot,root.RooFit.Components("ExtUnMatchedFunc"),root.RooFit.ProjWData(workspace.data("weightedSet")))

    if category == 1: # matched
        workspace.data("weightedSet").plotOn(plot,root.RooFit.Cut("match==match::matched"))
        workspace.pdf("FitFunc").plotOn(plot,root.RooFit.Components("ExtMatchedFunc"),root.RooFit.ProjWData(workspace.data("weightedSet")))
      
    if category == 2: # both
        #,root.RooFit.Binning(root.RooBinning(NBinsMZZ,mZZLow2,mZZHi2) )
        workspace.data("weightedSet").plotOn(plot)
        workspace.pdf("FitFunc").plotOn(plot,root.RooFit.ProjWData(workspace.data("weightedSet")),\
                                        root.RooFit.FillStyle(1001),\
                                        root.RooFit.DrawOption("F"),\
                                        root.RooFit.FillColor(root.kOrange))
        workspace.pdf("FitFunc").plotOn(plot,root.RooFit.Components("ExtUnMatchedFunc"),root.RooFit.ProjWData(workspace.data("weightedSet")),\
                                        root.RooFit.FillStyle(1001),\
                                        root.RooFit.DrawOption("F"),\
                                        root.RooFit.FillColor(root.kViolet))        
        workspace.pdf("FitFunc").plotOn(plot,root.RooFit.ProjWData(workspace.data("weightedSet")),\
                                        root.RooFit.LineColor(root.kOrange+2))
        workspace.pdf("FitFunc").plotOn(plot,root.RooFit.Components("ExtUnMatchedFunc"),root.RooFit.ProjWData(workspace.data("weightedSet")),\
                                        root.RooFit.LineColor(root.kViolet+2))        
        workspace.data("weightedSet").plotOn(plot)

    c = root.TCanvas("c","c",600,600)

    plot.SetMinimum(0)
    maximum = root.TMath.MaxElement(plot.getObject(0).GetN(),plot.getObject(0).GetY())
    plot.SetMaximum(1.2*maximum)

    ##for WW
    #plot.GetXaxis().SetTitle("mWW [GeV]");
    #plot.SetTitle("");

    plot.Draw()

    filename = "plots/"+descriptor
    if category == 0:
        filename+= "_unmatched"
    if category == 1:
        filename+= "_matched"
    
    c.SaveAs(filename+".eps")
    c.SaveAs(filename+".png")

    plot.SetMinimum(maximum / max(2,workspace.data("weightedSet").numEntries()))
    plot.Draw()

    c.SetLogy(True)
    
    c.SaveAs(filename+"_log.eps")
    c.SaveAs(filename+"_log.png")
    

def processSubsample(inputpath,njets,pur, lep,plotonly):

    print ''
    print ''
    print ''
    print ''
    print '==================================================================================='
    print 'Fitting shape for events in ',inputpath,' (',str(njets),'J ',str(pur),' ',str(lep),')'
    
    bosontype = deduceBosonType(inputpath) # we don't use this yet, but meybe we will?
    descriptor = desc(inputpath) # core string of the input file. This determines the file to read the initial values as well as names of output plots

    # set up variables, functions, datasets
    workspace = root.RooWorkspace("ws","ws")
    defineVars(descriptor,njets,workspace,plotonly)           
    readTree(inputpath,njets,pur,lep,workspace)
    ConstructPdf(workspace)

    # fit goes here
    data = workspace.data("weightedSet")
    print 'Goint to fit dataset containing ',    data.sumEntries()
    if not plotonly:
        result = workspace.pdf("FitFunc").fitTo( data , root.RooFit.Save() )
        result.Print("v")
        pur_str=""
        if pur==0 :
            pur_str="LP"
        elif pur==1 :
            pur_str="HP"
        else:
            pur_str=""

        lep_str=""
        if lep==0 :
            lep_str="ELE"
        elif lep==1 :
            lep_str="MU"
        else:
            lep_str="ALL"

        suffix= str( njets )+"J_" +pur_str+"_"+lep_str
            
        workspace.set("pars").writeToFile("pars/outpars_"+descriptor +"_" +suffix+  ".config")
                
    plot(0,workspace,descriptor+"_"+suffix)
    plot(1,workspace,descriptor+"_"+suffix)
    plot(2,workspace,descriptor+"_"+suffix)
    #workspace.Print("v")

   
def main():
    parser = argparse.ArgumentParser(description='Signal Shape Fitting Tool')
    parser.add_argument('-j','--njets',     help='number of jets: 1 or 2 or 3(both), default:both'  ,type=int, choices=[1,2,3]    , default = 3)
    parser.add_argument('-p','--purity',     help='purity category: 0 (low purity) or 1 (high purity) or -1 (no purity selection), default:-1'  ,type=int, choices=[-1,0,1]    , default = -1)
    parser.add_argument('-l','--lep',     help='lepton flavor: 0 (ele) or 1 (muons) or 2 (both, no lepton flavor selection), default:both'                   ,type=int, choices=[0,1,2]    , default = 2)
    parser.add_argument('-f','--filepath',  help='path to input trees, default: ./trees'                             , default = "../trees" )                                      
    parser.add_argument('--plotonly',  help='don\'t refit, just redraw plots from available parameter files, default = False', default = False, type=bool )                                 
    parser.add_argument('-m','--mass', help='process only signal with this mass value in the file name', type=int, default=-1)

    args = parser.parse_args()
    
    root.gROOT.SetBatch(True)
    root.gSystem.Load('libHiggsAnalysisCombinedLimit')

    print 'PRINTING ARGUMENTS:'
    print args

    filelist = []
    # do we run on a single file or a whole diretory?
    if os.path.isfile(args.filepath): # single file
        filelist.append(args.filepath)
    else: #lets run on all suitable files in this directory
        for file in os.listdir(args.filepath):
            filelist.append(args.filepath + "/" +  file)

   
    for file in filelist:
        print 'File is ',file
        if(checkfile(desc(file))):
            if (desc(file).find('M'+str(args.mass))==-1 and args.mass>0) :
                continue
            for nj in 1,2:
                if (nj!=args.njets and args.njets!=3):
                    continue  
                for np in -1,0,1:
                    if (np!=args.purity):
                        continue
                    if (nj==2):                        
                        np=-1
                    for lf in 0,1:
                        if(lf!=args.lep and args.lep!=2):
                            continue
                        if (args.lep==2):
                            if (lf>0):
                                continue
                            else:
                                lf=2
                        processSubsample(file,nj,np,lf,args.plotonly)
            




if __name__ == "__main__":
    main()
