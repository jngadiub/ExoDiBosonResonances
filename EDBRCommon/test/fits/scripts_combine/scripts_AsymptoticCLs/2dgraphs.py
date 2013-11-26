import ROOT as root
from array import *


def createColors(i):
##     r = [0., 0.0, 1.0, 1.0, 1.0]
##     g = [0., 0.0, 0.0, 1.0, 1.0]
##     b = [0., 1.0, 0.0, 0.0, 1.0]
##     stop = [0., .25, .50, .75, 1.0]
    r = [1.0, 0.0, 0.0]
    g = [0.0, 1.0, 0.0]
    b = [1.0, 0.0, 1.0]
    stop = [0.,.50, 1.0]
    ra= array('d',r)
    ga= array('d',g)
    ba= array('d',b)
    stopa= array('d',stop)
    FI = root.TColor.CreateGradientColorTable(3, stopa, ra, ga, ba, i);
    
def setColors2d():
    r = [0., 0.0, 1.0, 1.0, 1.0]
    g = [0., 0.0, 0.0, 1.0, 1.0]
    b = [0., 1.0, 0.0, 0.0, 1.0]
    stop = [0., .25, .50, .75, 1.0]
    ra= array('d',r)
    ga= array('d',g)
    ba= array('d',b)
    stopa= array('d',stop)
    FI = root.TColor.CreateGradientColorTable(5, stopa, ra, ga, ba, 100);
    

def setFPStyle():
    

  root.gStyle.SetPadBorderMode(0)
  root.gStyle.SetFrameBorderMode(0)
  root.gStyle.SetPadBottomMargin(0.12)
  root.gStyle.SetPadLeftMargin(0.12)
  root.gStyle.SetCanvasColor(root.kWhite)
  root.gStyle.SetCanvasDefH(600) #Height of canvas
  root.gStyle.SetCanvasDefW(600) #Width of canvas
  root.gStyle.SetCanvasDefX(0)   #POsition on screen
  root.gStyle.SetCanvasDefY(0)

  root.gStyle.SetPadTopMargin(0.05)
  root.gStyle.SetPadBottomMargin(0.15)#0.13)
  root.gStyle.SetPadLeftMargin(0.15)#0.16)
  root.gStyle.SetPadRightMargin(0.05)#0.02)



 # For the Pad:
  root.gStyle.SetPadBorderMode(0)
  # root.gStyle.SetPadBorderSize(Width_t size = 1)
  root.gStyle.SetPadColor(root.kWhite)
  root.gStyle.SetPadGridX(root.kFALSE)
  root.gStyle.SetPadGridY(root.kFALSE)
  root.gStyle.SetGridColor(0)
  root.gStyle.SetGridStyle(3)
  root.gStyle.SetGridWidth(1)

  # For the frame:
  root.gStyle.SetFrameBorderMode(0)
  root.gStyle.SetFrameBorderSize(1)
  root.gStyle.SetFrameFillColor(0)
  root.gStyle.SetFrameFillStyle(0)
  root.gStyle.SetFrameLineColor(1)
  root.gStyle.SetFrameLineStyle(1)
  root.gStyle.SetFrameLineWidth(1)

  root.gStyle.SetAxisColor(1, "XYZ")
  root.gStyle.SetStripDecimals(root.kTRUE)
  root.gStyle.SetTickLength(0.03, "XYZ")
  root.gStyle.SetNdivisions(510, "XYZ")
  root.gStyle.SetPadTickX(1)  # To get tick marks on the opposite side of the frame
  root.gStyle.SetPadTickY(1)
  root.gStyle.SetGridColor(0)
  root.gStyle.SetGridStyle(3)
  root.gStyle.SetGridWidth(1)


  root.gStyle.SetTitleColor(1, "XYZ")
  root.gStyle.SetTitleFont(42, "XYZ")
  root.gStyle.SetTitleSize(0.05, "XYZ")
  # root.gStyle.SetTitleXSize(Float_t size = 0.02) # Another way to set the size?
  # root.gStyle.SetTitleYSize(Float_t size = 0.02)
  root.gStyle.SetTitleXOffset(1.15)#0.9)
  root.gStyle.SetTitleYOffset(1.3) # => 1.15 if exponents
  root.gStyle.SetLabelColor(1, "XYZ")
  root.gStyle.SetLabelFont(42, "XYZ")
  root.gStyle.SetLabelOffset(0.007, "XYZ")
  root.gStyle.SetLabelSize(0.045, "XYZ")

  root.gStyle.SetPadBorderMode(0)
  root.gStyle.SetFrameBorderMode(0)
  root.gStyle.SetTitleTextColor(1)
  root.gStyle.SetTitleFillColor(10)
  root.gStyle.SetTitleFontSize(0.05)


  for i in xrange(10):
      lsstr = "[12 12"
      lsstr +=" "+str(4*(i+1)) +" 12"
      lsstr +="]"
      print lsstr
      root.gStyle.SetLineStyleString(11+i,lsstr)






