// Read the tree and make transverse distance histos plot

#include "TCanvas.h"
#include "TROOT.h"
#include "TH1.h"
#include "TTree.h"
#include "TFile.h"
#include "TLegend.h"

int making_isodistance_histo(){
	// Opening a file
	TFile* inputFile = new TFile("SK.root");
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
	inputFile->GetObject("isotopes_data",DataTree);
	int nentries = (int)DataTree->GetEntries();
	cout << "Number of entries in the Tree: " << nentries << endl;
	if( nentries < 1 ){
		cerr << "No data to analyze." << endl;
		return 0;
	}

	// creating variables for the branches
	double z;
	int A, Z;
	// Attaching to branches
	DataTree->SetBranchAddress("A",&A);
	DataTree->SetBranchAddress("Z",&Z);
	DataTree->SetBranchAddress("z",&z);


	// Create a new file
	string filename = "TransverseDistanceIsotopes.root";
	TFile *outputFile = new TFile(filename.c_str(),"RECREATE");
  	if (outputFile->IsOpen()) {
    		cout << "Everything is fine!" << endl;
  	}
  	else {
    		cerr << "That should not happen." << endl;
		return 0;
  	}
	
	//Creating the histograms
	TH1* h1 = new TH1D("TransverseDistanceIsotopes", "Transverse Distance for all the isotopes", 300, 0.0, 300.0);
	TH1* h2 = new TH1D("TransverseDistanceNitrogen", "Transverse Distance for N16", 300, 0.0, 300.0);
	TH1* h3 = new TH1D("TransverseDistanceBoron", "Transverse Distance for B8", 300, 0.0, 300.0);

	// Making the canvas we will draw in
	TCanvas *c1 = new TCanvas();

	// Running through the tree and filling the histogram
	for( int i =0; i < nentries; ++i){
		DataTree->GetEntry(i);
		double distance = z / 10; // to convert mm to cm
		h1->Fill(distance);
		if(A == 16 && Z == 7){h2->Fill(distance);}//N16 - Z = 7 
		if(A == 8 && Z == 5){h3->Fill(distance);}//B8 - Z = 5
 	}	
	
	// Checking if the histograms were filled
	cout << "Number of entries in the h1gram : " << h1->GetEntries() << endl;
	cout << "Number of entries in the h1gram : " << h2->GetEntries() << endl;
	cout << "Number of entries in the h1gram : " << h3->GetEntries() << endl;

	// Build a legend - x1,y1,x2,y2 
	TLegend leg(.7 ,.15 ,.85 ,.35);
	leg.SetFillColor(0);

	// drawing the comulative distribution 1
	TH1* hc = h1->GetCumulative();
	hc->SetTitle("");
	hc->SetLineColor(4);
    	hc->SetLineWidth(2);
	hc->GetXaxis()->SetTitle("Isotope distance to muon track  [cm]");
	hc->GetYaxis()->SetTitle("Probability");
	hc->GetYaxis()->SetTitleOffset(1.1);
	hc->GetXaxis()->SetLabelSize(.04);
	hc->GetYaxis()->SetLabelSize(.045);
	hc->GetXaxis()->SetTitleSize(.045);
	hc->GetYaxis()->SetTitleSize(.045);
	hc->SetStats(kFALSE);
	hc->Scale(1/h1->GetEntries()); //normalizing
	hc->Draw("HIST C 9");
	// Saving the histogram	
	hc->Write();
	// Adding the legend
	leg.AddEntry(hc,"   total","l");

	// drawing the comulative distribution 3
	hc = h3->GetCumulative();
	hc->SetTitle("");
	hc->SetLineColor(2);
    	hc->SetLineWidth(2);
    	hc->SetLineStyle(2);
	hc->SetStats(kFALSE);
	hc->Scale(1/h3->GetEntries()); //normalizing
	hc->Draw("HIST C 9 SAME");
	// Saving the histogram	
	hc->Write();
	// Adding the legend
	leg.AddEntry(hc,"   ^{8}B","l");

	// drawing the comulative distribution 2
	hc = h2->GetCumulative();
	hc->SetTitle("");
	hc->SetLineColor(8);
    	hc->SetLineWidth(2);
    	hc->SetLineStyle(9);
	hc->SetStats(kFALSE);
	hc->Scale(1/h2->GetEntries()); //normalizing
	hc->Draw("HIST C 9 SAME");
	// Saving the histogram	
	hc->Write();
	// Adding the legend
	leg.AddEntry(hc,"  ^{16}N","l");


	// Draw a legend 
	leg.DrawClone("Same");


	// finilizing the figure and saving it
	c1->SaveAs("TrasverseDistanceIsotopes.png");

	// Closing hist file
	inputFile->Close();
	//outputFile->Close();

	return 1;

}
