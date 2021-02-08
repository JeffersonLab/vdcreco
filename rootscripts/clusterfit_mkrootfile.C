#include "TTree.h"
#include "TString.h"
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

// bool read_array( ifstream& inp, vector<double>& arr )
// {
//   int n = arr.size();
//   for( size_t i=0; i<arr.size(); ++i ) {
//     inp >> arr[i];
//     if( !inp )
//       break;
//   }
//   if( !inp )
//     return true;
//   return false;
// }

void clusterfit_mkrootfile(const char* infilename="clusterfit.dat")
{
  ifstream inp, inpt;
  double x,s,t,chi2,dx,tx,ts,tt;
  int iev,ispec,iproj,iplane,first,last,npt;
  // const size_t npt = 7;
  // vector<double> y(npt), sig(npt), ty(npt), tsig(npt);
  // vector<double> *py = &y;
  // vector<double> *psig = &sig;
  // vector<double> *pty = &ty;
  // vector<double> *ptsig = &tsig;

  inp.open(infilename);
  if(!inp) {
    cerr << "Cannot open " << infilename << endl;
    return;
  }
  inpt.open("truthdata.dat");
  if(!inpt) {
    cerr << "Cannot open truthdata.dat" << endl;
    return;
  }    

  TString infile(infilename), rootfile(infile);
  Ssiz_t pos = infile.Index(".dat");
  if( pos != kNPOS )
    rootfile.Replace(pos,4,".root");
  else
    rootfile.Append(".root");
  TFile* f = new TFile(rootfile, "RECREATE", "Cluster fit results");
  f->SetCompressionAlgorithm(ROOT::ECompressionAlgorithm::kLZMA);
  f->SetCompressionLevel(7);
  f->cd();

  TTree* tree = new TTree("T","Cluster fit results");
  tree->Branch("ev", &iev);
  tree->Branch("sp", &ispec);
  tree->Branch("uv", &iproj);
  tree->Branch("pl", &iplane);
  tree->Branch("wlo", &first);
  tree->Branch("whi", &last);
  tree->Branch("n", &npt);
  tree->Branch("x", &x);
  tree->Branch("s", &s);
  tree->Branch("t", &t);
  tree->Branch("chi2", &chi2);
  tree->Branch("dx", &dx);
  // tree->Branch("y", &py);
  // tree->Branch("sig", &psig);
  tree->Branch("tx",&tx);
  tree->Branch("ts",&ts);
  tree->Branch("tt",&tt);
  // tree->Branch("ty", &pty);
  // tree->Branch("tsig", &ptsig);

  while(1) {
    int iline;
    inp  >> iline >> iev >> ispec >> iproj >> iplane >> first >> last
         >> npt >> x >> s >> t >> chi2 >> dx;
    if( !inp )
      break;
    // if( read_array(inp,y) )
    //   break;
    // if( read_array(inp,sig) )
    //   break;

    inpt >> iline >> tx >> ts >> tt;
    if( !inpt )
      break;
    // if( read_array(inpt,ty) )
    //   break;
    // if( read_array(inpt,tsig) )
    //   break;
    tree->Fill();
  }

  inp.close();
  inpt.close();
  tree->Write();
  f->Close();
}
