#include <Riostream.h>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm> 
//#include <cstdlib>
#include "TTree.h"
#include "TH1D.h"
#include "TFile.h"
#include "TROOT.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPaveText.h"
//void getSigmaBands(string fileName);
void plot_golfcourse_Asymptotic_Nevents(bool unblind=false, char* width=0);
void setFPStyle();
bool isZZChannel=true;
double expo_interp(double s2, double s1,  double newM,double m2,double m1){

  if(m1>m2){
    double tmps=s1;
    double tmpm=m1;
    s1=s2;
    m1=m2;
    s2=tmps;
    m2=tmpm;
  }
  double deltaM=m2-m1;
  double alpha=(log(s2)-log(s1)) / deltaM;
  double newS=s1*pow(exp(newM-m1),alpha);
  return newS;
}



double linear_interp( double s2, double s1, double mass, double m2, double m1 ) {
  if(m1>m2){
    double tmps=s1;
    double tmpm=m1;
    s1=s2;
    m1=m2;
    s2=tmps;
    m2=tmpm;
  }
  return (s1 + ( s2-s1 ) * ( mass-m1 ) / ( m2-m1 ));

}



const float intLumi=19.5;
const float BRZZ2l2q=isZZChannel?0.0941:0.2882464;
void plot_golfcourse_Asymptotic_Nevents(bool unblind, char* width){

  bool useNewStyle=true;
  if(useNewStyle)  setFPStyle();

  TFile *fFREQ=0;
  if(width==0)
    fFREQ = new TFile("higgsCombineEXOZZ.Asymptotic.TOTAL.root","READ");
  else{
    char fnam[50];
    sprintf(fnam,"higgsCombineEXOZZ.Asymptotic.%s.TOTAL.root",width);
    fFREQ=new TFile(fnam,"READ");
  }

  TTree *t=(TTree*)fFREQ->Get("limit");

  double mh,limit;
  float quant;
  t->SetBranchAddress("mh",&mh);
  t->SetBranchAddress("limit",&limit);
  t->SetBranchAddress("quantileExpected",&quant);

  //1st loop on tree for preparing mH ordered list
  vector<double> v_mhTMP;
  for(int i=0;i<t->GetEntries();i++){
    t->GetEntry(i);
    if(quant>-1.01&&quant<-0.99){
      v_mhTMP.push_back(mh);
    }
  }
  std::sort(v_mhTMP.begin(),v_mhTMP.end());

  int nMH=v_mhTMP.size();
  int iMH=0;
  vector<double> v_mh, v_median,v_68l,v_68h,v_95l,v_95h, v_obs;
  while(iMH<nMH){
    double mhTMP=v_mhTMP.at(iMH);
    for(int i=0;i<t->GetEntries();i++){
      // int i=j;
      // if(j==t->GetEntries())i=0;
      t->GetEntry(i);
      //cout<<"i "<<i<<flush<<"  m = "<<mh<<endl;
      // if(mh==600)cout<<"$$$$$$$$$ TREE 600 $$$$$$$$$$$$$$"<<endl;

      if(mh!=mhTMP)continue;//follow exactly the order of v_mhTMP

      if(quant>-1.01&&quant<-0.99){
	v_obs.push_back(limit);
	v_mh.push_back(mh);
      }
      else if(quant>0.02&&quant<0.03)v_95l.push_back(limit);
      else if(quant>0.15&&quant<0.17)v_68l.push_back(limit);
      else if(quant>0.49&&quant<0.51)v_median.push_back(limit);
      else if(quant>0.83&&quant<0.85)v_68h.push_back(limit);
      else if(quant>0.965&&quant<0.98){
	//   cout<<"95% -> at M="<<mh<<" I found "<<limit<<endl;
	v_95h.push_back(limit);
      }

      else {cout<<"Error! Quantile =  "<<quant<<endl;}
    }
    iMH++;
  }//end while loop
  cout<<"Out of the loop !"<<endl;
  ////////////////////////////////////////
  ///

  //this actually does NOT contain theory xsect, 
  //it is named like this just for historical reasons 
  string xsect_file_th="./fracBWOutMzzRange_xzz.txt";
  
  ifstream xsect_file(xsect_file_th.c_str(),ios::in);
  if (! xsect_file.is_open()){ cout<<"Failed to open file with xsections"<<endl;}
  float mH, wTMP,frac;
 

  vector<float> v_mhxs, v_xs,  v_brzz2l2q;
  while(xsect_file.good()){
    xsect_file >> mH>> wTMP>>frac;
    if(mH==700)cout<<"M="<<mH<<"  W="<<wTMP<<"   Frac="<<frac<<endl;
    if(mH<600.0)continue;
  
    char wTMP_str[64];
    sprintf(wTMP_str,"%.3f",wTMP);
    if(wTMP==0.35)cout<<"wTMP= "<<wTMP_str<<endl;
    if(strcmp(wTMP_str,width)!=0)continue;

    v_mhxs.push_back(mH);
    v_xs.push_back(frac);
   
 
  }
  cout<<"Size of theor "<<v_mhxs.size()<<endl;
  xsect_file.close();
  ///////////////

  const int nMass= v_mh.size();
  double mass[nMass],mass1[nMass],obs_lim_cls[nMass]; 
  double medianD[nMass];
  double up68err[nMass],down68err[nMass],up95err[nMass],down95err[nMass];
  double xs[nMass];
  int nMassEff=0,nMassEff1=0;
  int nM95=0;
  double mass95[nMass],median95[nMass];
  int nexcluded=0;
  bool excl; 
  for(int im=0;im<nMass;im++){
    if( mass[nMassEff-1]>1600.) cout<<"Array "<<im<<flush<<"  m = "<<v_mh.at(im)<<endl;;
    //protection against messed up jobs
    excl=false;
    if(v_68h.at(im)>=v_95h.at(im) || v_68l.at(im)<=v_95l.at(im) ){
      cout<<"Point at M = "<<v_mh.at(im) <<" excluded: "<<v_95l.at(im)<<"  "<<v_68l.at(im)<<"  "<<v_median.at(im)<<"  "<<v_68h.at(im)<<"  "<<v_95h.at(im)<< endl;
      nexcluded++;
      // continue;
      excl=true; 
    }
    //    if(im%2==1)excl=true;//sample only one half of the points

    //search for right index in theor vectors
    bool found=false;
    int indtmp=0,ind=-1, swapind=-1;
    while(!found){
      if(v_mhxs.at(indtmp)==v_mh.at(im)){found=true;ind=indtmp;}
      if(swapind==-1 && v_mh.at(im)<v_mhxs.at(indtmp)){swapind=indtmp;}
      indtmp++;
      if(size_t(indtmp)==v_mhxs.size()){
	cout<<"!!! m="<<flush<<v_mh.at(im)<<" NOT found in theor matrix."<<endl;
	break;
      }
    }//end while    
   
    if(!found && swapind!=-1){
      ind = swapind;
    }

    
    //if you want the limit on the nr of events, just don't multiply for the th xsect
    //(cards had to be prepared properly, though)
    double fl_xs=double(v_xs.at(ind));

    if(!found){
      cout<<"(2) m="<<v_mh.at(im)<<" NOT found in array of BW fractions"<<endl;
      //      fl_xs  = expo_interp(v_xs.at(ind),v_xs.at(ind-1),v_mh.at(im),v_mhxs.at(ind),v_mhxs.at(ind-1));
      continue;
    }


    mass[nMassEff]=v_mh.at(im);
    //if( mass[nMassEff]==600.0)cout<<"=============> 600 !!!"<<endl;
    obs_lim_cls[nMassEff]=v_obs.at(im)*fl_xs;
    nMassEff++;
    if(!excl){
      mass1[nMassEff1]=v_mh.at(im);
      medianD[nMassEff1]=v_median.at(im)*fl_xs;
      up68err[nMassEff1]=(v_68h.at(im)-v_median.at(im))*fl_xs;
      down68err[nMassEff1]=(v_median.at(im)-v_68l.at(im))*fl_xs;
      cout<<"M="<<mass1[nMassEff1]<<"  Median="<<medianD[nMassEff1]<<endl;
      
      //scale factor 100 for making the xsect visible
      xs[nMassEff1]=fl_xs;//*100.0;

      nMassEff1++;
      

      bool skip95= false;//
      
      if(skip95 )continue;
      mass95[nM95]=v_mh.at(im);
      median95[nM95]=v_median.at(im)*fl_xs;
      up95err[nM95]=(v_95h.at(im)-v_median.at(im))*fl_xs;
      down95err[nM95]=(v_median.at(im)-v_95l.at(im))*fl_xs;
   
      //  cout<<"M95: "<< mass95[nM95]<<" "<<median95[nM95]<<" +"<<up95err[nM95]<<"   -"<< down95err[nM95]<<
      //	" ("<<v_95h.at(im) <<" - "<<v_median.at(im) <<")"<<endl;
      nM95++; 
    }//end if not excluded mass point
  }//end loop over im (mass points)
  cout<<"Excluded "<<nexcluded<<" sick mass points."<<endl;

  


  //  cout<<"Working on TGraph"<<endl;
  TGraphAsymmErrors *grobslim_cls=new TGraphAsymmErrors(nMassEff,mass,obs_lim_cls);
  grobslim_cls->SetName("LimitObservedCLs");
  TGraphAsymmErrors *grmedian_cls=new TGraphAsymmErrors(nMassEff1,mass1,medianD);
  grmedian_cls->SetName("LimitExpectedCLs");
  TGraphAsymmErrors *gr68_cls=new TGraphAsymmErrors(nMassEff1,mass1,medianD,0,0,down68err,up68err);
  gr68_cls->SetName("Limit68CLs");
  TGraphAsymmErrors *gr95_cls=new TGraphAsymmErrors(nM95,mass95,median95,0,0,down95err,up95err);
  gr95_cls->SetName("Limit95CLs");

  // TGraphAsymmErrors *grthSM=new TGraphAsymmErrors(nMassEff1,mass1,xs,0,0,0,0);//xs_downerr,xs_uperr);
  TGraph *grthSM=new TGraph(nMassEff1,mass1,xs);//xs_downerr,xs_uperr);
  grthSM->SetName("SMXSection");
 
  // cout<<"Plotting"<<endl;
  double fr_left=590.0, fr_down=0.0005,fr_right=2020.0,fr_up=1.0;
  if(!isZZChannel){fr_left=500.0, fr_down=0.005,fr_right=2600.0,fr_up=30.0;}
  TCanvas *cMCMC=new TCanvas("c_lim_Asymp","canvas with limits for Asymptotic CLs",630,600);
  cMCMC->cd();
  cMCMC->SetGridx(1);
  cMCMC->SetGridy(1);
  // draw a frame to define the range

  TH1F *hr = cMCMC->DrawFrame(fr_left,fr_down,fr_right,fr_up,"");
  TString VV = "ZZ";
  if(!isZZChannel)VV="WW";
  hr->SetXTitle("M_{G*} [GeV]");
  hr->SetYTitle("N events excl. at 95% C.L.");// #rightarrow 2l2q
  // cMCMC->GetFrame()->SetFillColor(21);
  //cMCMC->GetFrame()->SetBorderSize(12);
  
  gr95_cls->SetFillColor(kYellow);
  gr95_cls->SetFillStyle(1001);//solid
  gr95_cls->SetLineStyle(kDashed);
  gr95_cls->SetLineWidth(3);
  gr95_cls->GetXaxis()->SetTitle("M_{G*} [GeV]");
  gr95_cls->GetYaxis()->SetTitle("N events excl. at 95% C.L.");// #rightarrow 2l2q
  gr95_cls->GetXaxis()->SetRangeUser(fr_left,fr_right);
  
  gr95_cls->Draw("3");
  
  gr68_cls->SetFillColor(kGreen);
  gr68_cls->SetFillStyle(1001);//solid
  gr68_cls->SetLineStyle(kDashed);
  gr68_cls->SetLineWidth(3);
  gr68_cls->Draw("3same");
  grmedian_cls->GetXaxis()->SetTitle("M_{G*} [GeV]");
  grmedian_cls->GetYaxis()->SetTitle("N events excl. at 95% C.L.");// #rightarrow 2l2q
  grmedian_cls->SetMarkerStyle(24);//25=hollow squre
  grmedian_cls->SetMarkerColor(kBlack);
  grmedian_cls->SetLineStyle(2);
  grmedian_cls->SetLineWidth(3);
  //grmedian_cls->SetMinimum(0.0);
  // grmedian_cls->SetMaximum(8.0);
 
  grobslim_cls->SetMarkerColor(kBlack);
  grobslim_cls->SetMarkerStyle(21);//24=hollow circle
  grobslim_cls->SetMarkerSize(1.0);
  grobslim_cls->SetLineStyle(1);
  grobslim_cls->SetLineWidth(3);
  
  grthSM->SetLineColor(kRed);
  grthSM->SetLineWidth(2);
  grthSM->SetLineStyle(kSolid);
  grthSM->SetFillColor(kRed);
  grthSM->SetFillStyle(3344);

  //  grthSM->Draw("L3");

  grmedian_cls->Draw("L");
  if(unblind)grobslim_cls->Draw("LP");

 //draw grid on top of limits
  gStyle->SetOptStat(0);
  TH1D* postGrid = new TH1D("postGrid","",1,fr_left,fr_right);
  postGrid->GetYaxis()->SetRangeUser(fr_down,fr_up);
  postGrid->Draw("AXIGSAME");

  TLine *l1=new TLine();
  l1->SetLineStyle(1);
  l1->SetLineWidth(2.0);
  l1->SetLineColor(kRed);
  //  l1->DrawLine(200.0,1.0,600.0,1.0);
  //  cMCMC->Update();
  cMCMC->RedrawAxis("");
   gPad->RedrawAxis("");
  // hr->GetYaxis()->DrawClone();
   cMCMC->Update();
  


  //more graphics
  TLegend *leg = new TLegend(.46,.75,.94,.92);
  //   TLegend *leg = new TLegend(.35,.71,.90,.90);
   leg->SetFillColor(0);
   leg->SetShadowColor(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.025);
   //   leg->SetBorderMode(0);
   if(unblind)leg->AddEntry(grobslim_cls, "Asympt. CL_{S} Observed", "LP");
   leg->AddEntry(gr68_cls, "Asympt. CL_{S}  Expected #pm 1#sigma", "LF");
   leg->AddEntry(gr95_cls, "Asympt. CL_{S}  Expected #pm 2#sigma", "LF");
   leg->Draw();
   
 if(useNewStyle){
   TPaveText* cmslabel = new TPaveText( 0.145, 0.953, 0.6, 0.975, "brNDC");
   cmslabel->SetFillColor(kWhite);
   cmslabel->SetTextSize(0.038);
   cmslabel->SetTextAlign(11);
   cmslabel->SetTextFont(62);
   cmslabel->SetBorderSize(0);
   //   std::string leftText = "CMS Preliminary 2011";
   std::string leftText = "CMS";
   std::string units = "fb ^{-1}";
   char lumiText[300];
   sprintf( lumiText, "%.1f %s", intLumi, units.c_str());
   cmslabel->AddText(Form("%s,  #sqrt{s} = 8 TeV, %s", leftText.c_str(), lumiText));
   //cmslabel->Draw();

   TPaveText* label_sqrt = new TPaveText(0.4,0.953,0.96,0.975, "brNDC");
   label_sqrt->SetFillColor(kWhite);
   label_sqrt->SetBorderSize(0);
   label_sqrt->SetTextSize(0.038);
   label_sqrt->SetTextFont(62);   
   label_sqrt->SetTextAlign(31); // align right
   // label_sqrt->AddText("#sqrt{s} = 7 TeV");
   label_sqrt->AddText(Form("%s, L = %s at  #sqrt{s} = 8 TeV", leftText.c_str(), lumiText));
   label_sqrt->Draw();

   }
   else{

   TLatex * latex = new TLatex();
   latex->SetNDC();
   latex->SetTextSize(0.04);
   latex->SetTextAlign(31);
   latex->SetTextAlign(11); // align left 
   latex->DrawLatex(0.18, 0.96, "CMS preliminary 2012");
   latex->DrawLatex(0.60,0.96,Form("%.1f fb^{-1} at #sqrt{s} = 8 TeV",intLumi));
   
   }

   TLine *l1b=new TLine();
   l1b->SetLineStyle(1);
   l1b->SetLineWidth(2.0);
   l1b->SetLineColor(kRed);
   //l1b->DrawLine(200.0,1.0,600.0,1.0);
   cMCMC->Update();
   
   
   // cMCMC->RedrawAxis("");
   gPad->RedrawAxis("");
   // hr->GetYaxis()->DrawClone();
   cMCMC->Update();
   char fnam[50];
   if(width != 0){
     sprintf(fnam,"EXOZZ_2l2q_UL_Asymptotic_%s.root",width);
     cMCMC->SaveAs(fnam);
     sprintf(fnam,"EXOZZ_2l2q_UL_Asymptotic_%s.eps",width);
     cMCMC->SaveAs(fnam);
     sprintf(fnam,"EXOZZ_2l2q_UL_Asymptotic_%s.png",width);
     cMCMC->SaveAs(fnam);
     gPad->SetLogy();
     sprintf(fnam,"EXOZZ_2l2q_UL_Asymptotic_%s_log.eps",width);
     cMCMC->SaveAs(fnam);
     sprintf(fnam,"EXOZZ_2l2q_UL_Asymptotic_%s_log.png",width);
     cMCMC->SaveAs(fnam);
   }
   else{
     cMCMC->SaveAs("EXOZZ_2l2q_UL_Asymptotic.root");
     cMCMC->SaveAs("EXOZZ_2l2q_UL_Asymptotic.eps");
     cMCMC->SaveAs("EXOZZ_2l2q_UL_Asymptotic.png");
     gPad->SetLogy();
     cMCMC->SaveAs("EXOZZ_2l2q_UL_Asymptotic_log.eps");
     cMCMC->SaveAs("EXOZZ_2l2q_UL_Asymptotic_log.png");
  // cMCMC->SaveAs("ClsLimit_1fb.png");
   }

   if(width==0)
     sprintf(fnam,"AsymptoticCLs_TGraph.root");
   else
     sprintf(fnam,"AsymptoticCLs_TGraph_%s.root",width);

   TFile *outfile=new TFile(fnam,"RECREATE");
   outfile->cd();
   if(unblind)grobslim_cls->Write();
   grmedian_cls->Write();
   outfile->Close();


}//end main

