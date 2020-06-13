// Read the tree and make transverse distance histos plot

#include "TCanvas.h"
#include "TROOT.h"
#include "TH1.h"
#include "TTree.h"
#include "TFile.h"
#include "TLegend.h"

#include <iostream> 
#include <vector> 

int distance_histo(){
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
	vector<double>* TransParticlesDistance = new vector<double>();
	vector<int>* TransParticlesCode = new vector<int>();
	int TransParticlesSize;


	// Attaching to branches
	DataTree->SetBranchAddress("TransParticlesSize",&TransParticlesSize);
	DataTree->SetBranchAddress("TransParticlesCode",&TransParticlesCode);
	DataTree->SetBranchAddress("TransParticlesDistance",&TransParticlesDistance);


	// create container for particles names // convertiong to string later with stoi(str1)
    	string particles[6][4]; 
    	particles[0][0] =  "11"; // "e-"; 
	particles[0][1] = "-11"; // "e+";
	particles[0][2] = "electrons&positrons";
	particles[0][3] = "e^{#pm}";
	particles[1][0] = "22"; // "gamma";
	particles[1][1] = "0"; //"none";
	particles[1][2] = "gammas";
	particles[1][3] = "#gamma";
	particles[2][0] = "2112"; //"neutron";
	particles[2][1] = "0"; //"none";
	particles[2][2] = "neutrons";
	particles[2][3] = "n";
	particles[3][0] = "2212"; //"proton";
	particles[3][1] = "0"; //"none";
	particles[3][2] = "protons";
	particles[3][3] = "p";
	particles[4][0] = "211"; //"pi+";
	particles[4][1] = "0"; //"none";
	particles[4][2] = "pionsplus";
	particles[4][3] = "#pi^{+}";
	particles[5][0] = "-211"; //"pi-";
	particles[5][1] = "0"; //"none";
	particles[5][2] = "pionsminus";
	particles[5][3] = "#pi^{-}";

	// create container for colors of the particles histograms
	int colors[6] = {8,4,2,13,7,6};

	// Create a new file
	string filename = "TransParticlesDistance.root";
	TFile *outputFile = new TFile(filename.c_str(),"RECREATE");
  	if (outputFile->IsOpen()) {
    		cout << "Everything is fine!" << endl;
  	}
  	else {
    		cerr << "That should not happen." << endl;
		return 0;
  	}
	
	//Declaring the histogram
	TH1* h[6];

	// Making the canvas we will draw in
	auto c1 = new TCanvas("c1","c1",10,10,650,600);

	// Build a legend - x1,y1,x2,y2 
	TLegend leg(.6 ,.65 ,.9 ,.9);
	leg.SetFillColor(0);
	leg.SetNColumns(2);

	// naming the histograms
	for( int j =0; j < 6 ; ++j){
		//Creating the histogram
		string histtitle = "Transverse distance for " + particles[j][2];
		string histname = "h" + particles[j][2];
		h[j] = new TH1D(histname.c_str(), histtitle.c_str(), 300, 0.0, 300.0);
			
	}

	// Running through the tree and filling the histograms
	for( int i =0; i < nentries; ++i){
		DataTree->GetEntry(i);

		for( int k =0; k < TransParticlesSize ; k++){//running through the vetors

			for( int j =0; j < 6 ; ++j){	
				if( (stoi(particles[j][0]) == TransParticlesCode->at(k)) || (stoi(particles[j][1])  == TransParticlesCode->at(k)) ){ // if desired particles	
				double distance = TransParticlesDistance->at(k) / 10.; // to convert mm to cm
				h[j]->Fill(distance);
				}
			}
		}
 	}


	// Checking if the histogram was filled
	for( int j =0; j < 6; ++j){
		cout << "Number of entries in the h1gram : " << h[j]->GetEntries() << endl;

			
		// Histogram attributes
   		h[j]->SetLineColor(colors[j]);
    		h[j]->SetLineWidth(3);
    		h[j]->SetLineStyle(2);
		h[j]->GetXaxis()->SetTitle("Distance to muon track  [cm]");
		h[j]->GetYaxis()->SetTitle("Normalized probability");
		h[j]->GetYaxis()->SetTitleOffset(1.15);
		h[j]->GetXaxis()->SetLabelSize(.038);
		h[j]->GetYaxis()->SetLabelSize(.038);
		h[j]->GetXaxis()->SetTitleSize(.038);
		h[j]->GetYaxis()->SetTitleSize(.042);
		h[j]->SetStats(kFALSE);

		// Saving the histogram	
		h[j]->Write();

		// normalizing the histogram
		Double_t norm = h[j]->GetEntries(); 
		h[j]->Scale(1/norm);

		// drawing the histograms
		h[j]->SetTitle("");
		h[j]->Draw(" HIST SAME C 9");
		h[j]->SetAxisRange(0.0001, 0.1,"Y");

		// Adding the legend
		leg.AddEntry(h[j],particles[j][3].c_str(),"l");
 	}

	// Draw a legend 
	leg.DrawClone("Same");

	// finilizing the figure and saving it
	gPad->SetLogy();
	c1->SaveAs("TrasverseDistance.png");

	// Closing hist file
	inputFile->Close();
	//outputFile->Close();

	return 1;

}
