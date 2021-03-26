
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
//specific
#include "geometry.h"
#include "analysis.h"
#include "misc.h"
#include "read.h"


using namespace std;

int main(int argc, char *argv[]) {
  string rootfileStr = argv[1]; // takes in format e.g. ../RootAnalysis/finishedRootfiles/18_cosmics_vb58_50k_2808_PARTS.root
  std::cout << "1 " << rootfileStr << endl;
  
  // string directory = "" + (string) argv[2];
  // find out run file name, assuming it ends in .root
  // std::cout << "2 " << directory << endl;

  string runName = "";
  for (int i = rootfileStr.size()-5; i >= 0; i--) {
    if (rootfileStr.at(i) == '/' || rootfileStr.at(i) == '\\') {
      if (i == rootfileStr.size()-5) {
        runName = "default_name_CHANGE";
      }
      break;
    }
    runName[i] = rootfileStr[i];
  }
  printf("Hello world");
  std::cout << "3 " << runName << "\n" << endl;


  // if not otherwise specified, files will be save to ../RootAnalysis/integralAnalysis/runName
  // HARDCODED PATH
  string saveFolder = "";
  if (argc < 3) { // || (string(argv[2])).empty() ) {
    saveFolder = Form("/mnt/d/Programme/RootAnalysis/RootAnalysis/integralAnalysis/%s", runName.c_str());
    std::cout << " argc < 3 " << endl;
  } else {
    std::cout << "argc not <3  " << argc << endl;
    string saveFolder = argv[2]; // path specified when analysis is called (makeIntegralsPDF.sh)
    std::cout << saveFolder << endl;
  }

  // char* rootfile;
  // char* rootdir;
  // string str_obj_file(rootfileStr);
  // rootfile = &str_obj_file[0];
  // std::cout << rootfile << endl;
  // string str_obj_dir(rootfileStr);
  // rootdir = &str_obj_dir[0];
  // std::cout << rootdir << endl;

  int runNumber = 18; // DUMMY

  //Creating variables to read data from the TTree stored in the .file into our own TTree:
  int runPosition;
  float runEnergy;
  float photonCountPerEvent;
  int nEntries;
  const Int_t nCh = 8;                    //eight channels for WOM in COSMICS
  
  //Style Settings:
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);
  gStyle->SetGridColor(16);
  gStyle->SetLineScalePS(1);


  FILE* integralMeans = fopen((saveFolder + "integralMeans.txt").c_str(), "w");
  TLine* meanLineVec[nCh];                //array of lines for mean values


  std::cout << " \n hello rootfile \n" << rootfileStr << "\n" << endl;
  printf("Analysing    file: %s\n", rootfileStr.c_str());
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

  std::cout << "1" << endl;
  
  // char rootDir[] = rootfile;
  // std::cout << " before " << rootdir << endl;
  // *(strrchr(rootdir, '/') + 1) = 0; 
  // std::cout << "after " << rootdir << endl;
  // std::cout << " after rootfile " << rootfile << endl;
  // put extra NULL check before if path can have 0 '\' also

  //Drawing range for histograms:
  Int_t xMin, xMax;
  xMin = -10;
  xMax = 5000;

  //Retrieve light yield data for all channels
  TH1F* histVec[nCh];                     //array of histograms
  Float_t histMeanVec[nCh];               //array of mean values
  Float_t histMeanErrVec[nCh];            //array of mean value errors
  Float_t histStdDevVec[nCh];             //array of standard deviations
  Float_t histStdDevErrVec[nCh];          //array of standard deviation errors
  Float_t histMaxY[nCh];

  std::cout << "2" << endl;

  TString histName, histTitle, histDraw;  //name and title for histogram; as well as the command to draw it which will be formatted later
  Int_t histEntries;                      //number of entries in each histogram

  //Setting up the canvas:
  TCanvas canvas("canvas", "Light yield for Channels", 1557, 2000);
  TPaveLabel title(0.1, 0.96, 0.9, 0.99, Form("Light yield for different Channels, Run %d, Angles, Position", runNumber));
  TPaveLabel xTitle(0, 0.01, 1, 0.03, "Signal integral [mV*ns]");
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
  //graphPad.SetLeftMargin(0.2);
  //graphPad.SetBottomMargin(0.15);

  std::cout << "3" << endl;

  for (int nPad = 1; nPad < 9; nPad++) {
      graphPad.cd(nPad);
      gPad->SetLeftMargin(.065); //.18
      gPad->SetBottomMargin(.052); //.15
      gPad->SetRightMargin(0.065);
      gPad->SetGrid();
  }

  std::cout << "4" << endl;

  for (int i = 0; i < nCh; i++) {
    graphPad.cd(i + 1);

    //Naming the histogram and setting upt the draw command:
    histTitle.Form("Channel %d (XXX degrees)", i);
    histName.Form("Hist%d", i);
    histDraw.Form("integral_hist_%d>>Hist%d", i, i);
    std::cout << "5 " << i << endl;
    std::cout << histDraw << endl;



    //Drawing the Histogram:
    histVec[i] = new TH1F(histName, histTitle, (xMax - xMin), xMin, xMax);
    std::cout << "5.1 " << i << endl;
    gStyle->SetTitleSize(0.08, "t"); 
    //histVec[i]->SetTitleSize(0.25, "t");
    //histVec[i]->GetXaxis()->SetTitle("Number of photoelectrons");
    //histVec[i]->GetXaxis()->SetTitleSize(.07);
    histVec[i]->GetXaxis()->SetLabelSize(.06);
    //histVec[i]->GetYaxis()->SetTitle("Number of entries");
    //histVec[i]->GetYaxis()->SetTitleSize(.07);
    histVec[i]->GetYaxis()->SetLabelSize(.06);
    histVec[i]->SetLineColorAlpha(kBlack, 0.7);
    histVec[i]->SetFillColorAlpha(kBlack, 0.5);
    histVec[i]->SetMarkerStyle(8);
    histVec[i]->SetMarkerSize(0.2);
    histVec[i]->SetMarkerColorAlpha(kBlack, 0.6);
    std::cout << "5.2 " << i << endl;

    tree->Draw(histDraw, "", "HIST");

    std::cout << "5.3 " << i << endl;

    //Getting histogram maximum (how high to draw the lines):
    histMaxY[i] = histVec[i]->GetBinContent(histVec[i]->GetMaximumBin());

    /*
    //Finding the left edge for mean calculation:
    histLeftEdge[i] = 10 + TMath::Floor(0.1 * histVec[i]->GetMean());
    //Start out at a bin right of the leftmost bin (ignore 0 --> Underflow, 1-9 --> below 0), depending on where the mean is (not starting out at the first bin makes the algorithm more stable;
    //e.g. if the first bin is randomly very small, the condition below will instantly be fulfilled, which we do not want
    float leftSum = 0;
    float rightSum = 0;
    float leftMean = 0;
    float rightMean = 0;
    Double_t binContent;
    int counter;

    while (rightMean <= 1.3 * leftMean)
    {
        if (counter >= 1000)    //Sometimes the condition is not met at all -- in this case, terminate after 1000 (arbitrary number) tries and continue with next histogram
        {
            histLeftEdge[i] == 0;
            break;
        }
        counter += 1;
        leftSum = 0.0;
        rightSum = 0.0;
        for (int bin = 10; bin <= histLeftEdge[i]; bin++)
        {
            binContent = histVec[i]->GetBinContent(bin);
            leftSum += binContent;    //Sum of all bins left of histLeftEdge[i] (including histLeftEdge[i])
        }
        leftMean = (float)leftSum / (histLeftEdge[i] - 9);
        for (int bin = 1; bin <= 10; bin++)
        {
            rightSum += histVec[i]->GetBinContent(histLeftEdge[i] + bin);
        }
        rightMean = (float)rightSum / 10.0;     //Mean of five bins right of histLeftEdge[i] (excluding histLeftEdge[i])
        histLeftEdge[i] += 1;                   //Move histLeftEdge[i] to the right until the condition is met
    }
    photonCut[i] = 0;
    if (histLeftEdge[i] - 11 > 0 && histLeftEdge[i] > 10 + TMath::Ceil(0.1 * histVec[i]->GetMean()))
    {
        photonCut[i] = histLeftEdge[i] - 11;
    }

    //Declaring the leftmost bin for the calculation of the histogram mean and the total light yield:
    histVec[i]->GetXaxis()->SetRange(histLeftEdge[i], xMax); //sets axis range to start at the specified lower bin, since only bins inside axis range are included in GetMean-calculation
    */
    //Obtaining results:
    histMeanVec[i] = histVec[i]->GetMean();
    histMeanErrVec[i] = histVec[i]->GetMeanError();
    histStdDevVec[i] = histVec[i]->GetStdDev();
    histStdDevErrVec[i] = histVec[i]->GetStdDevError();

    std::cout << "6 " << i << endl;

    //Getting histogram sum (total number of photons, for light yield vs f_{max}/f_{min} plot):
    int nCells = histVec[i]->GetNcells();
    nEntries = histVec[i]->GetEntries();
    for (int j = 1; j <= nCells; j++)
    {
        if (histVec[i]->GetBinContent(j) > 0)
        {
            photonCountPerEvent += histVec[i]->GetBinContent(j) * (float)j;
        }
    }

    //Drawing mean lines:

    std::cout << "7 " << i << endl;

    meanLineVec[i] = new TLine(histMeanVec[i], 0, histMeanVec[i], histMaxY[i] * 1.01);
    meanLineVec[i]->SetLineColor(4);
    meanLineVec[i]->SetLineStyle(2);
    meanLineVec[i]->SetLineWidth(4);
    meanLineVec[i]->Draw("same");

    //Drawing lines for leftmost bin included in mean calculation:
    /*if (photonCut[i] > 0)
    {
        leftEdgeLineVec[arg][i] = new TLine(photonCut[i], 0, photonCut[i], histMaxY[arg][i] * 1.01);
        leftEdgeLineVec[arg][i]->SetLineColor(46);
        leftEdgeLineVec[arg][i]->SetLineStyle(5);
        leftEdgeLineVec[arg][i]->SetLineWidth(4);
        leftEdgeLineVec[arg][i]->Draw("same");
    }
    */

    //Resetting Drawing Range:
    histVec[i]->GetXaxis()->SetCanExtend(kTRUE);
    histVec[i]->ExtendAxis(histMeanVec[i] * 5, histVec[i]->GetXaxis());
    histVec[i]->GetXaxis()->SetRangeUser(histMeanVec[i] * (-0.2), TMath::Min(histMeanVec[i] * 4.99, (double)histVec[i]->GetNbinsX()));

    //Customizing the legend:
    TLegend* histLeg = new TLegend(0.44, 0.68, 0.92, 0.85);
    histLeg->SetFillColorAlpha(kWhite, 0); //translucent legend
    histLeg->SetBorderSize(1);
    histLeg->AddEntry(meanLineVec[i], Form("Mean = %1.2f #pm %1.2f", histMeanVec[i], histMeanErrVec[i]), "l");
    histLeg->AddEntry((TObject*)0, Form("Entries = %d", (int) histVec[i]->GetEntries()));
    // histLeg->AddEntry((TObject*)0, Form("Mean = %1.2f #pm %1.2f", histMeanVec[i], histMeanErrVec[i]), "");
    gStyle->SetLegendTextSize(0.05);
    /*if (photonCut[i] > 0)
    {
        histLeg->AddEntry(leftEdgeLineVec[arg][i], Form("Cutoff Value: %d", photonCut[i]), "l");
    }
    */
    //histLeg->AddEntry((TObject*)0, Form("#sigma = %1.2f #pm %1.2f", histStdDevVec[i], histStdDevErrVec[i]), "");
    histLeg->Draw();
  }   //END OF LOOP OVER CHANNELS  

  title.SetLabel(Form("Light yield for different Channels, Run %d, Angle XXX, Position XXX", runNumber));

  // save integral hist mean values & errors & stdev in integralsMeans
  std::fprintf(integralMeans, "#integralMeans\tintegralMeanErr\tStdDev\tStdDevErr\n");
  for (int i=0; i<8; i++) {
  std::fprintf(integralMeans, "%f\t%f\t%f\t%f", histMeanVec[i], histMeanErrVec[i], histStdDevVec[i], histStdDevErrVec[i]);
  std::fprintf(integralMeans, "\n");
  }
  fclose(integralMeans);
  
  // string location = rootdir;
  string loc_name = saveFolder + "/integrals.pdf";
  std::cout << loc_name << endl;
  canvas.SaveAs(loc_name.c_str());
  //cIntegralsWOM.Clear();

  // FILE* MeanList = fopen(Form("%sMeanList.txt", outPath.c_str(), runNumber[arg]), "w");

  return 0;
}
