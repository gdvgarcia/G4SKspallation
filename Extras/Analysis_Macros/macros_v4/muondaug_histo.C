// Read the tree and make transverse distance histos plot

#include "TCanvas.h"
#include "TROOT.h"
#include "TH1.h"
#include "TTree.h"
#include "TFile.h"
#include "TLegend.h"

#include <vector> 

int muondaug_histo(){
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
	vector<double>* FirstGenEnergy 		= new vector<double>();
	vector<int>* 	FirstGenParticle 	= new vector<int>();
	int 		FirstGenSize;


	// Attaching to branches
	DataTree->SetBranchAddress("FirstGenSize",	&FirstGenSize);
	DataTree->SetBranchAddress("FirstGenParticle",	&FirstGenParticle);
	DataTree->SetBranchAddress("FirstGenEnergy",	&FirstGenEnergy);


	
	// Getting the number of muons simulated
	cout << "Number of muons simulated: " << nentries << endl;

	// create container for particles names // convertiong to string later with stoi(str1)
    	string particles[6][4]; 
    	particles[0][0] =  "11"; // "e-"; 
	particles[0][1] = "0"; // "none";
	particles[0][2] = "electrons";
	particles[0][3] = "e^{-}";
	particles[1][0] = "-11"; // "e+";
	particles[1][1] = "0"; //"none";
	particles[1][2] = "positrons";
	particles[1][3] = "e^{+}";
	particles[2][0] = "22"; //"gamma";
	particles[2][1] = "0"; //"none";
	particles[2][2] = "gammas";
	particles[2][3] = "#gamma";
	particles[3][0] = "211"; //"pi+";
	particles[3][1] = "-211"; //"pi-";
	particles[3][2] = "pionsplusminus";
	particles[3][3] = "#pi^{#pm}";

	// create container for colors of the particles histograms
	int colors[6] = {4,6,8,2}; // 8 green, 4 blue, 2 is red, 13 is gray, 7 is cyan and 6 pink

	// Create a new file
	string filename = "MuonDaugthersSpec.root";
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
	TLegend leg(.65 ,.725 ,.9 ,.9);
	leg.SetFillColor(0);
	leg.SetNColumns(2);

	// Building log binning for h
	const Int_t nbins = 100;
   	Double_t xmin = 1e-2;
   	Double_t xmax = 1e2;
   	Double_t logxmin = TMath::Log10(xmin);
   	Double_t logxmax = TMath::Log10(xmax);
   	Double_t binwidth = (logxmax-logxmin)/nbins;
   	Double_t xbins[nbins+1];
   	xbins[0] = xmin;
   	for (Int_t i=1;i<=nbins;i++) {
      		xbins[i] = xmin + TMath::Power(10,logxmin+i*binwidth);
   	}

	// naming the histograms
	for( int j =0; j < 4 ; ++j){
		//Creating the histogram
		string histtitle = "Spectrum   for " + particles[j][2];
		string histname = "h" + particles[j][2];
		h[j] = new TH1D(histname.c_str(), histtitle.c_str(),nbins,xbins);

	}

	// Running through the tree and filling the histogram
	for( int i =0; i < nentries; ++i){
		DataTree->GetEntry(i);

		for( int k =0; k < FirstGenSize ; k++){//running through the vetors

			for( int j =0; j < 4 ; ++j){
				if( (stoi(particles[j][0]) == FirstGenParticle->at(k)) || (stoi(particles[j][1])  == FirstGenParticle->at(k)) ){ // if desired particle		
					double Weight = (1./nentries); // divided by the number of muons simulated to normalize	
					h[j]->Fill(FirstGenEnergy->at(k)/1000., Weight); // divided by 1000 to convert to GeV fill with energy and weight divided by the number of events
				}
			}
		}
 	}

	for( int j =0; j < 4 ; ++j){
	
		// Checking if the histogram was filled
		cout << "Number of entries in the h1gram : " << h[j]->GetEntries() << endl;

			
		// Histogram attributes
   		h[j]->SetLineColor(colors[j]);
    		h[j]->SetLineWidth(2);
    		h[j]->SetLineStyle(2);
		h[j]->GetXaxis()->SetTitle("Energy [GeV]");
		h[j]->GetYaxis()->SetTitle("dN/dlog_{10}E");
		h[j]->GetYaxis()->SetTitleOffset(1.);
		h[j]->GetXaxis()->SetTitleOffset(1.15);
		h[j]->GetXaxis()->SetLabelSize(.038);
		h[j]->GetYaxis()->SetLabelSize(.038);
		h[j]->GetXaxis()->SetTitleSize(.038);
		h[j]->GetYaxis()->SetTitleSize(.042);
		h[j]->SetAxisRange(0.00001, 10.,"Y");
		h[j]->SetAxisRange(0.1, 100.,"X");
		h[j]->SetStats(kFALSE);

		// Saving the histogram	
		h[j]->Write();

		// drawing the histograms
		h[j]->SetTitle("");
		h[j]->DrawClone("HIST 9 SAME");
		// Adding the legend
		leg.AddEntry(h[j],particles[j][3].c_str(),"l");
 	}

	// Draw a legend 
	leg.DrawClone("Same");

	// finilizing the figure and saving it
	gPad->SetLogy();
	gPad->SetLogx();
	//->Range(-10,-1,10,1);
	c1->SaveAs("MuonDaugthersSpec.png");

	// Closing hist file
	inputFile->Close();
	//outputFile->Close();

	return 1;
}
