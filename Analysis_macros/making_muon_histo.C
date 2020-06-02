// Read the tree and make muon histos plot

#include "TCanvas.h"
#include "TROOT.h"
#include "TH1.h"
#include "TTree.h"
#include "TFile.h"
#include "TLegend.h"

int making_muon_histo(){
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
	inputFile->GetObject("muon_data",DataTree);
	int nentries = (int)DataTree->GetEntries();
	cout << "Number of entries in the Tree: " << nentries << endl;
	if( nentries < 1 ){
		cerr << "No data to analyze." << endl;
		return 0;
	}

	// creating variables for the branches
	double Energy, Energy1, x, y;
	int EventID, EventID1, nmuclose; //nmuclose number of muons less than 30 cm away from straight line path
	// Attaching to branches
	DataTree->SetBranchAddress("Energy",&Energy);
	DataTree->SetBranchAddress("x",&x);
	DataTree->SetBranchAddress("y",&y);
	DataTree->SetBranchAddress("EventID",&EventID);

	
	// Getting the number of muons simulated
	DataTree->GetEntry( nentries - 1 );
	int nmuons = EventID;
	cout << "Number of muons simulated: " << nmuons << endl;

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
	// Building log binning for h1l
	const Int_t nbins = 500;
   	//Double_t xmin = 1e-1;
   	//Double_t xmax = 1e4;
   	//Double_t logxmin = TMath::Log10(xmin);
   	//Double_t logxmax = TMath::Log10(xmax);
   	//Double_t binwidth = (logxmax-logxmin)/nbins;
   	//Double_t xbins[nbins+1];
   	//xbins[0] = xmin;
   	//for (Int_t i=1;i<=nbins;i++) {
      	//	xbins[i] = xmin + TMath::Power(10,logxmin+i*binwidth);
   	//}
   	//TH1F *h1l = new TH1F("hspecl","Muon spectrum with log bins",nbins,xbins);

	// Making the canvas we will draw in
	TCanvas *c1 = new TCanvas();

	// Running through the tree and filling the histogram
	for( int i =0; i < nentries - 1; i = i + 2){
		DataTree->GetEntry(i);
		Energy1 = Energy;
		EventID1 = EventID;
		DataTree->GetEntry(i + 1);
		if ( EventID1 == EventID){
			h1->Fill(Energy1/1000, Energy1/1000); // fill with the energy normalized by the energy - divide per 1000 to convert to GeV
			//h1l->Fill(Energy1/1000, Energy1/1000); // fill with the energy normalized by the energy - divide per 1000 to convert to GeV
			h2->Fill((Energy1 - Energy)/1000); // divide per 1000 to convert to GeV
			if ( x*x + y*y < 90000 ){ nmuclose ++;} //if displaced less than 30 cm
		}		
	}
				
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
	//h1l->SetLineColor(4);
    	//h1l->SetLineWidth(2);
    	//h1l->SetLineStyle(1);
	//h1l->GetXaxis()->SetTitle("Energy [GeV]");
	//h1l->GetYaxis()->SetTitle("Energy x flux per energy [cm^{-2} s^{-1} ]");
	//h1l->GetYaxis()->SetTitleOffset(1.);
	//h1l->GetXaxis()->SetLabelSize(.035);
	//h1l->GetYaxis()->SetLabelSize(.04);
	//h1l->GetXaxis()->SetTitleSize(.045);
	//h1l->GetYaxis()->SetTitleSize(.045);
	//h1l->SetStats(kFALSE);
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

	//c1->Clear();
	//h1l->SetTitle("");
	//h1l->DrawClone("HIST C 9");
	// finilizing the figure and saving it
	//c1->SaveAs("MuonSpectraLogbin.png");

	cout << "Percentage of muons with less than 30 cm displacement : " << nmuclose/(1. * nmuons) << endl;


	// Closing hist file
	inputFile->Close();
	//outputFile->Close();

	return 1;

}
