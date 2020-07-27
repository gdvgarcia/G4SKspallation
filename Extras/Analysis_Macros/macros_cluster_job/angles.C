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

int angles(){


	// Opening the file with the tree
	TFile* inputFile = new TFile("angles.root");
	// checking if the file is open correctly
	if (inputFile->IsOpen()) {
    		cout << "Reading file: " << inputFile->GetName() << endl;
  	}
  	else {
    		cerr << "Error opening input file!" << endl;
    		return 0;
  	}

	// Getting the tree
	TTree* DataTree;
	inputFile->GetObject("angles",DataTree);
	int nentries = (int)DataTree->GetEntries();
	cout << "Number of entries in the Tree: " << nentries << endl; // remember that the try has one entry per event so nentries = n events
	if( nentries < 1 ){
		cerr << "No data to analyze." << endl;
		return 0;
	}

	int n30 = 0;


	// creating variables for the branches of the trans distance
	double Theta;
	DataTree->SetBranchAddress("Theta",&Theta);


	TH1* h = new TH1D("theta", "theta", 90, -1.6, 1.6);



	// Running through the tree and filling the histograms
	for( int i =0; i < nentries; ++i){
		// getting data from the try for the next event
		DataTree->GetEntry(i);

		h->Fill(Theta);

		if( Theta < 0.785398){ n30++;}
		
		cout << "Event " << i << " has been processed. With a theta of = " << Theta << endl;
 	}


		cout << "Less than 30 degrees is  " << (1.*n30)/(1.*nentries) *100 << " % " << endl;




	// Making the canvas we will draw in
	auto c1 = new TCanvas("c1","c1",10,10,750,750);

	gPad->SetMargin(0.20, 0.05, 0.20, 0.05); // (left, right, bottom, top)
	gPad->SetTickx(1); 
	gPad->SetTicky(1);


	//
	// MAKING THE TRANS DISTANCE FIGURE
	//
	// Checking if the histogram was filled for the trans distance
	
	// Histogram attributes
   	h->SetLineColor(2);
    	h->SetLineWidth(4);
    	h->SetLineStyle(1);
	h->GetXaxis()->SetTitle("Angle [rad]");
	h->GetYaxis()->SetTitle("Normalized probability");	
	h->GetYaxis()->SetTitleOffset(0.75);
	h->GetXaxis()->SetTitleOffset(1.4);
	h->GetXaxis()->SetLabelOffset(.005);
	h->GetYaxis()->SetTitleOffset(1.6);
	h->GetXaxis()->SetTitleOffset(1.25);
	h->GetYaxis()->SetNdivisions( 6,true);
	h->GetXaxis()->SetLabelSize(.05);
	h->GetYaxis()->SetLabelSize(.05);
	h->GetXaxis()->SetTitleSize(.055);
	h->GetYaxis()->SetTitleSize(.055);
	h->GetXaxis()->SetTickLength(0.02);
	h->GetYaxis()->SetTickLength(0.02);
	//h2->SetAxisRange(0., 0.1,"Y");
	//h2->SetAxisRange(0., 1.57079632679,"X");
	//h->SetAxisRange(0., 0.1,"Y");
	//h->SetAxisRange(0., 1.57079632679,"X");
	h->SetStats(kFALSE);


	// drawing the histograms
	h->SetTitle("");


	h->GetXaxis()->SetNdivisions( 6,true);
	//h->SetAxisRange(0., 0.05,"Y");
	h->SetAxisRange(-0.01, 1.6,"X");


	h->DrawNormalized(" HIST C9");


	

	c1->SaveAs("AngularDistribution.eps");

	




	return 1;

}
