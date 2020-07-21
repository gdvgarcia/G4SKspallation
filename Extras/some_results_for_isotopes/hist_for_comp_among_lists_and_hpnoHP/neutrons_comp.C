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

int neutrons_comp(){
	// Opening the file with the tree
	TFile* inputFile = new TFile("TransNeutrons.root");
	// checking if the file is open correctly
	if (inputFile->IsOpen()) {
    		cout << "Reading file: " << inputFile->GetName() << endl;
  	}
  	else {
    		cerr << "Error opening input file!" << endl;
    		return 0;
  	}



	// create container for colors of the particles histograms
	int colors[10] = {8,4,2,13,7,6,4,6,8,2};
	


	// Create a new file where we gonna save the final histograms
	string filename = "histos.root";
	TFile *outputFile = new TFile(filename.c_str(),"RECREATE");
  	if (outputFile->IsOpen()) {
    		cout << "Everything is fine!" << endl;
  	}
  	else {
    		cerr << "That should not happen." << endl;
		return 0;
  	}


	//Creating the histograms for isodistance
	TH1* hHP;
	inputFile->GetObject("alldata2",hHP);
	TH1* hNO;
	inputFile->GetObject("alldata2noHP",hNO);


	// Making the canvas we will draw in
	auto c1 = new TCanvas("c1","c1");
   	c1->SetCanvasSize(750, 750);
   	c1->SetWindowSize(600, 600);

	gPad->SetLogy(1);
	gPad->SetMargin(0.20, 0.05, 0.20, 0.05); // (left, right, bottom, top)
	gPad->SetTickx(1); 
	gPad->SetTicky(1);


	// Build a legend - x1,y1,x2,y2 for muon daughters
	TLegend* leg = new TLegend(.7 ,.725 ,.9 ,.9);
	leg->SetFillColor(0);
	leg->SetNColumns(1);






	//
	// MAKING THE TRANS DISTANCE FIGURE
	//
	// Checking if the histogram was filled for the trans distance for HP
		cout << "Number of entries in the trans dist hist  : " << hHP->GetEntries() << endl;

			
		// Histogram attributes
   		hHP->SetLineColor(2);
    		hHP->SetLineWidth(3);
    		hHP->SetLineStyle(1);
		hHP->GetXaxis()->SetTitle("Distance to muon track [cm]");
		hHP->GetYaxis()->SetTitle("Normalized probability");
		//h[0][j]->SetTitleFont(62,"xyz"); // font option
		//h[0][j]->SetLabelFont(62,"xyz"); // font option
		hHP->GetYaxis()->SetTitleOffset(1.6);
		hHP->GetXaxis()->SetTitleOffset(1.25);
		hHP->GetXaxis()->SetLabelSize(.05);
		hHP->GetYaxis()->SetLabelSize(.05);
		hHP->GetXaxis()->SetTitleSize(.055);
		hHP->GetYaxis()->SetTitleSize(.055);
		hHP->GetXaxis()->SetTickLength(0.02);
		hHP->GetYaxis()->SetTickLength(0.02);
		hHP->GetXaxis()->SetNdivisions( 10,true);
		hHP->SetStats(kFALSE);


		// Saving the histogram	
		hHP->Write();


		// normalizing the histogram
		Double_t norm = hHP->GetEntries(); 
		hHP->Scale(1/norm);


		// drawing the histograms
		hHP->SetTitle("");
		hHP->Draw(" HIST SAME 9");
		hHP->SetAxisRange(0.000001, 0.05,"Y");
		hHP->SetAxisRange(0., 1000,"X");


		// Adding the legend
		leg->AddEntry(hHP, "HP ON","E");

// Checking if the histogram was filled for the trans distance for no HP
		cout << "Number of entries in the trans dist hist  : " << hNO->GetEntries() << endl;

			
		// Histogram attributes
   		hNO->SetLineColor(1);
    		hNO->SetLineWidth(3);
    		hNO->SetLineStyle(1);
		hNO->GetXaxis()->SetTitle("Distance to muon track [cm]");
		hNO->GetYaxis()->SetTitle("Normalized probability");
		//h[0][j]->SetTitleFont(62,"xyz"); // font option
		//h[0][j]->SetLabelFont(62,"xyz"); // font option
		hNO->GetYaxis()->SetTitleOffset(1.6);
		hNO->GetXaxis()->SetTitleOffset(1.25);
		hNO->GetXaxis()->SetLabelSize(.05);
		hNO->GetYaxis()->SetLabelSize(.05);
		hNO->GetXaxis()->SetTitleSize(.055);
		hNO->GetYaxis()->SetTitleSize(.055);
		hNO->GetXaxis()->SetTickLength(0.02);
		hNO->GetYaxis()->SetTickLength(0.02);
		hNO->GetXaxis()->SetNdivisions( 10,true);
		hNO->SetStats(kFALSE);


		// Saving the histogram	
		hNO->Write();


		// normalizing the histogram
	 	norm = hNO->GetEntries(); 
		hNO->Scale(1/norm);


		// drawing the histograms
		hNO->SetTitle("");
		hNO->Draw(" HIST SAME 9");
		hNO->SetAxisRange(0.000001, 0.05,"Y");
		hNO->SetAxisRange(0., 1000,"X");


		// Adding the legend
		leg->AddEntry(hNO, "HP OFF","E");
 	




	// Draw a legend 
	leg->DrawClone("Same");


	// finilizing the figure and saving it
	c1->SaveAs("TrasverseDistanceNeutrons.eps");


	return 1;

}
