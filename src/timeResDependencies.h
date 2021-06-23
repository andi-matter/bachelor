//Including root functionalities:
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TLine.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TStyle.h>
#include <TString.h>
#include <TCanvas.h>
#include <TPaveLabel.h>
#include <TPad.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TMath.h>
#include <TLegend.h>
#include <TCut.h>
#include <THStack.h>
#include <TGaxis.h>
#include <TF1.h>
#include <TError.h> // root verbosity level
#include <TApplication.h>
#include <TNtuple.h>
#include <TImage.h>
#include <TAttImage.h>
#include <TSpectrum.h>

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

// root
#include <TMath.h>

using namespace std;

void timeResDependencies(TTree* tree, string rootFilename, string saveFolder, float* positionInfo);

void timeResDependencies(TTree* tree, string rootFilename, string saveFolder, float* positionInfo) {
  int runNumber = positionInfo[0];
	float angleLowerLimit = positionInfo[1];
	float angleUpperLimit = positionInfo[2];
	float posLeftLimit = positionInfo[3];
	float posRightLimit  = positionInfo[4];

  //Style Settings:
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);
  gStyle->SetGridColor(16);
  gStyle->SetLineScalePS(1);
  // gStyle->SetOptTitle(kFALSE);
  // gStyle->SetPalette(kSolar);

  std::string locationTimes = saveFolder + "/timeResDependencies.pdf";

  string histTitles[] = {"#DeltaT of amp. PMT 1", "#DeltaT of amp. PMT 2", "#DeltaT of amp. PMT 3", "#DeltaT of amp. PMT 4", "#DeltaT of t_{12}", "#DeltaT of t_{34}"};

  string histNamesTree[] = {"timeResApprox:minimum_ch10", "timeResApprox:minimum_ch11", "timeResApprox:minimum_ch12", "timeResApprox:minimum_ch13", "timeResApprox:Time_diff_top", "timeResApprox:Time_diff_bot"};

  string histNames[] = {"minCh10", "minCh11", "minCh12", "minCh13", "t12", "t34"};

  TString histDraw[6];
  TString xTitles[] = {"Minimum (mV)", "Minimum (mV)", "Minimum (mV)", "Minimum (mV)", "time (ns)", "time (ns)"};
  TString ylabel = "#DeltaT (ns)";

  TH1F* histVec[6];

  TCanvas canvas("canvas", "Time resolution dependecies", 1557, 2000);
  TPaveLabel title(0.1, 0.96, 0.9, 0.99, Form("Incidence Time; Run %d, [%.1f, %.1f] deg., [%.1f, %.1f]cm", runNumber, angleLowerLimit, angleUpperLimit, posLeftLimit, posRightLimit));
  TPaveLabel xTitle(0, 0.01, 1, 0.03, "");
  TPaveLabel yTitle(0.01, 0, 0.03, 1, ylabel);
  title.SetTextSize(.7);
  xTitle.SetTextSize(.7);
  yTitle.SetTextAngle(90);
  yTitle.SetTextSize(.017);
  xTitle.SetLineColor(0);
  yTitle.SetLineColor(0);
  title.SetBorderSize(0);
  xTitle.SetBorderSize(0);
  yTitle.SetBorderSize(0);
  title.SetFillColor(0);
  xTitle.SetFillColor(0);
  yTitle.SetFillColor(0);
  title.Draw();
  xTitle.Draw();
  yTitle.Draw();

  TPad graphPad("Graphs", "Graphs", 0.03, 0.03, 1, 0.96);
  graphPad.Draw();
  graphPad.cd();
  graphPad.Divide(2, 3);

  Int_t xMin[] = {-50, -50, -50, -10, 80,80};
  Int_t xMax[] = {10, 10, 10, 10, 140, 140};
  Int_t yMin[] = {-10};
  Int_t yMax[] = {10};
  // Int_t binFactor[] = {5, 5, 5, 10, 5, 5, 5, 5};

  for (int nPad = 1; nPad < 7; nPad++) {
		graphPad.cd(nPad);
		gPad->SetLeftMargin(.08); //.18
		gPad->SetBottomMargin(.052); //.15
		gPad->SetRightMargin(0.065);
		gPad->SetGrid();
  }

  for (int i=0; i<6; i++) {
    graphPad.cd(i + 1);
    histDraw[i] = Form("%s>>%s", histNamesTree[i].c_str(), histNames[i].c_str());

    histVec[i] = new TH1F(histNames[i].c_str(), histTitles[i].c_str(), 100, xMin[i], xMax[i]);
    

    gStyle->SetTitleSize(0.08, "t"); 
		
    histVec[i]->GetXaxis()->SetLabelSize(.06);
    histVec[i]->GetYaxis()->SetLabelSize(.06);
    histVec[i]->SetLineColorAlpha(kBlack, 0.7);
    histVec[i]->SetFillColorAlpha(kBlack, 0.5);
    histVec[i]->SetMarkerStyle(kCircle);
    // histVec[i]->SetMarkerSize(5);
    // histVec[i]->SetMarkerColorAlpha(kBlack, 1);
    histVec[i]->SetXTitle(xTitles[i]);
    histVec[i]->GetXaxis()->SetTitle(xTitles[i]);

    tree->Draw(histDraw[i], "", "HIST");

  }
  canvas.SaveAs(locationTimes.c_str());

}