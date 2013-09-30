#!/usr/bin/env python


import ROOT as root
import optparse
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

    workspace.var("mZZ").setBins(10000,"cache") 

    Core = root.RooBreitWigner("Core","Core",workspace.var("mZZ"),workspace.var("mean_match"),workspace.var("width_match"))
    res_mean = root.RooRealVar("res_mean","resmean",0)
    Resol     = root.RooDoubleCB("Resol","Resol",workspace.var("mZZ"),res_mean,workspace.var("sigma_match"),workspace.var("alpha1_match"),workspace.var("n1_match"),workspace.var("alpha2_match"),workspace.var("n2_match"))

    PlotFunc = root.RooFFTConvPdf("PlotFunc","PlotFunc",workspace.var("mZZ"),Core, Resol)
    PlotFunc.setBufferFraction(1.0)
    
    getattr(workspace,'import')(PlotFunc)

# set up the variables to be used in the fit. Will need ot be extended if we use different funcional forms for different channels
def defineVars(descriptor,workspace,inputpath,lep):
    # matched parameters
    mean_match = root.RooRealVar("mean_match","mean_match",0)
    sigma_match = root.RooRealVar("sigma_match","sigma_match",0)
    width_match = root.RooRealVar("width_match","width_match",0)
    alpha1_match = root.RooRealVar("alpha1_match","alpha1_match",0)
    n1_match = root.RooRealVar("n1_match","n1_match",0)
    alpha2_match = root.RooRealVar("alpha2_match","alpha2_match",0)
    n2_match = root.RooRealVar("n2_match","n2_match",0)
    

    fitpars   = root.RooArgSet(mean_match,sigma_match,width_match)
    fitpars.add(alpha1_match)
    fitpars.add(n1_match)
    fitpars.add(alpha2_match)
    fitpars.add(n2_match)

    workspace.defineSet("pars",fitpars,True)


    #read nominal input parameters to set mass range
    filename ="pars/inpars_"
    filename += descriptor +".config"
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
     
    getattr(workspace,'import')(mzz)
    getattr(workspace,'import')(weight)
    print 'TEMP NBINS: ', workspace.var("mZZ").getBins()

    #now read the CB shape from the narrow width fits
    mass = 0
    flavor = 0
    lep_str=""
    if lep==0 :
        lep_str="ELE"
    elif lep==1 :
        lep_str="MU"
    else:
        lep_str="ALL"

    substrings = inputpath.split("_")
    for substr in substrings:
        if len(substr)>1 and substr[0]=="M" and substr[1]!="U":
            mass = substr[1:]
            
    fnam = "pars/outpars_BulkG_ZZ_lljj_c0p2_M"
    fnam = fnam + mass + "_1J__"+ lep_str + ".config"
    print "override:take nominal parameters from:" + fnam
    workspace.defineSet("old","mean_match,sigma_match,alpha1_match,n1_match,alpha2_match,n2_match")
    workspace.set("old").readFromFile(fnam)
    print str(workspace.var("mean_match").getVal())



def readTree(filename,pur,lep, workspace):
    # set up dataset, filtering for the proper jet category
    print 'Reading tree with arguments: ',pur,' ' ,lep

    varlist = root.RooArgSet(workspace.var("mZZ"),workspace.var("weight"))
    dataset = root.RooDataSet("dataset","dataset",varlist)

    mzz = workspace.var("mZZ")
    weight = workspace.var("weight")
   

    #read the tree
    infile = root.TFile.Open(filename)
    tree = infile.Get("SelectedCandidates")
 ###   print 'Input tree contains: ',tree.GetEntries(),' entries'
    for event in tree:
    ###    print 'nCands=',event.nCands
        for i in range(event.nCands):
       ###     print 'nXj=',event.nXjets[i],'  Region=',event.region[i],'  MZZ=',event.mZZ[i]
            if event.nXjets[i]==1 and event.region[i]==1 \
            and event.mZZ[i]> mzz.getMin() and event.mZZ[i]< mzz.getMax(): # select events in signal region with corect jet number
                if (event.lep[i]!=lep and lep!=2): #if lep==2, no check on lep flavor, i.e. sum ele+mu
                    continue
                
                if (pur<0 or event.vTagPurity[i]==pur):
             ###       print 'vTag=',event.vTagPurity[i],'  ---> event selected'
                    mzz.setVal(event.mZZ[i])
                    weight.setVal(event.weight)
                    ###print deduceBosonType(filename)
                    if  deduceBosonType(filename)=="Z":
                        if event.nsubj21[i] > 999.0: #nsubjettiness cut for ZZ
                            continue
                    else: #WW
                        dummy =  event.nsubj21[i]
                        
                    
                    dataset.add(root.RooArgSet(mzz,weight))
    
    weightedSet = root.RooDataSet("weightedSet","weightedSet",dataset,varlist,"","weight");
    weightedSet.Print("v")


    getattr(workspace,'import')(weightedSet)


