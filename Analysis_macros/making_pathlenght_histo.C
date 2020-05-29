// Read the tree and make a pathlenght histo plot

#include "TCanvas.h"
#include "TROOT.h"
#include "TH1.h"
#include "TTree.h"
#include "TFile.h"
#include "TLegend.h"

int making_pathlenght_histo(){
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
	inputFile->GetObject("particles_data",DataTree);
	int nentries = (int)DataTree->GetEntries();
	cout << "Number of entries in the Tree: " << DataTree->GetEntries() << endl;

	// creating variables for the branches
	Char_t Particle [25]; // contains the particle name
	double Energy, PathLenght;
	int EventID;
	// Attaching to branches
	DataTree->SetBranchAddress("Particle",&Particle);
	DataTree->SetBranchAddress("Energy",&Energy);
	DataTree->SetBranchAddress("PathLenght",&PathLenght);
	DataTree->SetBranchAddress("EventID",&EventID);

	
	// Getting the number of muons simulated
	DataTree->GetEntry( nentries - 1 );
	int nmuons = EventID;
	cout << "Number of muons simulated: " << nmuons << endl;

	// create container for particles names
    	string particles[6][3]; 
    	particles[0][0] = "e-";
	particles[0][1] = "e+";
	particles[0][2] = "eMinusePlus";
	particles[1][0] = "gamma";
	particles[1][1] = "none";
	particles[1][2] = "gamma";
	particles[2][0] = "neutron";
	particles[2][1] = "none";
	particles[2][2] = "neutron";
	particles[3][0] = "proton";
	particles[3][1] = "none";
	particles[3][2] = "proton";
	particles[4][0] = "pi+";
	particles[4][1] = "none";
	particles[4][2] = "piPlus";
	particles[5][0] = "pi-";
	particles[5][1] = "none";
	particles[5][2] = "piMinus";

	// create container for colors of the particles histograms
	int colors[6] = {8,4,2,13,7,6};

	// Create a new file
	string filename = "PathLength.root";
	TFile *outputFile = new TFile(filename.c_str(),"RECREATE");
  	if (outputFile->IsOpen()) {
    		cout << "Everything is fine!" << endl;
  	}
  	else {
    		cerr << "That should not happen." << endl;
		return 0;
  	}
	
	//Declaring the histogram
	TH1* h1;

	// Making the canvas we will draw in
	TCanvas *c1 = new TCanvas();

	// Build a legend - x1,y1,x2,y2 
	TLegend leg(.55 ,.75 ,.9 ,.9);
	leg.SetFillColor(0);
	leg.SetNColumns(2);

	for( int j =0; j < 6 ; ++j){
		if( particles[j][1].compare("none") != 0 ){// selecting the particles
			//Creating the histogram
			string histtitle = "Path Length spectra for " + particles[j][0] + " + " + particles[j][1];
			string histname = "h" + particles[j][2];
			h1 = new TH1D(histname.c_str(), histtitle.c_str(), 500, 0.0, 500.0);

			// Running through the tree and filling the histogram
			for( int i =0; i < nentries; ++i){
				DataTree->GetEntry(i);
				if( (particles[j][0].compare(Particle) == 0) || (particles[j][1].compare(Particle) == 0) ){ // if desired particle		
					double Weight = (PathLenght/nmuons)/10; // divided by ten to get the result in cm and divided by the number of muons simulated	
					h1->Fill(Energy, Weight);} // fill with path lenght weight devided by the number of events
 			}
		}
		else{
			//Creating the histogram
			string histtitle = "Path Length spectra for " + particles[j][0];
			string histname = "h" + particles[j][2];
			h1= new TH1D(histname.c_str(), histtitle.c_str(), 500, 0.0, 500.0);

			// Running through the tree and filling the histogram
			for( int i =0; i < nentries; ++i){
				DataTree->GetEntry(i);
				if( (particles[j][0].compare(Particle) == 0) ){ // if desired particle	
					double Weight = (PathLenght/nmuons)/10; // divided by ten to get the result in cm and divided by the number of muons simulated	
					h1->Fill(Energy, Weight);} // fill with path lenght weight devided by the number of events
 			}	
		}
	
		// Checking if the histogram was filled
		cout << "Number of entries in the h1gram : " << h1->GetEntries() << endl;

			
		// Histogram attributes
   		h1->SetLineColor(colors[j]);
    		h1->SetLineWidth(2);
    		h1->SetLineStyle(2);
		h1->GetXaxis()->SetTitle("E [MeV]");
		h1->GetYaxis()->SetTitle("Path  length [cm / MeV]");
		h1->GetYaxis()->SetTitleOffset(1.);
		h1->GetXaxis()->SetLabelSize(.04);
		h1->GetYaxis()->SetLabelSize(.045);
		h1->GetXaxis()->SetTitleSize(.045);
		h1->GetYaxis()->SetTitleSize(.045);
		h1->SetStats(kFALSE);

		// Saving the histogram	
		h1->Write();

		// drawing the histograms
		if ( j == 0 ){
		h1->SetTitle("");
		h1->DrawClone("C9");
		}		
		h1->DrawClone("SAME");
		// Adding the legend
		leg.AddEntry(h1,particles[j][0].c_str(),"l");
 	}

	// Draw a legend 
	leg.DrawClone("Same");

	// finilizing the figure and saving it
	gPad->SetLogy();
	c1->SaveAs("PathLength.png");

	// Closing hist file
	inputFile->Close();
	//outputFile->Close();

	return 1;

}
