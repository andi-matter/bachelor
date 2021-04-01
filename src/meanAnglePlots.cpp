// reads finished Rootfile and extracts Phi_ew histograms.
// fits histogram with no channel omitted to gauss and saves it
// makes pdf with histograms where each time one channel gets omitted

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

//andrea
#include "meanAngleFuncs.h"

using namespace std;

int main(int argc, char *argv[]) {
  string rootfileStr = argv[1]; // takes in format e.g. ../RootAnalysis/finishedRootfiles/18_cosmics_vb58_50k_2808_PARTS.root
  // std::cout << "1 " << rootfileStr << endl;
  
  string runNameRaw;
  // cout << rootfileStr.size()-6 << endl;
  for (int i = rootfileStr.size()-6; i >= 0; i--) {
    if (rootfileStr.at(i) == '/' || rootfileStr.at(i) == '\\') {
        if (i == rootfileStr.size()-6) {
          runNameRaw = "EGNAHC_eman_tluafed"; //"default_name_CHANGE";
          // cout << 21 << endl;
        }
        // cout << 23 << endl;
        break;
    }
    // cout << rootfileStr[i] << endl;
    // cout << rootfileStr.at(i) << endl;
    runNameRaw += rootfileStr.at(i);
    // cout << runNameRaw << endl;
    // cout << 22 << endl;
  }
  // printf("Hello world");
  // std::cout << "3 " << runNameRaw << "\n" << endl;

  string runName = "";
  for (int i=0; i<runNameRaw.size(); i++) {
    // cout << runNameRaw.at(runNameRaw.size()-1-i) << endl;
    runName += runNameRaw.at(runNameRaw.size()-1-i);
  }
  // cout << "finished runName " << runName << endl;

  // if not otherwise specified, files will be save to ../RootAnalysis/integralAnalysis/runName
  // HARDCODED PATH
  string saveFolder;
  if (argc < 3) { // || (string(argv[2])).empty() ) {
    saveFolder = "/mnt/d/Programme/RootAnalysis/RootAnalysis/integralAnalysis/" + runName;
    // std::cout << " argc < 3 " << saveFolder <<"DUB" << endl;
  } else {
    // std::cout << "argc not <3  " << argc << endl;
    string saveFolder = argv[2]; // path specified when analysis is called (makeIntegralsPDF.sh)
    // std::cout << saveFolder << endl;
  }

  string allChannelsParam_loc = saveFolder + "/allChannelsParam.txt";
  FILE* allChannelsParam = fopen(allChannelsParam_loc.c_str(), "w");

  string omitChannelsParam_loc = saveFolder + "/omitChannelsParam.txt";
  FILE* omitChannelsParam = fopen(omitChannelsParam_loc.c_str(), "w");


  // open rootfile:
  TFile file(rootfileStr.c_str());
 
  if (file.IsZombie())
    {
        std::cout << "Problem with file " << rootfileStr << "; check if file path is correct!" << endl;
        exit(-1);
    }
  std::cout << file.IsZombie() << endl;

  TTree* tree = new TTree;
  file.GetObject("T", tree);
  tree->GetEntry(1);

  // declare stuff to access data
  int nCh = 8;
  TH1F* omitChannels[nCh];

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);
  gStyle->SetGridColor(16);
  gStyle->SetLineScalePS(1);

  TCanvas canvas("canvas", "Phi_ew for channels", 1557, 2000);
  TPaveLabel title(0.1, 0.96, 0.9, 0.99, "Phi_ew omitting different channels; centered opposite omitted channel");
  TPaveLabel xTitle(0, 0.01, 1, 0.03, "Phi_ew (deg.)");
  TPaveLabel yTitle(0.01, 0, 0.03, 1, "Number of Entries");
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
  graphPad.Divide(2, 4);

  for (int nPad = 1; nPad < 9; nPad++) {
      graphPad.cd(nPad);
      gPad->SetLeftMargin(.065); //.18
      gPad->SetBottomMargin(.052); //.15
      gPad->SetRightMargin(0.065);
      gPad->SetGrid();
  }

  TString histTitle;
  TString histName;
  TString histDraw;

  Int_t xMax = 200;
  Int_t xMin = -200;


  for (int i=0; i<nCh; i++) {
    // omitChannels[i] = (TH1F*) file.Get(Form("Phi_ew_omit_ch%d", i));
    graphPad.cd(i + 1);

    //Naming the histogram and setting upt the draw command:
    histTitle.Form("Omitting Channel %d", i);
    histName.Form("Hist%d", i);
    histDraw.Form("Phi_ew_omit_ch%d>>Hist%d", i, i);

    omitChannels[i] = new TH1F(histName, histTitle, (xMax - xMin), xMin, xMax);

    gStyle->SetTitleSize(0.08, "t"); 
    //histVec[i]->SetTitleSize(0.25, "t");
    //histVec[i]->GetXaxis()->SetTitle("Number of photoelectrons");
    //histVec[i]->GetXaxis()->SetTitleSize(.07);
    omitChannels[i]->GetXaxis()->SetLabelSize(.06);
    //histVec[i]->GetYaxis()->SetTitle("Number of entries");
    //histVec[i]->GetYaxis()->SetTitleSize(.07);
    omitChannels[i]->GetYaxis()->SetLabelSize(.06);
    omitChannels[i]->SetLineColorAlpha(kBlack, 0.7);
    omitChannels[i]->SetFillColorAlpha(kBlack, 0.5);
    omitChannels[i]->SetMarkerStyle(8);
    omitChannels[i]->SetMarkerSize(0.2);
    omitChannels[i]->SetMarkerColorAlpha(kBlack, 0.6);

    tree->Draw(histDraw, "", "HIST");

    TLegend* histLeg = new TLegend(0.64, 0.68, 0.92, 0.85);
    histLeg->SetFillColorAlpha(kWhite, 0); //translucent legend
    histLeg->SetBorderSize(1);
    histLeg->AddEntry((TObject*)0, Form("Entries = %d", (int) omitChannels[i]->GetEntries()));
    // histLeg->AddEntry((TObject*)0, Form("Mean = %1.2f #pm %1.2f", histMeanVec[i], histMeanErrVec[i]), "");
    gStyle->SetLegendTextSize(0.05);

    histLeg->Draw();

  }

