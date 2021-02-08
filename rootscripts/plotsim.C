#include "TCanvas.h"

{
  n = 7;  // cluster size
  gStyle->SetOptStat("e");
  gStyle->SetStatX(0.88);
  gStyle->SetStatY(0.88);
  gStyle->SetStatW(0.15);
  gStyle->SetStatH(0.125);
  gStyle->SetOptFit(1);

  latex = new TLatex();

  pnpt = new TCut(Form("n==%d",n));
  npt = *pnpt;
  c1 = new TCanvas("c1",
                   "Fit Results with Simulated Cluster Data ("
                   +TString(npt.GetTitle())+")",
                   2400,1350);
  c1->SetFrameFillColor(0);
  c1->SetFrameFillStyle(0);
  c1->SetFrameBorderMode(0);
  c1->Divide(2,2);
  
  vu = c1->cd(1);
  vu->SetFillStyle(4000);
  hu = new TH1F("hu","u position residual (#mum)",
                250,-200.,200.);
  hu->GetXaxis()->SetTitle("#Deltau (#mum)");
  T->Draw("1e6*(x-tx)>>hu",npt);
  ru = hu->Fit("gaus","S");
  latex->DrawLatex( 0.5*200.,
                    0.6*hu->GetMaximum(),
                    Form("#sigma = %4.1lf #mum",
                         ru->Parameter(2))
                    );

  vs = c1->cd(2);
  vs->SetFillStyle(4000);
  hs = new TH1F("hs","u slope residual (mrad)",250,-100.,100.);
  hs->GetXaxis()->SetTitle("#Delta#theta (mrad)");
  T->Draw("1e3*(s-ts)>>hs",npt);
  rs = hs->Fit("gaus","S");
  latex->DrawLatex( 0.45*100.,
                    0.6*hs->GetMaximum(),
                    Form("#sigma = %4.1lf mrad", rs->Parameter(2))
                    );

  vt = c1->cd(3); 
  vt->SetFillStyle(4000);
  ht = new TH1F("ht","time offset residual (ns)",250,-10.,10.);
  ht->GetXaxis()->SetTitle("#Deltat_{0} (ns)");
  T->Draw("1e9*(t-tt)>>ht",npt);
  rt = ht->Fit("gaus","S");
  latex->DrawLatex( 0.5*10.,
                    0.6*ht->GetMaximum(),
                    Form("#sigma = %3.1lf ns", rt->Parameter(2))
                    );

  vc = c1->cd(4);
  vc->SetFillStyle(4000);
  hi = 2.;
  ndof = 1.;
  if( n==6 ) {
    hi = 10.;
    ndof = 2.;
  } else if( n==7 ) {
    hi = 15.;
    ndof = 3.;
  }
  hchi2 = new TH1F("hchi2","#chi^{2}",250,0.,hi);
  hchi2->GetXaxis()->SetTitle("#chi^{2}");
  T->Draw("chi2>>hchi2",npt);
  fchi2 = new TF1("fchi2","[1]*TMath::Power(x,[0]/2.-1.)*TMath::Exp(-x/2.)/TMath::Power(2.,[0]/2.)/TMath::Gamma([0]/2.)");
  fchi2->SetParameters(ndof,2.*hchi2->GetBinContent(1));
  fchi2->SetParNames("ndof","Constant");
  hchi2->Fit("fchi2");
  latex->DrawLatex( 0.7*hi,
                    0.6*hchi2->GetMaximum(),
                    Form("dof = %4.2lf", fchi2->GetParameter(0))
                    );

  // Global title
  c1->cd(0);
  txt = new TText(0.43,0.96,Form("Cluster size = %d",n));
  txt->SetTextSize(0.04);
  txt->Draw();
  c1->Update();
  c1->SaveAs(Form("sim-resid-npt%d.png",n));
}