def main():
    root.gROOT.SetBatch(True)

    widths=[]

    f = open('widths.txt')
    for line in iter(f):
        widths.append(line.strip())


    createColors(len(widths))
    
    sw = sorted(widths , key=float)

    f.close()

    expected_graphs = {}
    for width in sw:
        fn = "AsymptoticCLs_TGraph_"+width+".root"
        file = root.TFile.Open(fn)
        expected_graphs[width] = file.Get("LimitExpectedCLs")
        


    setFPStyle()

    cMCMC=root.TCanvas("c_lim_Asymp","canvas with limits for Asymptotic CLs",630,600)
    cMCMC.cd()
    cMCMC.SetGridx(1)
    cMCMC.SetGridy(1)

    hr = cMCMC.DrawFrame(600,0.001,2000,1,"")
    hr.SetXTitle("M_{1} [GeV]")
    hr.SetYTitle("N_{events}") #rightarrow 2l2q

    n=0
    for width in sw:
        expected_graphs[width].SetLineStyle(11+n)
        expected_graphs[width].SetLineColor(root.TColor.GetColorPalette(n))
        expected_graphs[width].Draw("same")
        n+=1

        
    leg = root.TLegend(.66,.55,.94,.92);
   
    leg.SetFillColor(0);
    leg.SetShadowColor(0);
    leg.SetTextFont(42);
    leg.SetTextSize(0.025);
    #   leg.SetBorderMode(0);
    #if(unblind)leg.AddEntry(grobslim_cls, "Asympt. CL_{S} Observed", "LP");
    #leg.AddEntry(gr68_cls, "Asympt. CL_{S}  Expected #pm 1#sigma", "LF");
    #leg.AddEntry(gr95_cls, "Asympt. CL_{S}  Expected #pm 2#sigma", "LF");
    for width in sw:        
        leg.AddEntry(expected_graphs[width], "#Gamma= "+width+" #times M1", "L" ) #rightarrow 2l2q

    leg.Draw();


    root.gPad.RedrawAxis("")
    cMCMC.Update()
    cMCMC.SaveAs("test.eps")
    cMCMC.SaveAs("test.png")
    root.gPad.SetLogy();
    cMCMC.SaveAs("test_log.eps")
    cMCMC.SaveAs("test_log.png")


    #plot as 2d histo
    binsy = [0.001]
    for i in xrange(len(sw)):
        if i != len(sw)-1:
            binsy.append(0.5*(float(sw[i])+float(sw[i+1])))
        else:
            binsy.append(1.5*float(sw[i]))

    print binsy

    binsx=[]
    d1, d2 = root.Double(0), root.Double(0)
    expected_graphs[sw[0]].GetPoint( 0 , d1, d2 )
    last = float(d1)
    for i in xrange( expected_graphs[sw[0]].GetN()):
        expected_graphs[sw[0]].GetPoint( i , d1, d2 )
        binsx.append(0.5*(last+d1))
        last=float(d1)

    binsx.append(last+(last-binsx[expected_graphs[sw[0]].GetN()-1])  )
    binsx[0]=binsx[0]-(binsx[1]-binsx[0])

    print binsx
    binsxa=array('d',binsx)
    binsya=array('d',binsy)
    limhist = root.TH2D("limhist","limhist",len(binsx)-1,binsxa,len(binsy)-1,binsya)
    
    for w in sw:
        for m in xrange( expected_graphs[w].GetN()):
            expected_graphs[w].GetPoint( m , d1, d2 )
            limhist.Fill(d1,float(w),d2)

    
    setColors2d()
    
    limhist.Draw("COLZ")
    
    #root.gPad.RedrawAxis("")
    #cMCMC.Update()
    cMCMC.SaveAs("test2d.eps")
    cMCMC.SaveAs("test2d.png")
##     root.gPad.SetLogZ();
##     cMCMC.SaveAs("test2d_log.eps")
##     cMCMC.SaveAs("test2d_log.png")


if __name__ == "__main__":
    main()
