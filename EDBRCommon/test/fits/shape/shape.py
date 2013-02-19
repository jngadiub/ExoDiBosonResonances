#!/usr/bin/env python


import ROOT as root


def ConstructPdf(workspace):
    
    MatchedFuncBase   = root.RooVoigtian("MatchedFunc","MatchedFunc",workspace.var("mZZ"),workspace.var("mean_match"),workspace.var("sigma_match"),workspace.var("width_match"))
    totalnorm = root.RooRealVar("totalnorm","totalnorm",1,0,100000)
    matchnorm = root.RooProduct("matchnorm","matchnorm",root.RooArgSet(totalnorm,workspace.var("machfrac")))
    MatchedFunc = root.RooExtendPdf("ExtMatchedFunc","ExtMatchedFunc",MatchedFuncBase,matchnorm)
    
    UnMatchedFuncBase = root.RooCBShape("UnMatchedFunc","UnMatchedFunc",workspace.var("mZZ"),workspace.var("mean_unmatch"),workspace.var("sigma_unmatch"),workspace.var("alpha_unmatch"),workspace.var("n_unmatch") )
    unmatchnorm = root.RooFormulaVar("unmatchnorm","unmatchnorm","@0*(1-@1)",root.RooArgList(totalnorm,workspace.var("machfrac")))
    UnMatchedFunc = root.RooExtendPdf("ExtUnMatchedFunc","ExtUnMatchedFunc",UnMatchedFuncBase,unmatchnorm)

    FitFunc = root.RooSimultaneous("FitFunc","FitFunc", root.RooArgList(UnMatchedFunc,MatchedFunc), workspace.cat("match"))

    getattr(workspace,'import')(FitFunc)


def defineVars(mass,njets,workspace):
    mzz    = root.RooRealVar("mZZ","mZZ",400,2000) ## IMPORTANT: Master fit range must be the same as for the datacards
    weight = root.RooRealVar("weight","weight",0,100000)
    match  = root.RooCategory("match","match")
    match.defineType("unmatched",0)
    match.defineType("matched",1)
    # matched parameters
    mean_match = root.RooRealVar("mean_match","mean_match",0)
    sigma_match = root.RooRealVar("sigma_match","sigma_match",0)
    width_match = root.RooRealVar("width_match","width_match",0)
    
    # unmatched parameters
    mean_unmatch = root.RooRealVar("mean_unmatch","mean_unmatch",0)
    sigma_unmatch = root.RooRealVar("sigma_unmatch","sigma_unmatch",0)
    alpha_unmatch = root.RooRealVar("alpha_unmatch","alpha_unmatch",0)
    n_unmatch = root.RooRealVar("n_unmatch","n_unmatch",0)

    # realtive fractions
    machfrac = root.RooRealVar("machfrac","machfrac",0)

    fitpars   = root.RooArgSet(mean_match,sigma_match,width_match,mean_unmatch,sigma_unmatch,alpha_unmatch,n_unmatch,machfrac)

     
    getattr(workspace,'import')(mzz)
    getattr(workspace,'import')(weight)
    getattr(workspace,'import')(match)

    workspace.defineSet("pars",fitpars,True)
    
   
    filename = "pars/inpars_" + str( njets ) + "_" + str( mass ) + ".config" 
    workspace.set("pars").readFromFile(filename)
    #workspace.set("pars").writeToFile(filename)

         

def readTree(filename, njet, workspace):
    # set up dataset

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
            and event.mZZ[i]> mzz.getMin() and event.mZZ[i]< mzz.getMax() : # select events in signal region with corect jet number
                mzz.setVal(event.mZZ[i])
                weight.setVal(event.weight)
                if event.MCmatch[i]!=0:
                    match.setIndex(1)
                else:
                    match.setIndex(0)
                dataset.add(root.RooArgSet(mzz,match,weight))
    
    
    weightedSet = root.RooDataSet("weightedSet","weightedSet",dataset,varlist,"","weight");
    weightedSet.Print("v")

    getattr(workspace,'import')(weightedSet)



def plot( category , workspace):

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
    
    c.SaveAs("test.eps")

    plot.SetMinimum(maximum / workspace.data("weightedSet").numEntries())
    plot.Draw()

    c.SetLogy(True)

    c.SaveAs("test_log.eps")
    

   
def main():

    root.gROOT.SetBatch(True)

    mass = 300
    njets= 2

    # set up variables, functions, datasets
    workspace = root.RooWorkspace("ws","ws")
    defineVars(mass,njets,workspace)           
    readTree("../trees/treeEDBR_BulkG_ZZ_lljj_c0p2_M1000.root",2,workspace)
    ConstructPdf(workspace)

    # fit goes here
    data = workspace.data("weightedSet")
    result = workspace.pdf("FitFunc").fitTo( data , root.RooFit.Save() )
    result.Print("v")
    plot(2,workspace)
    #workspace.Print("v")


if __name__ == "__main__":
    main()
