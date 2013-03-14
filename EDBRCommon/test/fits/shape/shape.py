#!/usr/bin/env python


import ROOT as root
import argparse
import os,sys

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
    totalnorm = root.RooRealVar("totalnorm","totalnorm",100,0,100000)
    matchnorm = root.RooProduct("matchnorm","matchnorm",root.RooArgSet(totalnorm,workspace.var("machfrac")))
    MatchedFunc = root.RooExtendPdf("ExtMatchedFunc","ExtMatchedFunc",MatchedFuncBase,matchnorm)
    
    UnMatchedFuncBase = root.RooCBShape("UnMatchedFunc","UnMatchedFunc",workspace.var("mZZ"),workspace.var("mean_unmatch"),workspace.var("sigma_unmatch"),workspace.var("alpha_unmatch"),workspace.var("n_unmatch") )
    unmatchnorm = root.RooFormulaVar("unmatchnorm","unmatchnorm","@0*(1-@1)",root.RooArgList(totalnorm,workspace.var("machfrac")))
    UnMatchedFunc = root.RooExtendPdf("ExtUnMatchedFunc","ExtUnMatchedFunc",UnMatchedFuncBase,unmatchnorm)

    FitFunc = root.RooSimultaneous("FitFunc","FitFunc", root.RooArgList(UnMatchedFunc,MatchedFunc), workspace.cat("match"))

    getattr(workspace,'import')(FitFunc)

# set up the variables to be used in the fit. Will need ot be extended if we use different funcional forms for different channels
def defineVars(descriptor,njets,workspace,plotonly):
    mzz    = root.RooRealVar("mZZ","mZZ",400,2500) ## IMPORTANT: Master fit range must be the same as for the datacards
    weight = root.RooRealVar("weight","weight",0,100000)
    match  = root.RooCategory("match","match")
    match.defineType("unmatched",0)
    match.defineType("matched",1)
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

     
    getattr(workspace,'import')(mzz)
    getattr(workspace,'import')(weight)
    getattr(workspace,'import')(match)

    workspace.defineSet("pars",fitpars,True)
    
    filename = "pars/"
    if plotonly:
        filename +="outpars_"
    else:
        filename +="inpars_"
    filename += descriptor  +"_" +  str( njets ) +  ".config" 
    workspace.set("pars").readFromFile(filename)



def readTree(filename, njet, workspace):
    # set up dataset, filtering for the proper jet category

    varlist = root.RooArgSet(workspace.var("mZZ"),workspace.var("weight"),workspace.cat("match"))
    dataset = root.RooDataSet("dataset","dataset",varlist)

    mzz = workspace.var("mZZ")
    weight = workspace.var("weight")
    match = workspace.cat("match")


    #read the tree
    infile = root.TFile.Open(filename)
    tree = infile.Get("SelectedCandidates")
    for event in tree:
        for i in range(event.nCands):
            if event.nXjets[i]==njet and event.region[i]==1 \
            and event.mZZ[i]> mzz.getMin() and event.mZZ[i]< mzz.getMax(): # select events in signal region with corect jet number, with njettiness cut
                mzz.setVal(event.mZZ[i])
                weight.setVal(event.weight)
                if njet==2 : # mc matching active only for 2-jets right now
                    if event.MCmatch[i]!=0:
                        match.setIndex(1)
                    else:
                        match.setIndex(0)
                else:                     
                    if 1./(event.nsubj12[i]) > 0.45: #nsubjettiness cut
                        break                    
                    match.setIndex(1) #assume all 1-jet events to be matched for now
                    
                    
                dataset.add(root.RooArgSet(mzz,match,weight))
    
    
    weightedSet = root.RooDataSet("weightedSet","weightedSet",dataset,varlist,"","weight");
    weightedSet.Print("v")

    getattr(workspace,'import')(weightedSet)


# male pretty plots of the different categories
def plot( category , workspace, descriptor):

    plot = workspace.var("mZZ").frame()


    if category == 0: # no match
        workspace.data("weightedSet").plotOn(plot,root.RooFit.Cut("match==match::unmatched"))
        workspace.pdf("FitFunc").plotOn(plot,root.RooFit.Components("ExtUnMatchedFunc"),root.RooFit.ProjWData(workspace.data("weightedSet")))

    if category == 1: # matched
        workspace.data("weightedSet").plotOn(plot,root.RooFit.Cut("match==match::matched"))
        workspace.pdf("FitFunc").plotOn(plot,root.RooFit.Components("ExtMatchedFunc"),root.RooFit.ProjWData(workspace.data("weightedSet")))
      
    if category == 2: # both
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

    plot.Draw()

    filename = "plots/"+descriptor
    if category == 0:
        filename+= "_unmatched"
    if category == 1:
        filename+= "_matched"
    
    c.SaveAs(filename+".eps")

    plot.SetMinimum(maximum / max(2,workspace.data("weightedSet").numEntries()))
    plot.Draw()

    c.SetLogy(True)
    
    c.SaveAs(filename+"_log.eps")
    

def processSubsample(inputpath,njets,plotonly):
    
    bosontype = deduceBosonType(inputpath) # we don't use this yet, but meybe we will?
    descriptor = desc(inputpath) # core string of the input file. This determines the file to read the initial values as well as names of output plots

    # set up variables, functions, datasets
    workspace = root.RooWorkspace("ws","ws")
    defineVars(descriptor,njets,workspace,plotonly)           
    readTree(inputpath,njets,workspace)
    ConstructPdf(workspace)

    # fit goes here
    data = workspace.data("weightedSet")
    if not plotonly:
        result = workspace.pdf("FitFunc").fitTo( data , root.RooFit.Save() )
        result.Print("v")
        workspace.set("pars").writeToFile("pars/outpars_"+descriptor +"_" + str( njets ) +  ".config")
                
    plot(0,workspace,descriptor+"_"+str(njets))
    plot(1,workspace,descriptor+"_"+str(njets))
    plot(2,workspace,descriptor+"_"+str(njets))
    #workspace.Print("v")

   
def main():
    parser = argparse.ArgumentParser(description='Signal Shape Fitting Tool')
    parser.add_argument('-n','--njets',     help='number of jets: 1 or 2 or 3(both), default:both'                   ,type=int, choices=[1,2,3]    , default = 3)
    parser.add_argument('-f','--filepath',  help='path to input trees, default: ./trees'                             , default = "../trees" )                                      
    parser.add_argument('-p','--plotonly',  help='don\'t refit, just redraw plots from available parameter files, default = False', default = False, type=bool )                                 
    args = parser.parse_args()
    
    root.gROOT.SetBatch(True)
    root.gSystem.Load('libHiggsAnalysisCombinedLimit')

    print args

    filelist = []
    # do we run on a single file or a whole diretory?
    if os.path.isfile(args.filepath): # single file
        filelist.append(args.filepath)
    else: #lets run on all suitable files in this directory
        for file in os.listdir(args.filepath):
            filelist.append(args.filepath + "/" +  file)


    for file in filelist:
        if(checkfile(desc(file))):
            if args.njets==3:
                processSubsample(file,1,args.plotonly)
                processSubsample(file,2,args.plotonly)           
            else:
                processSubsample(file,args.njets,args.plotonly)
            




if __name__ == "__main__":
    main()
