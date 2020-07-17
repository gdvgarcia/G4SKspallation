// makes a figure rebinned

#include "TCanvas.h"
#include "TROOT.h"
#include "TH1.h"
#include "TTree.h"
#include "TFile.h"
#include "TLegend.h"
#include "TLatex.h"

#include <iostream> 
#include <vector> 

int rebinlogs(){
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


	// create container for colors of the particles histograms
	int colors[10] = {8,4,2,13,7,6,4,6,8,2};
	



	//Declaring the histogram for muons daughters
	TH1* hm[4];
	TH1* hmr[4];
	//Declaring the histograms for the muon
	TH1* hm1;
	TH1* hm1r;
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


	// Making the canvas we will draw in
	auto c1 = new TCanvas("c1","c1");
   	c1->SetCanvasSize(750, 750);
   	c1->SetWindowSize(600, 600);

	gPad->SetLogy(1);
	gPad->SetMargin(0.20, 0.05, 0.20, 0.05); // (left, right, bottom, top)
	gPad->SetTickx(1); 
	gPad->SetTicky(1);


	// Build the legends - x1,y1,x2,y2 
	TLegend* leg[4] = {0,0,0,0};
	// build legends to pathlength and trans distance
	for( int i = 0; i < 2 ; i++){
	leg[i] = new TLegend(.7 ,.65 ,.9 ,.9);
	leg[i]->SetFillColor(0);
	leg[i]->SetNColumns(2);
	}
	// Build a legend - x1,y1,x2,y2  for isodista
	leg[2] = new TLegend(.675 ,.25 ,.9 ,.5);
	leg[2]->SetFillColor(0);
	// Build a legend - x1,y1,x2,y2 for muon daughters
	leg[3] = new TLegend(.7 ,.725 ,.9 ,.9);
	leg[3]->SetFillColor(0);
	leg[3]->SetNColumns(2);



	
	//
	// MAKING THE MUON DAUG FIGURE
	//
	for( int j =0; j < 4 ; ++j){ // path length plotting
	
		// Checking if the histogram was filled
		cout << "Number of entries in the muon daug hist " << particles[j+6][2]  << " : " << hm[j]->GetEntries() << endl;

		histname = "hmnew" + to_string(j);;
		hmr[j] = hm[j]->Rebin(100,histname.c_str()); 

		// normalizing per event
		hmr[j]->Scale(1./(1.*nentries));
			
		// Histogram attributes
   		hmr[j]->SetLineColor(colors[j+6]);
    		hmr[j]->SetLineWidth(3);
    		hmr[j]->SetLineStyle(1);
		hmr[j]->GetXaxis()->SetTitle("Energy [GeV]");
		hmr[j]->GetYaxis()->SetTitle("dN/dlog_{10}E");
		hmr[j]->GetXaxis()->SetTitleOffset(1.25);
		hmr[j]->GetYaxis()->SetTitleOffset(1.4);
		hmr[j]->GetXaxis()->SetLabelSize(.05);
		hmr[j]->GetYaxis()->SetLabelSize(.05);
		hmr[j]->GetXaxis()->SetTitleSize(.055);
		hmr[j]->GetYaxis()->SetTitleSize(.055);
		hmr[j]->GetXaxis()->SetTickLength(0.02);
		hmr[j]->GetYaxis()->SetTickLength(0.02);
		hmr[j]->SetAxisRange(0.00001, 1.,"Y");
		hmr[j]->SetAxisRange(0.1, 100.,"X");
		hmr[j]->SetStats(kFALSE);

		// drawing the histograms
		hmr[j]->SetTitle("");
		if( j == 0){
			hmr[j]->DrawClone("HIST 9");
		}
		else{
			hmr[j]->DrawClone("HIST 9 SAME");
		}
		// Adding the legend
		leg[3]->AddEntry(hmr[j],particles[j+6][3].c_str(),"E");
 	}


	// Draw a legend 
	leg[3]->DrawClone("Same");


	// finilizing the figure and saving it
	gPad->SetLogy();
	gPad->SetLogx();


	//->Range(-10,-1,10,1);
	c1->SaveAs("MuonDaugthersSpec.eps");



	// clean for the next figure
	c1->Clear();
	gPad->SetLogx(0); 
	gPad->SetLogy(0);;


	//
	// MAKING THE MUON FIGURE
	//
	// Checking if the histogram was filled for the muon
	cout << "Number of entries in the muon spec hist : " << hm1->GetEntries() << endl;

	hm1r = hm1->Rebin(100,"hm1new"); 
	// normalizing per event
	hm1r->Scale(1./(1.*nentries));
				

	// Histograms attributes 
   	hm1r->SetLineColor(4);
    	hm1r->SetLineWidth(4);
    	hm1r->SetLineStyle(1);
	hm1r->GetXaxis()->SetTitle("Energy [GeV]");
	hm1r->GetYaxis()->SetTitle("N^{-1} dN/dlog_{10}E_{#mu} ");// [cm^{-2} s^{-1} ]");
	hm1r->GetYaxis()->SetTitleOffset(1.6);
	hm1r->GetXaxis()->SetTitleOffset(1.25);
	hm1r->GetXaxis()->SetLabelSize(.05);
	hm1r->GetYaxis()->SetLabelSize(.05);
	hm1r->GetXaxis()->SetTitleSize(.055);
	hm1r->GetYaxis()->SetTitleSize(.055);
	hm1r->GetXaxis()->SetTickLength(0.02);
	hm1r->GetYaxis()->SetTickLength(0.02);
	hm1r->SetAxisRange(0.00001, 0.01,"Y");
	hm1r->SetAxisRange(0.1, 10000.,"X");
	hm1r->SetStats(kFALSE);
		


	c1->Clear();
	hm1r->SetTitle("");
	hm1r->DrawClone("HIST C 9");


	// finilizing the figure and saving it
	gPad->SetLogx();
	gPad->SetLogy();
	c1->SaveAs("MuonSpectra.eps");



	// clean for the next figure
	c1->Clear();
	gPad->SetLogx(0); 
	gPad->SetLogy(0);;



	// Closing files
	inputFile->Close();


	return 1;

}