# make pretty plots of the different categories
def plot(  workspace, descriptor):

    NBinsMZZ=workspace.var("mZZ").getBins();
    mZZLow2=workspace.var("mZZ").getMin();
    mZZHi2=workspace.var("mZZ").getMax();
    plot = workspace.var("mZZ").frame(NBinsMZZ)
    print 'Bins: ',NBinsMZZ,' Range: ',mZZLow2,' - ',mZZHi2

      

    #,root.RooFit.Binning(root.RooBinning(NBinsMZZ,mZZLow2,mZZHi2) )
    workspace.data("weightedSet").plotOn(plot)
    workspace.pdf("PlotFunc").plotOn(plot,\
                                    root.RooFit.FillStyle(1001),\
                                    root.RooFit.DrawOption("F"),\
                                    root.RooFit.FillColor(root.kOrange))
    workspace.pdf("PlotFunc").plotOn(plot,\
                                    root.RooFit.LineColor(root.kOrange+2))
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
    
    c.SaveAs(filename+".eps")
    c.SaveAs(filename+".png")

    plot.SetMinimum(maximum / max(2,workspace.data("weightedSet").numEntries()))
    plot.Draw()

    c.SetLogy(True)
    
    c.SaveAs(filename+"_log.eps")
    c.SaveAs(filename+"_log.png")
    

def processSubsample(inputpath,pur, lep):

    print ''
    print ''
    print ''
    print ''
    print '==================================================================================='
    print 'Fitting shape for events in ',inputpath,' (',str(pur),' ',str(lep),')'
    
    bosontype = deduceBosonType(inputpath) # we don't use this yet, but meybe we will?
    descriptor = desc(inputpath) # core string of the input file. This determines the file to read the initial values as well as names of output plots

    # set up variables, functions, datasets
    workspace = root.RooWorkspace("ws","ws")
    defineVars(descriptor,workspace,inputpath,lep)           
    readTree(inputpath,pur,lep,workspace)
    ConstructPdf(workspace)



    # fit goes here
    data = workspace.data("weightedSet")
    print 'Goint to fit dataset containing ',    data.sumEntries()

    workspace.Print("v")
    
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
       
    suffix= pur_str+"_"+lep_str
        
    plot(workspace,descriptor+"_"+suffix)

    workspace.Print("v")

   
def main():
    parser = optparse.OptionParser(description='Signal Shape Fitting Tool')
    parser.add_option('-p','--purity',     help='purity category: 0 (low purity) or 1 (high purity) or -1 (no purity selection), default:-1',type=int    , default = -1)
    parser.add_option('-l','--lep',     help='lepton flavor: 0 (ele) or 1 (muons) or 2 (both, no lepton flavor selection), default:both',type=int    , default = 2)
    parser.add_option('-f','--filepath',  help='path to input trees, default: ./trees'                             , default = "../../trees/fullsig" )                                      
    parser.add_option('-m','--mass', help='process only signal with this mass value in the file name', type=int, default=-1)

    
    ( args, XXX) = parser.parse_args()
 
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
            for np in -1,0,1:                
                if (np!=args.purity):
                    print "skip prurity", np
                    continue
                for lf in 0,1:
                    if(lf!=args.lep and args.lep!=2):
                        print "skipping lepton flavor:", lf
                        continue
                    if (args.lep==2):
                        if (lf>0):
                            continue
                        else:
                            lf=2

                    processSubsample(file,np,lf)
            




if __name__ == "__main__":
    main()
