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

int bin_number(){
	// Opening the file with the tree
	TFile* inputFile = new TFile("alldata2.root");
	// checking if the file is open correctly
	if (inputFile->IsOpen()) {
    		cout << "Reading file: " << inputFile->GetName() << endl;
  	}
  	else {
    		cerr << "Error opening input file!" << endl;
    		return 0;
  	}




	// create container for particles codes // convertiong to ints later with stoi(str1)
    	string particles[10][4]; 
    	particles[0][0] =  "11"; // "e-"; 
	particles[0][1] = "-11"; // "e+";
	particles[0][2] = "electrons&positrons";
	particles[0][3] = " e^{#pm}";
	particles[1][0] = "22"; // "gamma";
	particles[1][1] = "0"; //"none";
	particles[1][2] = "gammas";
	particles[1][3] = " #gamma";
	particles[2][0] = "2112"; //"neutron";
	particles[2][1] = "0"; //"none";
	particles[2][2] = "neutrons";
	particles[2][3] = " n";
	particles[3][0] = "2212"; //"proton";
	particles[3][1] = "0"; //"none";
	particles[3][2] = "protons";
	particles[3][3] = " p";
	particles[4][0] = "211"; //"pi+";
	particles[4][1] = "0"; //"none";
	particles[4][2] = "pionsplus";
	particles[4][3] = " #pi^{+}";
	particles[5][0] = "-211"; //"pi-";
	particles[5][1] = "0"; //"none";
	particles[5][2] = "pionsminus";
	particles[5][3] = " #pi^{-}";
	// part for the muon daugh
    	particles[6][0] =  "11"; // "e-"; 
	particles[6][1] = "0"; // "none";
	particles[6][2] = "electrons";
	particles[6][3] = " e^{-}";
	particles[7][0] = "-11"; // "e+";
	particles[7][1] = "0"; //"none";
	particles[7][2] = "positrons";
	particles[7][3] = " e^{+}";
	particles[8][0] = "22"; //"gamma";
	particles[8][1] = "0"; //"none";
	particles[8][2] = "gammas";
	particles[8][3] = " #gamma";
	particles[9][0] = "211"; //"pi+";
	particles[9][1] = "-211"; //"pi-";
	particles[9][2] = "pionsplusminus";
	particles[9][3] = " #pi^{#pm}";


	//Declaring the histograms for the muon
	TH1* hm1;
	TH1* hm[4];
	inputFile->GetObject("hspec",hm1);
	int nentries = (int) hm1->GetEntries();
	cout << "Number of muons simulated (entries in the Tree): " << nentries << endl; // remember that the try has one entry per event so nentries = n events
	if( nentries < 1 ){
		cerr << "No data to analyze." << endl;
		return 0;
	}
	
	// variables for naming the hists
	string histname;



	// naming the histograms for muon daug hists
	for( int j =0; j < 4 ; ++j){
		//Creating the histogram
		histname = "daug_h" + particles[j+6][2];
		inputFile->GetObject(histname.c_str(), hm[j]);

	}


	// number of bins printed
	for( int j =0; j < 4 ; ++j){
		cout << "Number of bin in the muon daug hist " << particles[j+6][2]  << " : " << hm[j]->GetNbinsX()  << " . With the lowest edge at : " << hm[j]->GetBinLowEdge(1) << " and highest edge at : " << hm[j]->GetBinLowEdge(hm[j]->GetNbinsX())  + hm[j]->GetBinWidth (hm[j]->GetNbinsX()) << endl;
	}
	cout << "Number of bin in the muon spec hist : " << hm1->GetNbinsX()  << " . With the lowest edge at : " << hm1->GetBinLowEdge(1) << " and highest edge at : " << hm1->GetBinLowEdge(hm1->GetNbinsX())  + hm1->GetBinWidth (hm1->GetNbinsX()) << endl;



	inputFile->Close();



	return 1;

}
