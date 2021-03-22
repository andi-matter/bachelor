
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
    TString rootfile = argv[1];
    cout << "hello rootfile" << rootfile << endl;
    TFile* file = new TFile(rootfile);
    TTree* tree = (TTree*) file->Get("tree");

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

    return 0;
}