title.SetLabel(Form("Phi_ew omitting different Channel; 0deg. = opposite omitted Ch."));

string loc_name = saveFolder + "/phi_ew_omitting.pdf";
std::cout << loc_name << endl;
canvas.SaveAs(loc_name.c_str());

canvas.Clear();



// save phi ew histogram with no omissions too

//Naming the histogram and setting upt the draw command:
TString histTitle2;
TString histName2;
TString histDraw2;
histTitle2.Form("Phi_ew from all channels, centered around channel 2");
histName2.Form("Hist2");
histDraw2.Form("Phi_ew_all_ch>>Hist2");
cout << 2 << endl;
TH1F* allChannels = new TH1F(histName2, histTitle2, (xMax - xMin), xMin, xMax);
cout << 3 << endl;

cout << 1 << endl;
TCanvas canvas2("canvas", "Phi_ew from all channels, centered around channel 2", 2000, 1557);
TPaveLabel xTitle2(0, 0.01, 1, 0.03, "Phi_ew (deg.)");
TPaveLabel yTitle2(0.01, 0, 0.03, 1, "Number of Entries");

xTitle2.SetTextSize(1.2);
yTitle2.SetTextAngle(90);
yTitle2.SetTextSize(.03);
// xTitle2.SetLineColor(0);
// yTitle2.SetLineColor(0);

xTitle2.SetBorderSize(0);
yTitle2.SetBorderSize(0);

xTitle2.SetFillColor(0);
yTitle2.SetFillColor(0);

xTitle2.Draw();
yTitle2.Draw();
TPad graphPad2("Graphs", "Graphs", 0.03, 0.03, 1, 0.96);
graphPad2.Draw();
graphPad2.cd();

gPad->SetLeftMargin(.065); //.18
gPad->SetBottomMargin(.052); //.15
gPad->SetRightMargin(0.065);
gPad->SetGrid();

graphPad2.cd();

gStyle->SetTitleSize(0.05, "t"); 
//histVec[i]->SetTitleSize(0.25, "t");
//histVec[i]->GetXaxis()->SetTitle("Number of photoelectrons");
//histVec[i]->GetXaxis()->SetTitleSize(.07);
// allChannels->GetXaxis()->SetLabelSize(.06);
//histVec[i]->GetYaxis()->SetTitle("Number of entries");
//histVec[i]->GetYaxis()->SetTitleSize(.07);
// allChannels->GetYaxis()->SetLabelSize(.06);
allChannels->SetLineColorAlpha(kBlack, 0.7);
allChannels->SetFillColorAlpha(kBlack, 0.5);
allChannels->SetMarkerStyle(8);
allChannels->SetMarkerSize(0.2);
allChannels->SetMarkerColorAlpha(kBlack, 0.6);

tree->Draw(histDraw2, "", "HIST");

TLegend* histLeg2 = new TLegend(0.1, 0.78, 0.32, 0.85);
histLeg2->SetFillColorAlpha(kWhite, 0); //translucent legend
histLeg2->SetBorderSize(1);
histLeg2->AddEntry((TObject*)0, Form("Entries = %d", (int) allChannels->GetEntries()));
// histLeg->AddEntry((TObject*)0, Form("Mean = %1.2f #pm %1.2f", histMeanVec[i], histMeanErrVec[i]), "");
gStyle->SetLegendTextSize(0.03);

histLeg2->Draw();

string loc_name2 = saveFolder + "/phi_ew_all.pdf";
canvas2.SaveAs(loc_name2.c_str());

return 0;

}