void setFPStyle(){


  gStyle->SetPadBorderMode(0);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetCanvasDefH(600); //Height of canvas
  gStyle->SetCanvasDefW(600); //Width of canvas
  gStyle->SetCanvasDefX(0);   //POsition on screen
  gStyle->SetCanvasDefY(0);

  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.15);//0.13);
  gStyle->SetPadLeftMargin(0.15);//0.16);
  gStyle->SetPadRightMargin(0.05);//0.02);



 // For the Pad:
  gStyle->SetPadBorderMode(0);
  // gStyle->SetPadBorderSize(Width_t size = 1);
  gStyle->SetPadColor(kWhite);
  gStyle->SetPadGridX(false);
  gStyle->SetPadGridY(false);
  gStyle->SetGridColor(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);

  // For the frame:
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(1);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameLineColor(1);
  gStyle->SetFrameLineStyle(1);
  gStyle->SetFrameLineWidth(1);

  gStyle->SetAxisColor(1, "XYZ");
  gStyle->SetStripDecimals(kTRUE);
  gStyle->SetTickLength(0.03, "XYZ");
  gStyle->SetNdivisions(510, "XYZ");
  gStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  gStyle->SetPadTickY(1);
  gStyle->SetGridColor(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);


  gStyle->SetTitleColor(1, "XYZ");
  gStyle->SetTitleFont(42, "XYZ");
  gStyle->SetTitleSize(0.05, "XYZ");
  // gStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // gStyle->SetTitleYSize(Float_t size = 0.02);
  gStyle->SetTitleXOffset(1.15);//0.9);
  gStyle->SetTitleYOffset(1.3); // => 1.15 if exponents
  gStyle->SetLabelColor(1, "XYZ");
  gStyle->SetLabelFont(42, "XYZ");
  gStyle->SetLabelOffset(0.007, "XYZ");
  gStyle->SetLabelSize(0.045, "XYZ");

  gStyle->SetPadBorderMode(0);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetTitleTextColor(1);
  gStyle->SetTitleFillColor(10);
  gStyle->SetTitleFontSize(0.05);


  
}


