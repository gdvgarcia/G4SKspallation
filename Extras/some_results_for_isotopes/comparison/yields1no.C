// Read the tree and make transverse distance histos plot

#include "TCanvas.h"
#include "TROOT.h"
#include "TH1.h"
#include "TTree.h"
#include "TFile.h"
#include "TLegend.h"
#include "TLatex.h"

#include <iostream> 
#include <vector> 

int yields1no(){




	TH1* h = new TH1D("labels", "dsd", 30, 0.5, 30.5);



	// Making the canvas we will draw in
	auto c1 = new TCanvas("c1","c1",10,10,1000,600);

	gPad->SetLogy(1);

	// Build a legend - x1,y1,x2,y2 for muon daughters
	TLegend*  leg = new TLegend(.13 ,.74 ,.33 ,.915);
	leg->SetFillColor(0);
	leg->SetNColumns(2);




	for( int k =0; k < 30 ; k++){//running through the vetors for the transverse distance
				h->Fill(k);
			}
	




	//
	// MAKING THE TRANS DISTANCE FIGURE
	//
	// Checking if the histogram was filled for the trans distance
	
		// Histogram attributes
   		h->SetLineColor(1);
    		h->SetLineWidth(1);
    		h->SetLineStyle(1);
		h->GetXaxis()->SetTitle("Isotope");
		h->GetYaxis()->SetTitle("Yield  [x 10^{-7} #mu^{-1} g^{-1} cm^{2}]");
		//h->SetTitleFont(62,"xyz"); // font option
		//h->SetLabelFont(62,"xyz"); // font option
		h->GetYaxis()->SetTitleOffset(0.75);
		h->GetXaxis()->SetTitleOffset(1.4);
		h->GetXaxis()->SetLabelOffset(.005);

		h->GetYaxis()->SetLabelSize(.0465);
		h->GetXaxis()->SetTitleSize(.055);
		h->GetYaxis()->SetTitleSize(.055);
		h->SetStats(kFALSE);


		// Saving the histogram	
		h->Write();


		// normalizing the histogram 
		h->Scale(-1000.);


		// drawing the histograms
		h->SetTitle("");
		h->SetCanExtend(TH1::kAllAxes);
		h->LabelsDeflate("X");
		h->SetAxisRange(0.0005, 50000.,"Y");
		h->SetAxisRange(-2., 32.,"X");
		h->GetXaxis()->SetNdivisions(30);
		h->Draw(" HIST SAME");

	/*string isotopes[51] = {"^{8}He","^{9}He","^{8}Li","^{9}Li","^{10}Li","^{11}Li","^{8}Be","^{9}Be","^{10}Be",
"^{11}Be","^{12}Be","^{13}Be","^{8}B","^{9}B","^{10}B","^{11}B","^{12}B","^{13}B","^{14}B","^{15}B",
"^{9}C","^{10}C","^{11}C","^{12}C","^{13}C","^{14}C","^{15}C","^{16}C","^{11}N","^{12}N","^{13}N",
"^{14}N","^{15}N","^{16}N","^{17}N","^{18}N","^{12}O","^{13}O","^{14}O","^{15}O","^{16}O","^{17}O",
"^{18}O","^{19}O","^{13}F","^{14}F","^{15}F","^{16}F","^{17}F","^{18}F","^{20}Ne"};*/

	string isotopes[59] = {"He8","He9","Li8","Li9","Li10","Li11","Li12","Be8","Be9","Be10","Be11","Be12","Be13","Be14","B8","B9","B10","B11",
	"B12","B13","B14","B15","C8","C9","C10","C11","C12","C13","C14","C15","C16","N10","N11","N12","N13","N14","N15",
	"N16","N17","N18","O11","O12","O13","O14","O15","O16","O17","O18","O19","F13","F14","F15","F16","F17",
	"F18","F19","Ne15","Ne16","Ne20"};

	for(int j = 0; j < 30; j++){
		cout << " Label number " << j << " = " << isotopes[j] << endl;
		h->GetXaxis()->SetBinLabel(j+1, isotopes[j].c_str());
	}

		h->GetXaxis()->SetLabelSize(.0525);
		h->GetXaxis()->LabelsOption 	( 	"v"	);





	// finilizing the figure and saving it
	gPad->SetMargin(0.9, 0.05, 0.15, 0.05); // (left, right, bottom, top)
	gPad->SetTickx(1); 
	gPad->SetTicky(1);







	// The values and the errors on the Y axis
	const int n_points=30;
	double x_vals[n_points]= {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};
	double QGSP_BERT_errs[n_points]={0.02,0,0.06,0.04,0.02,0.01,0.01,0.94,0.16,0.2,0.02,0.03,0.03,0,	
	0.02,0.09,0.35,0.44,0.11,0.09,0.06,0.01,0.01,0.01,0.1,0.31,02.07,0.86,0.77,0.04};
	double QGSP_BERT_vals[n_points] = {0.09,-1.16,1.14,0.35,0.13,0.02,0.01,252.16,7.70,11.19,0.12,0.30,0.34,-1.16,0.13,2.08,
	34.63,55.64,3.21,1.95,0.94,0.01,0.01,0.04,2.58,22.9,1049.63,179.78,143.9,0.46};
	double QGSP_BERT_HP_errs[n_points]={0.02,0,0.06,0.04,0.02,0.01,0.01,0.96,0.17,0.2,0.02,0.03,0.03,0.004,0.02,0.09,0.35,0.45,
	0.12,0.09,0.07,0.01,0.01,0.01,0.1,0.31,02.09,01.08,0.78,0.11};
	double QGSP_BERT_HP_vals[n_points]={0.09,0.00,1.15,0.38,0.1,0.01,0.01,258.66,8.56,11.42,0.09,0.34,0.29,0,0.12,2.24,35.32,57.69,
	04.01,2.35,1.31,0.01,0.01,0.06,2.85,27.24,1225.69,329.71,169.6,3.16};
	double LiBeacom[n_points]={0.23,-1,13,1.9,-1,0.01,-1,-1,38,24,0.81,0.1,-1,-1,5.8,-1,77,174,12,1.9,0.02,-1,-1,0.89,7.6,105,792,225,64,0.82};

	// colors for the curves
	int colors [5] = {1,2,4,6,8};


	gStyle->SetEndErrorSize(5);

	// Instance of the graph
	TGraphErrors graph1(n_points,x_vals,QGSP_BERT_vals,nullptr,QGSP_BERT_errs);
	TGraphErrors graph12(n_points,x_vals,QGSP_BERT_vals,nullptr,QGSP_BERT_errs);
	TGraphErrors graph3(n_points,x_vals,QGSP_BERT_HP_vals,nullptr,QGSP_BERT_HP_errs);
	TGraphErrors graph32(n_points,x_vals,QGSP_BERT_HP_vals,nullptr,QGSP_BERT_HP_errs);
	TGraphErrors graph5(n_points,x_vals,LiBeacom,nullptr,nullptr);
	TGraphErrors graph52(n_points,x_vals,LiBeacom,nullptr,nullptr);
	//graph.SetTitle("Measurement XYZ;lenght [cm];Arb.Units");
	
	// Make the plot estetically better
	graph1.SetMarkerStyle(21);
	graph1.SetMarkerColor(colors[0]);
	graph1.SetLineColorAlpha ( 	colors[0], 0.05) ;
	graph1.SetLineWidth(2);
	graph1.SetFillColor(colors[0]);
	graph1.DrawClone("P Same");
	graph12.SetLineColorAlpha ( 	colors[0], 0.05) ;
	graph12.SetLineWidth(1);
	graph12.DrawClone("L Same");
	graph3.SetMarkerStyle(21);
	graph3.SetMarkerColor(colors[1]);
	graph3.SetLineColorAlpha ( 	colors[1], 0.05) ;
	graph3.SetLineWidth(2);
	graph3.SetFillColor(colors[1]);
	graph3.DrawClone("P Same");
	graph32.SetLineColorAlpha ( 	colors[1], 0.05) ;
	graph32.SetLineWidth(1);
	graph32.DrawClone("L Same");
	graph5.SetMarkerStyle(21);
	graph5.SetMarkerColor(colors[2]);
	graph5.SetLineColorAlpha ( 	colors[2], 0.05) ;
	graph5.SetLineWidth(2);
	graph5.SetFillColor(colors[2]);
	graph5.DrawClone("P Same");
	graph52.SetLineColorAlpha ( 	colors[2], 0.05) ;
	graph52.SetLineWidth(1);
	graph52.DrawClone("L Same");

	// Adding the legend
	leg->AddEntry(&graph1, "HP OFF","lep");
	//leg->AddEntry(&graph2, "FTFP_INCL","lep");
	leg->AddEntry(&graph3, "QGSP_BERT","lep");
	//leg->AddEntry(&graph4, "QGSP_BIC","lep");
	leg->AddEntry(&graph5, "Li&Beacom","lep");

	// Draw a legend 
	leg->DrawClone("Same");


	c1->SaveAs("Yield1no.eps");

	




	return 1;

}
