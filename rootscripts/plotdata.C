#include "TCanvas.h"

{
  // Plot 3-parameter fit results

  gStyle->SetOptStat("euo");
  gStyle->SetStatX(0.88);
  gStyle->SetStatY(0.88);
  gStyle->SetStatW(0.15);
  gStyle->SetStatH(0.125);
  gStyle->SetOptFit(1);
  h = 1350;

  simnpt = new TCut("n>4&&n<8");
  simarmL = new TCut("sp==0");
  simarmR = new TCut("sp==1");
  datnptL = new TCut("L.vdc.u1.clsiz>4&&L.vdc.u1.clsiz<8");
  datnptR = new TCut("R.vdc.u1.clsiz>4&&R.vdc.u1.clsiz<8");

  // Entries in fdat giving same number of clusters as all of fsim
  NDATL = 486032;
  NDATR = 486036;
  fdat = new TFile("/zfs/data/apex/rootfiles/apex_4976.root",
                   "READ");
  fsim = new TFile("/zfs/data/apex/RL_u1_cluster_curfit_nopivot.root",
                   "READ");
  fsim->cd();
 
  //========== POSITIONS AND SLOPES 3-p vs 2-p =================

  c1 = new TCanvas("c1", "Cluster Parameters: Position, Slope",
                   16*h/9,h);
  c1->Divide(2,2);

  c1->cd(1);
  huL = new TH1F("huL", "Cluster center in HRS-L u1 plane (m)",
                 450, -0.9, 0.9 );
  huL->GetXaxis()->SetTitle("u (m)");
  T->Draw("-x>>huL", *simnpt && *simarmL);

  c1->cd(2);
  huR = new TH1F("huR", "Cluster center in HRS-R u1 plane (m)",
                 450, -0.9, 0.9 );
  huR->GetXaxis()->SetTitle("u (m)");
  T->Draw("-x>>huR", *simnpt && *simarmR);

  c1->cd(3);
  hsL = new TH1F("hsL", "Slope in HRS-L u1 plane",
                 500, -0.5, 3.5 );
  hsL->GetXaxis()->SetTitle("slope");
  T->Draw("s>>hsL", *simnpt && *simarmL);

  c1->cd(4);
  hsR = new TH1F("hsR", "Slope in HRS-R u1 plane",
                 500, -0.5, 3.5 );
  hsR->GetXaxis()->SetTitle("slope");
  T->Draw("s>>hsR", *simnpt && *simarmR);

  fdat->cd();
  c1->cd(1);
  huLdat = huL->Clone("huLdat");
  ((TH1*)huLdat)->SetLineColor(kRed);
  T->Draw("L.vdc.u1.clpos>>huLdat", *datnptL, "", NDATL );
  huLdat->Draw();
  // huL->Scale(((TH1*)huLdat)->GetEntries()/huL->GetEntries(),"nosw2");
  huL->Draw("SAME");

  legend_uL = new TLegend(0.12, 0.775, 0.3, 0.875);
  legend_uL->AddEntry("huL","3-parameter fit");
  legend_uL->AddEntry("huLdat","2-parameter fit");
  legend_uL->Draw();

  c1->cd(2);
  huRdat = huR->Clone("huRdat");
  ((TH1*)huRdat)->SetLineColor(kRed);
  T->Draw("R.vdc.u1.clpos>>huRdat", *datnptR, "", NDATR );
  huRdat->Draw();
  // huR->Scale(((TH1*)huRdat)->GetEntries()/huR->GetEntries(),"nosw2");
  huR->Draw("SAME");

  legend_uR = new TLegend(0.12, 0.775, 0.3, 0.875);
  legend_uR->AddEntry("huR","3-parameter fit");
  legend_uR->AddEntry("huRdat","2-parameter fit");
  legend_uR->Draw();

  c1->cd(3);
  hsLdat = hsL->Clone("hsLdat");
  ((TH1*)hsLdat)->SetLineColor(kRed);
  T->Draw("1.0/L.vdc.u1.lslope>>hsLdat", *datnptL, "", NDATL );
  // hsL->Scale(((TH1*)hsLdat)->GetEntries()/hsL->GetEntries(),"nosw2");
  ((TH1*)hsLdat)->GetYaxis()->SetRangeUser(0.,
          500.*TMath::Ceil(hsL->GetMaximum()/500.+1.0));
  hsLdat->Draw();
  hsL->Draw("SAME");

  legend_uL = new TLegend(0.12, 0.775, 0.3, 0.875);
  legend_uL->AddEntry("hsL","3-parameter fit");
  legend_uL->AddEntry("hsLdat","2-parameter fit");
  legend_uL->Draw();

  c1->cd(4);
  hsRdat = hsR->Clone("hsRdat");
  ((TH1*)hsRdat)->SetLineColor(kRed);
  T->Draw("1.0/R.vdc.u1.lslope>>hsRdat", *datnptR, "", NDATR );
  //  hsR->Scale(((TH1*)hsRdat)->GetEntries()/hsR->GetEntries(),"nosw2");
  ((TH1*)hsRdat)->GetYaxis()->SetRangeUser(0.,
        500.*TMath::Ceil(hsR->GetMaximum()/500.+1.0));
  hsRdat->Draw();
  hsR->Draw("SAME");

  legend_uR = new TLegend(0.12, 0.775, 0.3, 0.875);
  legend_uR->AddEntry("hsR","3-parameter fit");
  legend_uR->AddEntry("hsRdat","2-parameter fit");
  legend_uR->Draw();

  c1->SaveAs("/zfs/data/apex/pos-slope-2vs3.png");

  //========== TIME OFFSETS =====================

  latex = new TLatex();

  c2 = new TCanvas("c2", "Fitted Time Offsets", 16*h/9,h);
  c2->Divide(2,2);

  fsim->cd();

  c2->cd(1);
  htL = new TH1F("htL", "Time offset in HRS-L u1 plane (ns)",
                 600, -300., 300. );
  htL->GetXaxis()->SetTitle("t_{0} (ns)");
  T->Draw("(1e9*t-8.5)>>htL", *simnpt && *simarmL);

  c2->cd(2);
  htR = new TH1F("htR", "Time offset in HRS-R u1 plane (ns)",
                 600, -300., 300. );
  htR->GetXaxis()->SetTitle("t_{0} (ns)");
  T->Draw("(1e9*t-9.25)>>htR", *simnpt && *simarmR);

  c2->cd(3);
  gStyle->SetStatX(0.4);
  gStyle->SetStatY(0.88);
  gStyle->SetOptStat("euo");
  htLz = new TH1F("htLz", "Time offset in HRS-L u1 plane (ns)",
                  500, -100., 50. );
  htLz->GetXaxis()->SetTitle("t_{0} (ns)");
  T->Draw("(1e9*t-8.5)>>htLz", *simnpt && *simarmL);
  rtL = htLz->Fit("gaus","S","",-7.,7.);
  latex->DrawLatex( -80.,
                    0.6*htLz->GetMaximum(),
                    Form("#sigma = %3.1lf ns", rtL->Parameter(2))
                    );

  c2->cd(4);
  htRz = new TH1F("htRz", "Time offset in HRS-R u1 plane (ns)",
                  600, -100., 50. );
  htRz->GetXaxis()->SetTitle("t_{0} (ns)");
  T->Draw("(1e9*t-9.25)>>htRz", *simnpt && *simarmR);
  rtR = htRz->Fit("gaus","S","",-7.,7.);
  latex->DrawLatex( -80.,
                    0.6*htRz->GetMaximum(),
                    Form("#sigma = %3.1lf ns", rtR->Parameter(2))
                    );

  c2->SaveAs("/zfs/data/apex/t0-LR.png");

  //========== TIME OFFSETS VS X and SLOPE =====================

  gStyle->SetOptStat(0);

  c3 = new TCanvas("c3", "Time Offset Correlations", 16*h/9,h);
  c3->Divide(2,1);

  c3->cd(1);
  gPad->SetGrid();
  htx = new TH2F("htx", "Time offset vs. position, HRS-L u1 plane ",
                 200, -0.6, 0.7, 250, -30., 15.);
  htx->GetXaxis()->SetTitle("u (m)");
  htx->GetYaxis()->SetTitle("t_{0} (ns)");
  T->Draw("(1e9*t-8.5):-x>>htx", *simnpt && *simarmL, "COLZ");

  c3->cd(2);
  gPad->SetGrid();
  hts = new TH2F("hts", "Time offset vs. slope, HRS-L u1 plane",
                 200, 1.1, 1.8, 250, -30., 15.);
  hts->GetXaxis()->SetTitle("slope");
  hts->GetYaxis()->SetTitle("t_{0} (ns)");
  T->Draw("(1e9*t-8.5):s>>hts", *simnpt && *simarmL, "COLZ");

  c3->SaveAs("/zfs/data/apex/toff-correl.png");

  //========== SLOPE VS. X ======================

  c4 = new TCanvas("c4", "Slope vs. Position", 16*h/9,h);
  c4->Divide(2,1);

  fdat->cd();

  c4->cd(1);
  gPad->SetGrid();
  hsx2 = new TH2F("hsx2", "Slope vs. position, L-u1, 2-parameter fit",
                  160, -0.8, 0.8, 200, 0.7, 2.2);
  hsx2->GetXaxis()->SetTitle("u (m)");
  hsx2->GetYaxis()->SetTitle("slope");
  T->Draw("1.0/L.vdc.u1.lslope:L.vdc.u1.clpos>>hsx2",
          *datnptL, "COLZ", NDATL );

  fsim->cd();

  c4->cd(2);
  gPad->SetGrid();
  hsx3 = new TH2F("hsx3", "Slope vs. position, L-u1, 3-parameter fit",
                  160, -0.8, 0.8, 200, 0.7, 2.2);
  hsx3->GetXaxis()->SetTitle("u (m)");
  hsx3->GetYaxis()->SetTitle("slope");
  T->Draw("s:-x>>hsx3", *simnpt && *simarmL, "COLZ");

  c4->SaveAs("/zfs/data/apex/slope-vs-pos-2and3.png");
  
  // (Page 5: s_L n==5, 6, 7 vs. simulation -> TTD problem?)
}

