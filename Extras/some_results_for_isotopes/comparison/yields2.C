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

int yields2(){




	TH1* h = new TH1D("labels", "dsd", 29, 0.5, 29.5);



	// Making the canvas we will draw in
	auto c1 = new TCanvas("c1","c1",10,10,1000,600);

	gPad->SetLogy(1);

	// Build a legend - x1,y1,x2,y2 for muon daughters
	TLegend*  leg = new TLegend(.7 ,.725 ,.9 ,.9);
	leg->SetFillColor(0);
	leg->SetNColumns(2);




	for( int k =0; k < 29 ; k++){//running through the vetors for the transverse distance
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
		h->SetAxisRange(-2., 31.,"X");
		h->GetXaxis()->SetNdivisions(63);
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

	for(int j = 0; j < 29; j++){
		cout << " Label number " << j << " = " << isotopes[j] << endl;
		h->GetXaxis()->SetBinLabel(j+1, isotopes[j+30].c_str());
	}

		h->GetXaxis()->SetLabelSize(.0525);
		h->GetXaxis()->LabelsOption 	( 	"v"	);





	// finilizing the figure and saving it
	gPad->SetMargin(0.9, 0.05, 0.15, 0.05); // (left, right, bottom, top)
	gPad->SetTickx(1); 
	gPad->SetTicky(1);







	// The values and the errors on the Y axis
	const int n_points=29;
	double x_vals[n_points]= {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29};
	double FTFP_BERT_HP_vals[n_points]={0.06,0,0.10,1.48,50.67,522.34,873.49,
	25.78,0.35,0.05,0.01,0.01,0.76,22.20,369.23,38383.63,16.73,74.15,0.08,0.01,0.03,0.15,0.43,0.33,
	0.05,0,0,0,0.15};
	double FTFP_BERT_HP_errs[n_points]={0.01,0.003,0.02,0.07,0.42,1.34,1.73,
	0.30,0.03,0.01,0.01,0.01,0.05,0.28,1.13,11.49,0.24,0.5,0.02,0.01,0.01,0.02,0.04,0.03,
	0.01,0.003,0.003,0,0.02};
	double FTFP_INCL_HP_errs[n_points]={0.01,0.02,0.04,0.13,0.31,1.18,1.64,
	0.25,0.03,0.01,0,0.01,0.06,0.27,0.98,9.82,0.21,0.43,0.01,0.003,0.01,0.04,0.05,0.05,
	0.02,0.01,0.003,0.01,0.02};
	double FTFP_INCL_HP_vals[n_points]={0.05,0.07,0.41,4.67,27.49,401.62,782.06,
	18.44,0.34,0.03,0,0.02,01.09,20.45,278.52,28069.47,12.32,54.1,0.05,0,0.01,0.52,0.83,0.65,
	0.09,0.01,0,0.01,0.15};
	double QGSP_BERT_HP_errs[n_points]={0.01,0.004,0.02,0.07,0.42,1.34,1.74,
	0.3,0.04,0.01,0.01,0,0.05,0.28,1.13,11.48,0.24,0.5,0.01,0.004,0.01,0.02,0.04,0.03,
	0.02,0.01,0.004,0.004,0.02};
	double QGSP_BERT_HP_vals[n_points]={0.06,0,0.07,1.57,48.45,506.13,854.43,
	25.28,0.36,0.03,0.01,0,0.69,21.58,356.87,37037.9,16.23,71.5,0.05,0,0.03,0.15,0.41,0.3,
	0.08,0.01,0,0,0.13};
	double QGSP_BIC_HP_errs[n_points]={0.02,0.01,0.03,0.12,0.35,1.46,2.03,
	0.31,0.04,0.02,0.003,0.02,0.07,0.31,01.06,10.56,0.23,0.46,0.01,0.01,0.01,0.03,0.05,0.05,
	0.01,0.02,0.01,0.01,0.02};
	double QGSP_BIC_HP_vals[n_points]={0.07,0.05,0.34,4.24,34.98,607.86,1177.37,
	27.52,0.52,0.07,0,0.15,1.56,28.03,324.29,31995.14,15.27,61.51,0.05,0.01,0.02,0.24,0.78,0.65,
	0.06,0.15,0.01,0.01,0.16};
	double QGSP_INCLXX_HP_errs[n_points]={0.01,0.003,0.01,0.07,0.35,1.26,1.66,
	0.28,0.04,0.01,0,0.01,0.04,0.21,01.03,10.23,0.21,0.45,0.01,0,0.01,0.01,0.02,0.03,
	0.01,0,0.003,0,0.01};
	double QGSP_INCLXX_HP_vals[n_points]={0.05,0,0.03,1.26,36.64,459.74,805.00,
	23.59,0.37,0.06,0,0.01,0.45,13.09,308.97,30458.11,13.25,58.13,0.03,0,0.01,0.01,0.12,0.3,
	0.06,0.00,0,0,0.01};

	// colors for the curves
	int colors [5] = {12,2,4,6,8};


	gStyle->SetEndErrorSize(5);

	// Instance of the graph
	TGraphErrors graph1(n_points,x_vals,FTFP_BERT_HP_vals,nullptr,FTFP_BERT_HP_errs);
	TGraphErrors graph12(n_points,x_vals,FTFP_BERT_HP_vals,nullptr,FTFP_BERT_HP_errs);
	TGraphErrors graph2(n_points,x_vals,FTFP_INCL_HP_vals,nullptr,FTFP_INCL_HP_errs);
	TGraphErrors graph22(n_points,x_vals,FTFP_INCL_HP_vals,nullptr,FTFP_INCL_HP_errs);
	TGraphErrors graph3(n_points,x_vals,QGSP_BERT_HP_vals,nullptr,QGSP_BERT_HP_errs);
	TGraphErrors graph32(n_points,x_vals,QGSP_BERT_HP_vals,nullptr,QGSP_BERT_HP_errs);
	TGraphErrors graph4(n_points,x_vals,QGSP_BIC_HP_vals,nullptr,QGSP_BIC_HP_errs);
	TGraphErrors graph42(n_points,x_vals,QGSP_BIC_HP_vals,nullptr,QGSP_BIC_HP_errs);
	TGraphErrors graph5(n_points,x_vals,QGSP_INCLXX_HP_vals,nullptr,QGSP_INCLXX_HP_errs);
	TGraphErrors graph52(n_points,x_vals,QGSP_INCLXX_HP_vals,nullptr,QGSP_INCLXX_HP_errs);
	//graph.SetTitle("Measurement XYZ;lenght [cm];Arb.Units");
	
	// Make the plot estetically better
	graph1.SetMarkerStyle(21);
	graph1.SetMarkerColor(colors[0]);
	graph1.SetLineColorAlpha ( 	colors[0], 0.05) ;
	graph1.SetLineWidth(2);
	graph1.SetFillColor(colors[0]);
	graph1.DrawClone("P Same");
	graph12.SetLineColorAlpha ( 	12, 0.05) ;
	graph12.SetLineWidth(1);
	graph12.DrawClone("L Same");
	graph2.SetMarkerStyle(21);
	graph2.SetMarkerColor(colors[1]);
	graph2.SetLineColorAlpha ( 	colors[1], 0.05) ;
	graph2.SetLineWidth(2);
	graph2.SetFillColor(colors[1]);
	graph2.DrawClone("P Same");
	graph22.SetLineColorAlpha ( 	colors[1], 0.05) ;
	graph22.SetLineWidth(1);
	graph22.DrawClone("L Same");
	graph3.SetMarkerStyle(21);
	graph3.SetMarkerColor(colors[2]);
	graph3.SetLineColorAlpha ( 	colors[2], 0.05) ;
	graph3.SetLineWidth(2);
	graph3.SetFillColor(colors[2]);
	graph3.DrawClone("P Same");
	graph32.SetLineColorAlpha ( 	colors[2], 0.05) ;
	graph32.SetLineWidth(1);
	graph32.DrawClone("L Same");
	graph4.SetMarkerStyle(21);
	graph4.SetMarkerColor(colors[3]);
	graph4.SetLineColorAlpha ( 	colors[3], 0.05) ;
	graph4.SetLineWidth(2);
	graph4.SetFillColor(colors[3]);
	graph4.DrawClone("P Same");
	graph42.SetLineColorAlpha ( 	colors[3], 0.05) ;
	graph42.SetLineWidth(1);
	graph42.DrawClone("L Same");
	graph5.SetMarkerStyle(21);
	graph5.SetMarkerColor(colors[4]);
	graph5.SetLineColorAlpha ( 	colors[4], 0.05) ;
	graph5.SetLineWidth(2);
	graph5.SetFillColor(colors[4]);
	graph5.DrawClone("P Same");
	graph52.SetLineColorAlpha ( 	colors[4], 0.05) ;
	graph52.SetLineWidth(1);
	graph52.DrawClone("L Same");

	// Adding the legend
	leg->AddEntry(&graph1, "FTFP_BERT","lep");
	leg->AddEntry(&graph2, "FTFP_INCL","lep");
	leg->AddEntry(&graph3, "QGSP_BERT","lep");
	leg->AddEntry(&graph4, "QGSP_BIC","lep");
	leg->AddEntry(&graph5, "QGSP_INCL","lep");

	// Draw a legend 
	leg->DrawClone("Same");


	c1->SaveAs("Yield2.eps");

	




	return 1;

}
