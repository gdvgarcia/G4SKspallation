// Read the tree and make muon histos plot

#include "TCanvas.h"
#include "TROOT.h"
#include "TH1.h"
#include "TTree.h"
#include "TFile.h"
#include "TLegend.h"

int muon_histo(){
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
	inputFile->GetObject("event_data",DataTree);
	int nentries = (int)DataTree->GetEntries();
	cout << "Number of entries in the Tree: " << nentries << endl;
	if( nentries < 1 ){
		cerr << "No data to analyze." << endl;
		return 0;
	}

	// creating variables for the branches
	double Energy, Edep;//, xIni, yIni, zIni, xFin, yFin, zFin;
	//int nmuclose; //nmuclose number of muons less than 30 cm away from straight line path - this was only useful for testing downward muons
	// Attaching to branches
	DataTree->SetBranchAddress("Energy",&Energy);
	//DataTree->SetBranchAddress("xIni",&xIni);
	//DataTree->SetBranchAddress("yIni",&yIni);
	//DataTree->SetBranchAddress("zIni",&zIni);
	//DataTree->SetBranchAddress("xFin",&xFin);
	//DataTree->SetBranchAddress("yFin",&yFin);
	//DataTree->SetBranchAddress("zFin",&zFin);
	DataTree->SetBranchAddress("Edep",&Edep);

	
	// Getting the number of muons simulated
	cout << "Number of muons simulated: " << nentries << endl;

	// Create a new file
	string filename = "MuonHists.root";
	TFile *outputFile = new TFile(filename.c_str(),"RECREATE");
  	if (outputFile->IsOpen()) {
    		cout << "Everything is fine!" << endl;
  	}
  	else {
    		cerr << "That should not happen." << endl;
		return 0;
  	}
	
	//Declaring the histograms
	TH1* h1 = new TH1D("hspec", "Muon spectrum", 1000, 0.1, 10000);
	TH1* h2 = new TH1D("hdep", "Muon energy loss", 60, 5, 20);

	// Making the canvas we will draw in
	TCanvas *c1 = new TCanvas();

	// Running through the tree and filling the histogram
	for( int i =0; i < nentries; i ++){
		DataTree->GetEntry(i);
		h1->Fill(Energy/1000., Energy/1000.); // fill with the energy normalized by the energy - divide per 1000 to convert to GeV
		h2->Fill((Edep)/1000.); // divide per 1000 to convert to GeV
		//if ( x*x + y*y < 90000 ){ nmuclose ++;} //if displaced less than 30 cm	
	}

	// Checking if the histogram was filled
	cout << "Number of entries in the h1gram : " << h1->GetEntries() << endl;

	// Checking if the histogram was filled
	cout << "Number of entries in the h1gram : " << h2->GetEntries() << endl;
				
	// Histograms attributes
   	h1->SetLineColor(4);
    	h1->SetLineWidth(2);
    	h1->SetLineStyle(1);
	h1->GetXaxis()->SetTitle("Energy [GeV]");
	h1->GetYaxis()->SetTitle("Energy x flux per energy [cm^{-2} s^{-1} ]");
	h1->GetYaxis()->SetTitleOffset(1.);
	h1->GetXaxis()->SetLabelSize(.035);
	h1->GetYaxis()->SetLabelSize(.04);
	h1->GetXaxis()->SetTitleSize(.045);
	h1->GetYaxis()->SetTitleSize(.045);
	h1->SetStats(kFALSE);
	
	h2->SetLineColor(4);
    	h2->SetLineWidth(2);
    	h2->SetLineStyle(1);
	h2->GetXaxis()->SetTitle("Muon energy loss [GeV]");
	h2->GetYaxis()->SetTitle("Normalized probability");
	h2->GetYaxis()->SetTitleOffset(1.);
	h2->GetXaxis()->SetLabelSize(.04);
	h2->GetYaxis()->SetLabelSize(.045);
	h2->GetXaxis()->SetTitleSize(.045);
	h2->GetYaxis()->SetTitleSize(.045);
	h2->SetStats(kFALSE);

	// Saving histograms	
	h1->Write();		
	//h1l->Write();
	h2->Write();		

	// drawing the histograms
	h2->SetTitle("");
	h2->DrawNormalized("HIST 9");
	// finilizing the figure and saving it
	c1->SaveAs("MuonEnergyLost.png");

	c1->Clear();
	h1->SetTitle("");
	h1->DrawNormalized("HIST C 9");
	// finilizing the figure and saving it
	gPad->SetLogx();
	gPad->SetLogy();
	c1->SaveAs("MuonSpectra.png");


	//cout << "Percentage of muons with less than 30 cm displacement : " << nmuclose/(1. * nentries) << endl;


	// Closing hist file
	inputFile->Close();
	//outputFile->Close();

	return 1;

}
