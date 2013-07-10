#!/usr/bin/env python


import ROOT as root
import optparse
import os,sys
import math

def lep_str(leptnum):
    if leptnum==0:
        return "ELE"
    if leptnum==1:
        return "MU"

    return "NOTFOUND"

def lep_str2(leptnum):
    if leptnum==0:
        return "ee"
    if leptnum==1:
        return "mm"

    return "NOTFOUND"
    
def pur_str(purnum):
    if purnum==0:
        return "LP"
    if purnum==1:
        return "HP"

    return ""

def fun_str(function):
    if function==0:
        return "exp"
    if function==1:
        return "levexp"
    if function==2:
        return "pl2"

    return ""


def load_signal(ws,njets,purity,lep,mass):
    filename  = "DataCards_XZZ_20130701/"+ str(mass)+"/xzz_"+lep_str2(lep)+str(njets)+"J"+pur_str(purity)+".input.root"
    print "loading signal shape from "+filename
    file = root.TFile.Open(filename)
    #file.ls()
    impws =file.Get("w")
    signame = "signal_xzz_"+lep_str(lep)+str(njets)+"J"+pur_str(purity)
    #impws.Print()
    getattr(ws,"import")(impws.pdf(signame))
    return signame

def create_bkg(ws,type,name):
    if type == 0: #exp
        a0 = root.RooRealVar(name+"p0",name+"p0",-0.25,-10.0,0.0);
        expo_fit=root.RooExponential(name,name,ws.var("mZZ"),a0);
        getattr(ws,"import")(expo_fit)
    if type == 1: # leveled expo
        f0b=root.RooRealVar(name+"f0b","sigma",200,0.0,300.0);
        f1b=root.RooRealVar(name+"f1b","alpha",0,-0.05,2.0);
        f2b=root.RooRealVar(name+"f2b","beta",0.0,-0.05,2.0);
        mb=root.RooRealVar(name+"mb","m",480,200.0,500.0);
        tb=root.RooRealVar(name+"tb","theta",0.0);
        f2b.setConstant(root.kTRUE);
        mb.setConstant(root.kTRUE);
        tb.setConstant(root.kTRUE);
        fit_2par=root.RooLevelledExp2(name,name,ws.var("mZZ"),f0b,f1b,f2b,mb,tb);
        getattr(ws,"import")(fit_2par)
        
    if type == 2: # modified powerlaw
        f0b=root.RooRealVar(name+"f0b","plaw index",-0.1,-10.0,0.0);
        f1b=root.RooRealVar(name+"f1b","plaw modifier",0,-1.,1.0);
        #f1b.setConstant(root.kTRUE)
        fit_2par=root.RooGenericPdf(name,"TMath::Power(@0,@1 + @0*@2)",root.RooArgList(ws.var("mZZ"),f0b,f1b));
        getattr(ws,"import")(fit_2par)
        



