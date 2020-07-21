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

int edep_spec(){
	// Opening the file with the tree
	TFile* inputFile = new TFile("EdepTotal.root");
	// checking if the file is open correctly
	if (inputFile->IsOpen()) {
    		cout << "Reading file: " << inputFile->GetName() << endl;
  	}
  	else {
    		cerr << "Error opening input file!" << endl;
    		return 0;
  	}



	// create container for colors of the particles histograms
	int colors[10] = {8,4,1,6,41,2,4,6,8,2};
	


	//Creating the histograms for isodistance
	TH1* h[6];
	inputFile->GetObject("alldata2FTFP_BERT_HP",h[0]);
	inputFile->GetObject("alldata2FTFP_INCLXX_HP",h[1]);
	inputFile->GetObject("alldata2noHP",h[2]);
	inputFile->GetObject("alldata2QGSP_BERT_HP",h[3]);
	inputFile->GetObject("alldata2QGSP_BIC_HP",h[4]);
	inputFile->GetObject("alldata2QGSP_INCLXX_HP",h[5]);


	// Making the canvas we will draw in
	auto c1 = new TCanvas("c1","c1");
   	c1->SetCanvasSize(750, 750);
   	c1->SetWindowSize(600, 600);


	gPad->SetMargin(0.20, 0.05, 0.20, 0.05); // (left, right, bottom, top)
	gPad->SetTickx(1); 
	gPad->SetTicky(1);


	// Build a legend - x1,y1,x2,y2 for muon daughters
	TLegend* leg = new TLegend(.525 ,.625 ,.925 ,.925);
	leg->SetFillColor(0);
	leg->SetNColumns(2);




	for(int j = 0; j < 6; j ++){
		cout << "Number of entries in the trans dist hist  : " << h[j]->GetEntries() << endl;

	h[j]->SetLineColor(colors[j]);
    	h[j]->SetLineWidth(4);
    	h[j]->SetLineStyle(1);
	h[j]->GetXaxis()->SetTitle("Muon energy loss [GeV]");
	h[j]->GetYaxis()->SetTitle("Normalized probability");
	h[j]->GetYaxis()->SetTitleOffset(1.6);
	h[j]->GetXaxis()->SetTitleOffset(1.25);
	h[j]->GetXaxis()->SetLabelSize(.05);
	h[j]->GetYaxis()->SetLabelSize(.05);
	h[j]->GetXaxis()->SetTitleSize(.055);
	h[j]->GetYaxis()->SetTitleSize(.055);
	h[j]->GetXaxis()->SetNdivisions( 5,true);
	h[j]->GetYaxis()->SetNdivisions( 6,true);
	h[j]->GetXaxis()->SetTickLength(0.02);
	h[j]->GetYaxis()->SetTickLength(0.02);
	h[j]->SetAxisRange(0., 25.,"X");
	//h[j]->SetAxisRange(0., 0.2,"Y");
	h[j]->SetStats(kFALSE);


	// drawing the histograms
	h[j]->SetTitle("");
	h[j]->DrawNormalized("HIST 9 SAME");

		
}


		// Adding the legend
		leg->AddEntry(h[0], "FTFP_BERT","E");
		leg->AddEntry(h[1], "FTFP_INCLXX","E");
		leg->AddEntry(h[2], "HP OFF","E");
		leg->AddEntry(h[3], "QGSP_BERT","E");
		leg->AddEntry(h[4], "QGSP_BIC","E");
		leg->AddEntry(h[5], "QGSP_INCLXX","E");
 




	// Draw a legend 
	leg->DrawClone("Same");


	// finilizing the figure and saving it
	c1->SaveAs("MuonEnergyLostSpec.eps");


	return 1;

}
