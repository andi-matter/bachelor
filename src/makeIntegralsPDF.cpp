
//root
#include <TLine.h>
#include <TString.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TMath.h>
#include <TF1.h>
#include <TStyle.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TEfficiency.h>
#include <TLegend.h>
#include <THStack.h>
#include <THistPainter.h>
#include <TText.h>
#include <TSpectrum.h>   // peakfinder
#include <TPolyMarker.h> // peakfinder
#include <TError.h>      // root verbosity level
#include <TSystem.h>     // root verbosity level
#include <TLatex.h>      // root verbosity level
#include <sys/resource.h>
//C, C++
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <numeric>
#include <tuple>
#include <map>
//specific
#include "geometry.h"
#include "analysis.h"
#include "misc.h"
#include "read.h"
#include <linux/limits.h>


int main(int argc, char *argv[]) {
    string rootfileStr = argv[1];
    // char [] = "test"; // rootfileStr;

    char* rootfile;
    char* rootdir;
    string str_obj_file(rootfileStr);
    rootfile = &str_obj_file[0];
    cout << rootfile << endl;
    string str_obj_dir(rootfileStr);
    rootdir = &str_obj_dir[0];
    cout << rootdir << endl;


    cout << " \n hello rootfile \n" << rootfile << "\n" << endl;
    TFile* file = TFile::Open(rootfile, "RECREATE");
    // string rootDir = *(file->cd(""));
    TTree* tree = (TTree*) file->Get("T");

    // char rootDir[] = rootfile;
    cout << " before " << rootdir << endl;
    *(strrchr(rootdir, '/') + 1) = 0; 
    cout << "after " << rootdir << endl;
    cout << " after rootfile " << rootfile << endl;
    // put extra NULL check before if path can have 0 '\' also

    // andrea
    TCanvas cIntegralsWOM("cIntegralsWOM", "cIntegralsWOM", 1000, 1000);
    cIntegralsWOM.Divide(2, 4);

    // andrea
    // after event loop!
    for (int i = 0; i < 8; i++) {
      cIntegralsWOM.cd(i+1);
      TString name("");
      name.Form("integral_hist_%d", i);
      gStyle->SetOptStat(1111);
      tree->Draw(name);      
    }
    string location = rootdir;
    string loc_name = location + "integrals.pdf";
    cout << loc_name << endl;
    cIntegralsWOM.Print((TString) location, "pdf");
    cIntegralsWOM.Clear();

    return 0;
}