def main():
    parser = optparse.OptionParser()
    parser.add_option('-j','--njets',     help='number of jets: 1 or 2 ,default:single'  ,type=int,  default = 1)
    parser.add_option('-p','--purity',     help='purity category: 0 (low purity) or 1 (high purity) or -1 (no purity selection), default:1'  ,type=int, default = 1)
    parser.add_option('-l','--lep',     help='lepton flavor: 0 (ele) or 1 (muons) or 2 (both, no lepton flavor selection), default:both'                   ,type=int, default = 2)
    parser.add_option('-f','--filepath',  help='directory with workspace'                             , default = "./" )                                      
    parser.add_option('-m','--mass', help='test signal yield for this mass', type=int, default=-1)
    parser.add_option('-n','--nexp', help='number of toys', type=int, default=100)
    parser.add_option('-g','--fgen', help='function to generate toys (0=exp,1=levexp,2=powerlaw)', type=int, default=1)
    parser.add_option('-r','--fres', help='function to fit toys (0=exp,1=levexp,2=powerlaw)', type=int, default=1)

    ( args, XXX) = parser.parse_args()
    
    root.gROOT.SetBatch(True)
    root.gSystem.Load('libHiggsAnalysisCombinedLimit')

    # worspace to handle functions for pseudoexperiments
    ws = root.RooWorkspace("pseudows")

    # load the input workspace with alpha files
    fname = args.filepath + "workspace_xzz_"+ str(args.njets) +"J_"+ pur_str(args.purity)+"_ALL_new.root"
    wsname  = "ws_alpha_xzz_" + str(args.njets) +"J_"+ pur_str(args.purity)+"_ALL"
    print "Reading sideband data from "+fname
    file = root.TFile.Open(fname,"READ")
    #file.ls()
    #print wsname
    ref  = file.Get(wsname)
    #ref.Print()


    #shapes with parameters to be fixed signal shape
    signame = load_signal(ws,args.njets,args.purity,args.lep,args.mass)
    create_bkg(ws,args.fgen,"genfunc")
    
    #ws.Print()

    #set up generator function
    fitres = ws.pdf("genfunc").fitTo(ref.data("dsDataSB2"),root.RooFit.Save(root.kTRUE),root.RooFit.SumW2Error(root.kTRUE),root.RooFit.PrintLevel(-1))
    fitres.Print()
    plot=ws.var("mZZ").frame()
    ref.data("dsDataSB2").plotOn(plot)
    ws.pdf("genfunc").plotOn(plot)
    canv = root.TCanvas()
    plot.Draw()
    plotname = "biasplots/sbfit_"+fun_str(args.fgen)+"_"+pur_str(args.purity)+".eps"
    canv.SaveAs(plotname)

    # set all signal and gen paramters constat
    argset = ws.allVars()
    iter = argset.createIterator()
    ws.allVars().Print()
    var = iter.Next()
    while var :
        var.setConstant(root.kTRUE)
        print "fixing "+var.GetName()+ " to " + str(var.getVal())
        var = iter.Next()
    ws.var("mZZ").setConstant(root.kFALSE)

    #roomcstudy
    create_bkg(ws,args.fres,"resfunc")
    ws.factory("RooAddPdf::fitf(resfunc,"+signame+",sigfrac[0.99,0,1])")
    mcs = root.RooMCStudy(ws.pdf("genfunc"),root.RooArgSet(ws.var("mZZ")),root.RooFit.FitModel(ws.pdf("fitf")),root.RooFit.FitOptions(root.RooFit.Save(root.kTRUE)),root.RooFit.Silence())

    numevents = ref.data("dsDataSIG").sumEntries()
    mcs.generateAndFit(args.nexp,int(numevents))

    #ws.pdf("fitf").Print("v")
    #ws.var("sigfrac").Print("v")
    
    #pars = mcs.fitResult(0).floatParsFinal()
    #pars.Print()

    frame1 = mcs.plotParam(ws.var("sigfrac"),root.RooFit.Bins(40),root.RooFit.Range(0.99,1.0)) ;
    #frame1.Draw()
    #plotname = "biasplots/biasresult_"+fun_str(args.fgen)+"_"+fun_str(args.fres)+"_"+str(args.mass)+lep_str(args.lep)+pur_str(args.purity)+".eps"
    #canv.SaveAs(plotname)


    pullhist = root.TH1F("pullhist","pullhist",40,0,3)

    for i in xrange(args.nexp):
        pullhist.Fill( (1.-mcs.fitResult(i).floatParsFinal().find("sigfrac").getVal())/mcs.fitResult(i).floatParsFinal().find("sigfrac").getError() )
        
    root.gStyle.SetOptFit()
    pullhist.Fit("gaus","","",0.1,3)
    pullhist.Draw()    
    plotname = "biasplots/pull_"+fun_str(args.fgen)+"_"+fun_str(args.fres)+"_"+str(args.mass)+lep_str(args.lep)+pur_str(args.purity)+".eps"
    canv.SaveAs(plotname)
    

#    for i in xrange(args.nexp):
#        print str(mcs.fitResult(i).floatParsFinal().find("sigfrac").getVal())
    

    
if __name__ == "__main__":
    main()
