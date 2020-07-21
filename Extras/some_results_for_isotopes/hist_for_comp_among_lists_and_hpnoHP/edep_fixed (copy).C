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

int edep_fixed(){
	// Opening the file with the tree
	TFile* inputFile = new TFile("EdepTotalBIGGESTSIMU.root");
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
	


	//Creating the histograms for isodistance
	TH1* hHP;
	inputFile->GetObject("alldata2",hHP);
	TH1* hNO;
	inputFile->GetObject("alldata2Simple",hNO);


	// Making the canvas we will draw in
	auto c1 = new TCanvas("c1","c1");
   	c1->SetCanvasSize(750, 750);
   	c1->SetWindowSize(600, 600);


	gPad->SetMargin(0.20, 0.05, 0.20, 0.05); // (left, right, bottom, top)
	gPad->SetTickx(1); 
	gPad->SetTicky(1);


	// Build a legend - x1,y1,x2,y2 for muon daughters
	TLegend* leg = new TLegend(.7 ,.725 ,.9 ,.9);
	leg->SetFillColor(0);
	leg->SetNColumns(1);






	hHP->SetLineColor(2);
    	hHP->SetLineWidth(4);
    	hHP->SetLineStyle(1);
	hHP->GetXaxis()->SetTitle("Muon energy loss [GeV]");
	hHP->GetYaxis()->SetTitle("Normalized probability");
	hHP->GetYaxis()->SetTitleOffset(1.6);
	hHP->GetXaxis()->SetTitleOffset(1.25);
	hHP->GetXaxis()->SetLabelSize(.05);
	hHP->GetYaxis()->SetLabelSize(.05);
	hHP->GetXaxis()->SetTitleSize(.055);
	hHP->GetYaxis()->SetTitleSize(.055);
	hHP->GetXaxis()->SetNdivisions( 5,true);
	hHP->GetYaxis()->SetNdivisions( 6,true);
	hHP->GetXaxis()->SetTickLength(0.02);
	hHP->GetYaxis()->SetTickLength(0.02);
	hHP->SetAxisRange(5., 20.,"X");
	//hHP->SetAxisRange(0., 0.2,"Y");
	hHP->SetStats(kFALSE);


	// drawing the histograms
	hHP->SetTitle("");
	hHP->DrawNormalized("HIST 9");

		// Adding the legend
		leg->AddEntry(hHP, "SK Spectrum","E");

// Checking if the histogram was filled for the trans distance for no HP
		cout << "Number of entries in the trans dist hist  : " << hNO->GetEntries() << endl;

			
	hNO->SetLineColor(1);
    	hNO->SetLineWidth(4);
    	hNO->SetLineStyle(1);
	hNO->GetXaxis()->SetTitle("Muon energy loss [GeV]");
	hNO->GetYaxis()->SetTitle("Normalized probability");
	hNO->GetYaxis()->SetTitleOffset(1.6);
	hNO->GetXaxis()->SetTitleOffset(1.25);
	hNO->GetXaxis()->SetLabelSize(.05);
	hNO->GetYaxis()->SetLabelSize(.05);
	hNO->GetXaxis()->SetTitleSize(.055);
	hNO->GetYaxis()->SetTitleSize(.055);
	hNO->GetXaxis()->SetNdivisions( 5,true);
	hNO->GetYaxis()->SetNdivisions( 6,true);
	hNO->GetXaxis()->SetTickLength(0.02);
	hNO->GetYaxis()->SetTickLength(0.02);
	hNO->SetAxisRange(5., 20.,"X");
	//hNO->SetAxisRange(0., 0.2,"Y");
	hNO->SetStats(kFALSE);


	// drawing the histograms
	hNO->SetTitle("");
		hNO->DrawNormalized(" HIST SAME 9");


		// Adding the legend
		leg->AddEntry(hNO, "Fixed Energy","E");
 	




	// Draw a legend 
	leg->DrawClone("Same");


	// finilizing the figure and saving it
	c1->SaveAs("MuonEnergyLostFixed.eps");


	return 1;

}
