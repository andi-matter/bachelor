//root
#include <TLine.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TStyle.h>
#include <TString.h>
#include <TCanvas.h>
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

//C, C++
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;






void strip_ext(char *fname)
{
	char *end = fname + strlen(fname);

	while (end > fname && *end != '.')
	{
		--end;
	}

	if (end > fname)
	{
		*end = '\0';
	}
}

vector<string> split(const string &str, const string &delim)
{
	vector<string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == string::npos)
			pos = str.length();
		string token = str.substr(prev, pos - prev);
		if (!token.empty())
			tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}
void extractIntegerWords(string str) 
{ 
    stringstream ss;     
  
    /* Storing the whole string into string stream */
    ss << str; 
  
    /* Running loop till the end of the stream */
    string temp; 
    int found; 
    while (!ss.eof()) { 
  
        /* extracting word by word from stream */
        ss >> temp; 
  
        /* Checking the given word is integer or not */
        if (stringstream(temp) >> found) 
            cout << found << " "; 
  
        /* To save from space at the end of string */
        temp = ""; 
    } 
} 

/*******************
__ FUNCTIONS ______
*******************/

int main(int argc, char *argv[]){

	cout<< "VALUE: "<<argv[1]<<endl;

	string fileLocation=string(argv[1]);
	vector<string> splitted=split(string(argv[1]),"//");
	string run_name=splitted[1];
	vector<string> runInformations=split(run_name.c_str(),"_"); 
	//26_pos7_angle0_e52_ch32.root
	//1_calib_vb58_sipm5d


	// parse input arguments
	/*int run_nr = atoi(runInformations[1]);
	if (s1.find("pos") != std::string::npos) {
    std::cout << "found!" << '\n';
	}
	int pos = atoi(extractIntegerWords[runInformations[2]]);
	string WC_v = (string)argv[5];
	//string particle = (string)argv[5];
	double energy = (double)atoi(extractIntegerWords[runInformations[2]])/10;
	//string target_wom = (string)argv[7];
*/

	/***** 
	__ INITIALIZE ___________________________
	*****/

	// ROOT GUI
	bool is_interactive = 0;
	// amp (0), charge (1)
	bool is_charge = 1;	
	// store results
	bool store_result = 1;
	bool legend=false;


	TApplication * ROOTapp;
	if (is_interactive){ROOTapp = new TApplication("ROOT interactive", &argc, argv);}
	

	// style settings
	// gStyle->SetErrorX(1);
	// gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(0);
	// gStyle->SetStatY(0.9);
	// gStyle->SetStatX(0.9);
	// gStyle->SetStatW(0.4);
	// gStyle->SetStatH(0.5); 
	gStyle->SetGridStyle(3);
	gStyle->SetGridWidth(1);
	gStyle->SetGridColor(16);
	// gStyle->SetImageScaling(30.);
	gStyle->SetLineScalePS(1); // export high resolution .pdf
	float alpha = 0.1;
	int lineW = 2;

	

	// open tree
	// string root_dir = "..//runs/"+run_name+"/"+run_name+".root";
	string out_dir = "./ly_histograms/charge/"; // dir. to export plots and txt file
	// export file names
	string out_pdf_name = Form("light_yield_%s.pdf",run_name.c_str()) ;// histogram pdf filename
	//string out_values_list_name = Form("mpv_%s.txt",target_wom.c_str()) ; // MPV list filename
	//string out_values_err_list_name = Form("mpv_err_%s.txt",target_wom.c_str()) ; // MPV err list

	TFile* file = new TFile(fileLocation.c_str());
	TTree* tree;
	file->GetObject("T",tree);


	// default hist x-axis maximum
	int upperA = 20000, upperB = 100, upperC = 450, upperD = 450;

	/***** 
	__ PE DISTRIBUTION _______
	*****/

	int n_ch = 31 ; // number of SiPM channels
	TH1F * h_vec[n_ch]; // histograms
	TLine * ln_vec[n_ch]; // vertical lines
	TF1 * fit_vec_g[n_ch], * fit_vec_l[n_ch], * fit_vec_v[n_ch];; // fit functions
	double amp_max[n_ch], peak[n_ch], peak_err[n_ch], rchi2_g[n_ch]; // gauss fit results
	int h_entries[n_ch];

	TCanvas *C1;
	C1 = new TCanvas("ly_dist","",1200,700);
	C1->Divide(8,4);

	//___ LOOP OVER CHANNELS, no SUM yet ___
	for (int i = 0; i < n_ch; ++i)
	{
		TString h_name1, h_title1, draw_cmnd1, cut_cmnd1;

		// set current wom id
		string wom_id;
		if (i<8)
		{wom_id = "WOM-D";}
		else if (i>=8 && i<16)
		{wom_id = "WOM-C";}
		else if (i>=16 && i<24)
		{wom_id = "WOM-A";}
		else if (i>=24 && i<31)
		{wom_id = "WOM-B";}


		/***** 
		__ DRAW HISTOGRAM ______________________________
		*****/

		int Xmin = -1000, Xmax = -2000; // histogram range
		double lan_range_lo = 0.5, lan_range_hi = Xmax-50; // landau fit range
		float frac_l=0.5,frac_u=0.4; // gauss fit range, threshold fraction

		C1->cd(i+1);
		h_name1.Form("h%d",i);
		//h_title1.Form("%s, run%d, pos%d, %s, %1.1f GeV, ch%d",wom_id.c_str(),run_nr,pos,particle.c_str(),energy,i);
		h_title1.Form("%s, ch%d",run_name.c_str(),i);

		// draw_cmnd1.Form("Integral[%d]>>h%d",i,i);
		draw_cmnd1.Form("Integral[%d]>>h%d",i,i);
		cut_cmnd1.Form("");
		// cut_cmnd1.Form("Integral[%d]>0",i);
		
		// canvas style
		// gPad->SetLogy();
		// gPad->SetGridx(); gPad->SetGridy();
		gPad->SetRightMargin(0.00);
		gPad->SetLeftMargin(.12);
		
		int n_bins = 150;
		// int n_bins = (int)(Xmax - Xmin)*1;

		// h_vec[i] = new TH1F(h_name1,h_title1,n_bins,2,1);
		h_vec[i] = new TH1F(h_name1,h_title1,n_bins,Xmin-0.5,Xmax-0.5);

		h_vec[i]->GetXaxis()->SetTitle("pulse-height(charge) [mv #times ns]");
		// h_vec[i]->GetXaxis()->SetTitle("number-of-photoelectrons [N_{pe}]");
		h_vec[i]->GetXaxis()->SetTitleOffset(1.3);
		h_vec[i]->GetYaxis()->SetTitle("entries");
		h_vec[i]->SetLineColorAlpha(kBlack,0.7);
		h_vec[i]->SetFillColorAlpha(kBlack,alpha);
		h_vec[i]->SetMarkerStyle(8);
		h_vec[i]->SetMarkerSize(0.2);
		h_vec[i]->SetMarkerColorAlpha(kBlack,0.6);

		tree->Draw(draw_cmnd1,cut_cmnd1,"HISTE");


		
	
		/***** 
		__ Maximum in range_________________
		*****/
		double h_mean =  h_vec[i]->GetMean();
    	double h_max_y = h_vec[i]->GetMaximum();

    	// get histogram maximum value & position in range (buggy...)
		// h_vec[i]->GetXaxis()->SetRangeUser(0,100);
		amp_max[i] = h_vec[i]->GetXaxis()->GetBinCenter(h_vec[i]->GetMaximumBin());
		float max = h_vec[i]->GetMaximum();
		int lower_bin = h_vec[i]->GetMaximumBin();
		int upper_bin = h_vec[i]->GetMaximumBin();
		// h_vec[i]->GetXaxis()->UnZoom();
		// printf("ch%d max amp: %f\n",i,amp_max[i]);		

		ln_vec[i] = new TLine(amp_max[i],0.1,amp_max[i],h_max_y);
		ln_vec[i]->SetLineColor(8);
		ln_vec[i]->SetLineWidth(1);
		ln_vec[i]->SetLineStyle(5);
		ln_vec[i]->Draw("same");

		/***** 
		__ FIT PEAK: GAUSS ______________________________
		*****/
		printf("max %1.1f\n",max);
		printf("h_max_y %1.1f\n",h_max_y );

		// fit range ± 0.5x maximum
		// limit lower range to +0.5 Npe
		while (h_vec[i]->GetBinContent(lower_bin) > max*frac_l && (h_vec[i]->GetBinLowEdge(lower_bin))>0.5 ) {lower_bin--;}
		while (h_vec[i]->GetBinContent(upper_bin) > max*frac_u) {upper_bin++;}

		float r1,r2;
		r1 = h_vec[i]->GetBinLowEdge(lower_bin);
		r2 = h_vec[i]->GetBinCenter(upper_bin)+0.5;
		printf("%1.1f %1.1f\n",r1,r2 );

		// fit_vec_g[i] = new TF1("g_fit","gaus",0.5,30);
		fit_vec_g[i] = new TF1("g_fit","gaus",r1,r2);
		fit_vec_g[i]->SetParameter(1,10);
		fit_vec_g[i]->SetLineColor(4);
		fit_vec_g[i]->SetLineStyle(1);
		// fit_vec_g[i]->SetLineStyle(5);
		fit_vec_g[i]->SetNpx(1000); // draw function with high resolution 
		h_vec[i]->Fit("g_fit","RQM");
		h_vec[i]->Draw("sameFUNC");

		Double_t par_g[3]; // to store fit results
		fit_vec_g[i]->GetParameters(&par_g[0]);

		// results
		peak[i] = fit_vec_g[i]->GetParameter(1);
		peak_err[i] = fit_vec_g[i]->GetParError(1);

		double sigma_g = fit_vec_g[i]->GetParameter(2);
		double sigma_g_err = fit_vec_g[i]->GetParError(2);
		double rchi2_g = fit_vec_g[i]->GetChisquare()/fit_vec_g[i]->GetNDF();

		// draw line for MPV
		TLine * ln_gauss_mpv = new TLine(peak[i],0,peak[i],h_max_y);
		ln_gauss_mpv->SetLineColor(4);
		ln_gauss_mpv->SetLineStyle(3);
		ln_gauss_mpv->Draw("same");

		// custom histogram legend with fit results
		TLegend *h_lan_leg = new TLegend(0.53,0.50,1.0,0.9);
		h_lan_leg->SetTextSize(0.035);
		h_lan_leg->AddEntry(h_vec[i],Form("#bf{data}"),"elpf");
		h_lan_leg->AddEntry((TObject*)0,Form("entries = %1.f",h_vec[i]->GetEntries()),"");
		h_lan_leg->AddEntry((TObject*)0,Form("mean = %1.f",h_vec[i]->GetMean()),"");
		h_lan_leg->AddEntry(ln_vec[i],Form("distribution max.: %1.1f",amp_max[i]),"l");
		// h_lan_leg->AddEntry((TObject*)0,Form("#sigma_{L} = %1.2f #pm %1.2f ",lan_sigma,lan_sigma_err),"");
		h_lan_leg->AddEntry(fit_vec_g[i],Form("Gaussian fit"),"l");
		h_lan_leg->AddEntry((TObject*)0,Form("#chi^{2}/ndf = %1.1f",rchi2_g),"");
		h_lan_leg->AddEntry(ln_gauss_mpv,Form("MPV = %1.2f #pm %1.2f ",peak[i],peak_err[i]),"l");
		h_lan_leg->AddEntry((TObject*)0,Form("#sigma = %1.2f #pm %1.2f",sigma_g,sigma_g_err),"");
		if(legend) h_lan_leg->Draw();


		/***** 
		__ FIT PEAK: LANDAU ______________________________
		*****/

		fit_vec_l[i] = new TF1("fit_l","landau",0.5,lan_range_hi);
		fit_vec_l[i]->SetLineColor(2);
		fit_vec_l[i]->SetNpx(1000); // draw function with high resolution 
		// fit_vec_l[i]->SetParameters(&par_g[0]);

		h_vec[i]->Fit("fit_l","RQMN");
		// h_vec[i]->Draw("sameFUNC");

		 // store fit results
		double mpshift  = -0.22278298;
		double lan_sigma = fit_vec_l[i]->GetParameter(2);
		double lan_sigma_err = fit_vec_l[i]->GetParError(2);
		float lan_mpv = fit_vec_l[i]->GetParameter(1);
		float lan_mpv_err = fit_vec_l[i]->GetParError(1);
		float lan_rchi2 = fit_vec_l[i]->GetChisquare()/fit_vec_l[i]->GetNDF();
		
		double lan_mpv_c = lan_mpv + mpshift*lan_sigma ;
		double lan_mpv_c_err = sqrt(lan_mpv_err*lan_mpv_err + (mpshift*lan_sigma_err)*(mpshift*lan_sigma_err)) ;
		
		

		// // draw line for MPV
		// TLine * ln_lan_mpv = new TLine(lan_mpv_c,0,lan_mpv_c,h_max_y);
		// ln_lan_mpv->SetLineColor(2);
		// ln_lan_mpv->SetLineStyle(2);
		// ln_lan_mpv->Draw("same");

		// // custom histogram legend with fit results
		// TLegend *h_lan_leg = new TLegend(0.53,0.30,1.0,0.9);
		// h_lan_leg->SetTextSize(0.04);
		// h_lan_leg->AddEntry(h_vec[i],Form("#bf{data}"),"elpf");
		// h_lan_leg->AddEntry((TObject*)0,Form("entries = %1.f",h_vec[i]->GetEntries()),"");
		// h_lan_leg->AddEntry(ln_vec[i],Form("dist. max. = %1.2f ",amp_max[i]),"l");
		// h_lan_leg->AddEntry(fit_vec_l[i],Form("Landau fit"),"l");
		// h_lan_leg->AddEntry((TObject*)0,Form("#chi^{2}_{L}/ndf = %1.1f",lan_rchi2),"");
		// h_lan_leg->AddEntry(ln_lan_mpv,Form("MPV_{L} = %1.2f #pm %1.2f ",lan_mpv_c,lan_mpv_c_err),"l");
		// // h_lan_leg->AddEntry((TObject*)0,Form("#sigma_{L} = %1.2f #pm %1.2f ",lan_sigma,lan_sigma_err),"");
		// h_lan_leg->AddEntry(fit_vec_g[i],Form("Gauss fit"),"l");
		// h_lan_leg->AddEntry((TObject*)0,Form("#chi^{2}_{G}/ndf = %1.1f",rchi2_g),"");
		// h_lan_leg->AddEntry(ln_gauss_mpv,Form("MPV_{G} = %1.2f #pm %1.2f ",peak[i],peak_err[i]),"l");
		// h_lan_leg->AddEntry((TObject*)0,Form("#sigma_{G} = %1.2f #pm %1.2f ",sigma_g,sigma_g_err),"");
		// h_lan_leg->Draw();
		
		


		// print results
		if (i<n_ch)
		{printf("%s, ch%d\n",run_name.c_str(),1 );}
		printf("Gauss Fit:   mean: %1.2f ± %1.2f | red. chi2 = %1.2f \n",peak[i],peak_err[i],rchi2_g);
		printf("Landau Fit:   MPV: %1.2f ± %1.2f | red. chi2 = %1.2f \n",lan_mpv_c,lan_mpv_err,lan_rchi2);
    	// printf("Vavilov Fit: MPV: %1.2f         | red. chi2 = %1.2f \n",vav_max,vav_rchi2);

	} // ---> end loop over channels + sum


	/***** 
	__ EXPORT ______________________________
	*****/

//	printf("%s %d %d %s \n",particle.c_str(),run_nr,pos,WC_v.c_str() );

	if (store_result)
	{
		/*FILE *mpv_list, *mpv_err_list;
		mpv_list = fopen(Form("%s%s",out_dir.c_str(),out_values_list_name.c_str()),"a");
		mpv_err_list =fopen(Form("%s%s",out_dir.c_str(),out_values_err_list_name.c_str()),"a");

		//   // print date to file
		// time_t now;
		// time(&now); 
		// fprintf(mpv_list,"\nlight yield analysis - %s\n",ctime(&now));
		// fprintf(mpv_err_list,"\nlight yield analysis - %s\n",ctime(&now));

		for (int i = 0; i < n_ch; ++i){fprintf(mpv_list, "%f ",peak[i] );}
		for (int i = 0; i < n_ch; ++i){fprintf(mpv_err_list, "%f ",peak_err[i] );}

		//fprintf(mpv_list, "%s %d %d %s\n",particle.c_str(),run_nr,pos,WC_v.c_str());
		//fprintf(mpv_err_list, "%s %d %d %s\n",particle.c_str(),run_nr,pos,WC_v.c_str());

*/
		gErrorIgnoreLevel = kError; // suppress root terminal output 
		C1->SaveAs(Form("%s%s",out_dir.c_str(),out_pdf_name.c_str()));
		gErrorIgnoreLevel = kUnset;

	}

	if (is_interactive){ROOTapp->Run();}
	

  	return 0;
}