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

int figures_tablev2(){
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
	


	// Create a new file where we gonna save the final histograms
	string filename = "histos.root";
	TFile *outputFile = new TFile(filename.c_str(),"RECREATE");
  	if (outputFile->IsOpen()) {
    		cout << "Everything is fine!" << endl;
  	}
  	else {
    		cerr << "That should not happen." << endl;
		return 0;
  	}


	//Declaring the histogram for distance and path length
	TH1* h[3][6];
	//Creating the histograms for isodistance
	TH1* h1;
	inputFile->GetObject("TransverseDistanceIsotopes",h1);
	TH1* h2;
	inputFile->GetObject("TransverseDistanceNitrogen",h2);
	TH1* h3;
	inputFile->GetObject("TransverseDistanceBoron", h3);
	//Declaring the histogram for muons daughters
	TH1* hm[4];
	//Declaring the histograms for the muon
	TH1* hm1;
	inputFile->GetObject("hspec",hm1);
	int nentries = (int) hm1->GetEntries();
	cout << "Number of muons simulated (entries in the Tree): " << nentries << endl; // remember that the try has one entry per event so nentries = n events
	if( nentries < 1 ){
		cerr << "No data to analyze." << endl;
		return 0;
	}
	TH1* hm2;
	inputFile->GetObject("hdep",hm2);


	// variables for naming the hists
	string histname;


	// naming the histograms for trans distance
	for( int j =0; j < 6 ; ++j){
		//Creating the histogram
		histname = "trans_h" + particles[j][2];
		inputFile->GetObject(histname.c_str(), h[0][j]);
			
	}


	// naming the histograms for path length
	for( int j =0; j < 6 ; ++j){
		//Creating the histogram
		histname = "path_h" + particles[j][2];
		inputFile->GetObject(histname.c_str(), h[1][j]);;
			
	}


	// naming the histograms for muon daug hists
	for( int j =0; j < 4 ; ++j){
		//Creating the histogram
		histname = "daug_h" + particles[j+6][2];
		inputFile->GetObject(histname.c_str(), hm[j]);

	}


	// naming the histograms for the parents
	for( int j =0; j < 6 ; ++j){
		//Creating the histogram
		histname = "parents_h" + particles[j][2];// + "2";
		inputFile->GetObject(histname.c_str(), h[2][j]);
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
	// MAKING THE TRANS DISTANCE FIGURE
	//
	// Checking if the histogram was filled for the trans distance
	for( int j =0; j < 6; ++j){
		cout << "Number of entries in the trans dist hist " << particles[j][2]  << " : " << h[0][j]->GetEntries() << endl;

			
		// Histogram attributes
   		h[0][j]->SetLineColor(colors[j]);
    		h[0][j]->SetLineWidth(3);
    		h[0][j]->SetLineStyle(1);
		h[0][j]->GetXaxis()->SetTitle("Distance to muon track [cm]");
		h[0][j]->GetYaxis()->SetTitle("Normalized probability");
		//h[0][j]->SetTitleFont(62,"xyz"); // font option
		//h[0][j]->SetLabelFont(62,"xyz"); // font option
		h[0][j]->GetYaxis()->SetTitleOffset(1.6);
		h[0][j]->GetXaxis()->SetTitleOffset(1.25);
		h[0][j]->GetXaxis()->SetLabelSize(.05);
		h[0][j]->GetYaxis()->SetLabelSize(.05);
		h[0][j]->GetXaxis()->SetTitleSize(.055);
		h[0][j]->GetYaxis()->SetTitleSize(.055);
		h[0][j]->GetXaxis()->SetTickLength(0.02);
		h[0][j]->GetYaxis()->SetTickLength(0.02);
		h[0][j]->GetXaxis()->SetNdivisions( 10,true);
		h[0][j]->SetStats(kFALSE);


		// Saving the histogram	
		h[0][j]->Write();


		// normalizing the histogram
		Double_t norm = h[0][j]->GetEntries(); 
		h[0][j]->Scale(1/norm);


		// drawing the histograms
		h[0][j]->SetTitle("");
		h[0][j]->Draw(" HIST SAME 9");
		h[0][j]->SetAxisRange(0.0001, 0.1,"Y");
		h[0][j]->SetAxisRange(0., 300,"X");


		// Adding the legend
		leg[0]->AddEntry(h[0][j], particles[j][3].c_str(),"E");
 	}


	// Draw a legend 
	leg[0]->DrawClone("Same");


	// finilizing the figure and saving it
	c1->SaveAs("TrasverseDistance.eps");



	// clean for the next figure
	c1->Clear();
	gPad->SetLogx(0);
	gPad->SetLogy(0);


	//
	// MAKING THE ISO DISTANCE FIGURE
	//
	// Checking if the histograms were filled for the isodistance
	cout << "Number of entries in the iso dist all hist : " << h1->GetEntries() << endl;
	cout << "Number of entries in the iso dist N hist : " << h2->GetEntries() << endl;
	cout << "Number of entries in the iso dist B hist : " << h3->GetEntries() << endl;


	// drawing the comulative distribution 1
	TH1* hc = h1->GetCumulative();
	hc->SetTitle("");
	hc->SetLineColor(4);
    	hc->SetLineWidth(3);
	hc->GetXaxis()->SetTitle("Isotope distance to muon track [cm]");
	hc->GetYaxis()->SetTitle("Probability");
	hc->GetYaxis()->SetTitleOffset(1.5);
	hc->GetXaxis()->SetTitleOffset(1.25);
	hc->GetXaxis()->SetLabelSize(.05);
	hc->GetYaxis()->SetLabelSize(.05);
	hc->GetXaxis()->SetTitleSize(.055);
	hc->GetYaxis()->SetTitleSize(.055);
	hc->SetStats(kFALSE);
	hc->Scale(1/h1->GetEntries()); //normalizing
	hc->SetAxisRange(0., 1.,"Y");
	hc->GetXaxis()->SetNdivisions( 10,true);
	hc->GetYaxis()->SetNdivisions( 5,true);
	hc->GetXaxis()->SetTickLength(0.02);
	hc->GetYaxis()->SetTickLength(0.02);
	hc->SetAxisRange(0., 300.,"X");
	hc->DrawClone("HIST 9");


	// Saving the histogram	
	hc->Write();


	// Adding the legend
	leg[2]->AddEntry(hc,"   total","L");


	// drawing the comulative distribution 3
	hc = h3->GetCumulative();
	hc->SetTitle("");
	hc->SetLineColor(2);
    	hc->SetLineWidth(3);
    	hc->SetLineStyle(1);
	hc->SetStats(kFALSE);
	hc->Scale(1/h3->GetEntries()); //normalizing
	hc->SetAxisRange(0., 1.,"Y");
	hc->SetAxisRange(0., 300.,"X");
	hc->DrawClone("HIST 9 SAME");
	// Saving the histogram	
	hc->Write();
	// Adding the legend
	leg[2]->AddEntry(hc,"   ^{8}B","L");


	// drawing the comulative distribution 2
	hc = h2->GetCumulative();
	hc->SetTitle("");
	hc->SetLineColor(8);
    	hc->SetLineWidth(3);
    	hc->SetLineStyle(9);
	hc->SetStats(kFALSE);
	hc->Scale(1/h2->GetEntries()); //normalizing
	hc->SetAxisRange(0., 1.,"Y");
	hc->SetAxisRange(0., 300.,"X");
	hc->DrawClone("HIST 9 SAME");
	// Saving the histogram	
	hc->Write();
	// Adding the legend
	leg[2]->AddEntry(hc,"  ^{16}N","L");


	// Draw a legend 
	leg[2]->DrawClone("Same");


	// finilizing the figure and saving it
	c1->SaveAs("TrasverseDistanceIsotopes.eps");

	

	// clean for the next figure
	c1->Clear();
	gPad->SetLogx(0);
	gPad->SetLogy(0);


	//
	// MAKING THE PATH LENGTH FIGURE
	//
	for( int j =0; j < 6 ; ++j){ // path length plotting
	
		// Checking if the histogram was filled
		cout << "Number of entries in the path length hist " << particles[j][2]  << " : " << h[1][j]->GetEntries() << endl;

		// normalizing per event
		h[1][j]->Scale(1./(1.*nentries));

			
		// Histogram attributes
   		h[1][j]->SetLineColor(colors[j]);
    		h[1][j]->SetLineWidth(3);
    		h[1][j]->SetLineStyle(1);
		h[1][j]->GetXaxis()->SetTitle("Energy [MeV]");
		h[1][j]->GetYaxis()->SetTitle("Path  length [cm MeV^{-1} ]");
		h[1][j]->GetYaxis()->SetTitleOffset(1.6);
		h[1][j]->GetXaxis()->SetTitleOffset(1.25);
		h[1][j]->GetXaxis()->SetLabelSize(.05);
		h[1][j]->GetYaxis()->SetLabelSize(.05);
		h[1][j]->GetXaxis()->SetTitleSize(.055);
		h[1][j]->GetYaxis()->SetTitleSize(.055);
		h[1][j]->SetAxisRange(0.001, 1000.,"Y");
		h[1][j]->SetAxisRange(0., 500.,"X");
		h[1][j]->GetXaxis()->SetNdivisions( 6,true);
		h[1][j]->GetXaxis()->SetTickLength(0.02);
		h[1][j]->GetYaxis()->SetTickLength(0.02);
		h[1][j]->SetStats(kFALSE);

		// Saving the histogram	
		h[1][j]->Write();

		// drawing the histograms
		h[1][j]->SetTitle("");
		if( j == 0){
			h[1][j]->DrawClone("HIST 9");
		}
		else{
			h[1][j]->DrawClone("HIST 9 SAME");
		}
		// Adding the legend
		leg[1]->AddEntry(h[1][j],particles[j][3].c_str(),"E");
 	}


	// Draw a legend 
	leg[1]->DrawClone("Same");


	// finilizing the figure and saving it
	gPad->SetLogy();


	//->Range(-10,-1,10,1);
	c1->SaveAs("PathLength.eps");



	// clean for the next figure
	c1->Clear();
	gPad->SetLogx(0); 
	gPad->SetLogy(0);;


	//
	// MAKING THE MUON DAUG FIGURE
	//
	for( int j =0; j < 4 ; ++j){ // path length plotting
	
		// Checking if the histogram was filled
		cout << "Number of entries in the muon daug hist " << particles[j+6][2]  << " : " << hm[j]->GetEntries() << endl;

		// normalizing per event
		hm[j]->Scale(1./(1.*nentries));
			
		// Histogram attributes
   		hm[j]->SetLineColor(colors[j+6]);
    		hm[j]->SetLineWidth(3);
    		hm[j]->SetLineStyle(1);
		hm[j]->GetXaxis()->SetTitle("Energy [GeV]");
		hm[j]->GetYaxis()->SetTitle("dN/dlog_{10}E");
		hm[j]->GetXaxis()->SetTitleOffset(1.25);
		hm[j]->GetYaxis()->SetTitleOffset(1.4);
		hm[j]->GetXaxis()->SetLabelSize(.05);
		hm[j]->GetYaxis()->SetLabelSize(.05);
		hm[j]->GetXaxis()->SetTitleSize(.055);
		hm[j]->GetYaxis()->SetTitleSize(.055);
		hm[j]->GetXaxis()->SetTickLength(0.02);
		hm[j]->GetYaxis()->SetTickLength(0.02);
		hm[j]->SetAxisRange(0.000001, 0.1,"Y");
		hm[j]->SetAxisRange(0.1, 100.,"X");
		hm[j]->SetStats(kFALSE);

		// Saving the histogram	
		hm[j]->Write();

		// drawing the histograms
		hm[j]->SetTitle("");
		if( j == 0){
			hm[j]->DrawClone("HIST 9");
		}
		else{
			hm[j]->DrawClone("HIST 9 SAME");
		}
		// Adding the legend
		leg[3]->AddEntry(hm[j],particles[j+6][3].c_str(),"E");
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

	// Checking if the histogram was filled for the muon
	cout << "Number of entries in the muon ener loss hist : " << hm2->GetEntries() << endl;


	// normalizing per event
	hm1->Scale(1./(1.*nentries));
				

	// Histograms attributes 
   	hm1->SetLineColor(4);
    	hm1->SetLineWidth(4);
    	hm1->SetLineStyle(1);
	hm1->GetXaxis()->SetTitle("Energy [GeV]");
	hm1->GetYaxis()->SetTitle("N^{-1} dN/dlog_{10}E_{#mu} ");// [cm^{-2} s^{-1} ]");
	hm1->GetYaxis()->SetTitleOffset(1.6);
	hm1->GetXaxis()->SetTitleOffset(1.25);
	hm1->GetXaxis()->SetLabelSize(.05);
	hm1->GetYaxis()->SetLabelSize(.05);
	hm1->GetXaxis()->SetTitleSize(.055);
	hm1->GetYaxis()->SetTitleSize(.055);
	hm1->GetXaxis()->SetTickLength(0.02);
	hm1->GetYaxis()->SetTickLength(0.02);
	hm1->SetAxisRange(0.00001, 0.01,"Y");
	hm1->SetAxisRange(0.1, 10000.,"X");
	hm1->SetStats(kFALSE);
	
	hm2->SetLineColor(4);
    	hm2->SetLineWidth(4);
    	hm2->SetLineStyle(1);
	hm2->GetXaxis()->SetTitle("Muon energy loss [GeV]");
	hm2->GetYaxis()->SetTitle("Normalized probability");
	hm2->GetYaxis()->SetTitleOffset(1.6);
	hm2->GetXaxis()->SetTitleOffset(1.25);
	hm2->GetXaxis()->SetLabelSize(.05);
	hm2->GetYaxis()->SetLabelSize(.05);
	hm2->GetXaxis()->SetTitleSize(.055);
	hm2->GetYaxis()->SetTitleSize(.055);
	hm2->GetXaxis()->SetNdivisions( 5,true);
	hm2->GetYaxis()->SetNdivisions( 6,true);
	hm2->GetXaxis()->SetTickLength(0.02);
	hm2->GetYaxis()->SetTickLength(0.02);
	hm2->SetAxisRange(5., 20.,"X");
	//hm2->SetAxisRange(0., 0.2,"Y");
	hm2->SetStats(kFALSE);


	// Saving histograms	
	hm1->Write();		
	//h1l->Write();
	hm2->Write();		

	// drawing the histograms
	hm2->SetTitle("");
	hm2->DrawNormalized("HIST 9");
	// finilizing the figure and saving it
	c1->SaveAs("MuonEnergyLost.eps");


	c1->Clear();
	hm1->SetTitle("");
	hm1->DrawClone("HIST C 9");


	// finilizing the figure and saving it
	gPad->SetLogx();
	gPad->SetLogy();
	c1->SaveAs("MuonSpectra.eps");



	// clean for the next figure
	c1->Clear();
	gPad->SetLogx(0); 
	gPad->SetLogy(0);;


	//
	// MAKING THE PARENTS FIGURE
	//
	// Checking if the histogram was filled for the parents
	for( int j =0; j < 6 ; ++j){
	
		// Checking if the histogram was filled
		cout << "Number of entries in the parents hist " << particles[j][2]  << " : " << h[2][j]->GetEntries() << endl;

		// normalizing per event
		h[2][j]->Scale(1./(1.*nentries));
			
		// Histogram attributes
   		h[2][j]->SetLineColor(colors[j]);
    		h[2][j]->SetLineWidth(3);
    		h[2][j]->SetLineStyle(1);
		h[2][j]->GetXaxis()->SetTitle("Energy [MeV]");
		h[2][j]->GetYaxis()->SetTitle("# of events");
		h[2][j]->GetYaxis()->SetTitleOffset(1.6);
		h[2][j]->GetXaxis()->SetTitleOffset(1.25);
		h[2][j]->GetXaxis()->SetLabelSize(.05);
		h[2][j]->GetYaxis()->SetLabelSize(.05);
		h[2][j]->GetXaxis()->SetTitleSize(.055);
		h[2][j]->GetYaxis()->SetTitleSize(.055);
		h[2][j]->GetXaxis()->SetNdivisions( 6,true);
		h[2][j]->GetXaxis()->SetTickLength(0.02);
		h[2][j]->GetYaxis()->SetTickLength(0.02);
		h[2][j]->SetAxisRange(0.00001, 100.,"Y");
		h[2][j]->SetAxisRange(-25., 500.,"X");
		h[2][j]->SetStats(kFALSE);


		// Saving the histogram	
		h[2][j]->Write();


		// drawing the histograms
		h[2][j]->SetTitle("");
		if( j == 0){
			h[2][j]->DrawClone("HIST 9");
		}
		else{
			h[2][j]->DrawClone("HIST 9 SAME");
		}
 	}


	// Draw a legend 
	leg[0]->DrawClone("Same");


	// finilizing the figure and saving it
	gPad->SetLogy();
	c1->SaveAs("ParentsSpectra.eps");





	// Closing files
	inputFile->Close();
	outputFile->Close();




	//
	// MAKING THE MUON MEAN AND THEN THE ISOTOPES LIST
	//




	// Opening the file with the tree
	TFile* listFile = new TFile("alldatalist.root");
	// checking if the file is open correctly
	if (listFile->IsOpen()) {
    		cout << "Reading file: " << listFile->GetName() << endl;
  	}
  	else {
    		cerr << "Error opening input file!" << endl;
    		return 0;
  	}


	// Getting the tree
	TTree* DataTree;
	listFile->GetObject("iso_list",DataTree);
	int nentries3 = (int)DataTree->GetEntries();

	// Getting the tree
	TTree* DataTree2;
	listFile->GetObject("muon_mean",DataTree2);
	int nentries2 = (int)DataTree2->GetEntries();
	cout << "Number of simulations (entries in the mean muon Tree): " << nentries2 << endl; // remember that the try has one entry per event so nentries = n events
	if( nentries2 < 1 ){
		cerr << "No data to analyze." << endl;
		return 0;
	}



	int 	A, Z, stable , quantity;
	// creating variables for the branches of the muon
	double Energy, Edep;



	DataTree->SetBranchAddress("A",&A);
	DataTree->SetBranchAddress("Z",&Z);
	DataTree->SetBranchAddress("Stable",&stable);
	DataTree->SetBranchAddress("Quantity",&quantity);
	DataTree2->SetBranchAddress("meanEnergy",&Energy);
	DataTree2->SetBranchAddress("meanEdep",&Edep);



	
	// variables for the mean of muon 
	double meanSpec = 0.;
	double meanLost = 0.;
	double stddevSpec = 0.;
	double stddevLost = 0.;
	// variables for the values
	vector<double> ESpec;
	vector<double> ELost;
	// variables for the sum of the difference square
	double sumEspec = 0.;
 	double sumElost = 0.;



	// Running through the muon tree and filling the mean calculator
	for( int i =0; i < nentries2; ++i){
		DataTree2->GetEntry(i);


		meanSpec = Energy + meanSpec; // somming up for the mean
		ESpec.push_back(Energy);
		if (Energy < 0.){ 
			cerr << "Negative Kinetic Energy Value found : " << Energy/1000. << "GeV" << endl; 
    			return 0;
		}
		meanLost = Edep + meanLost; // somming up for the mean
		ELost.push_back(Edep);


		
		cout << "Simulation " << i << " has been processed." << endl;
 	}


	// calculating the mean
	meanSpec = meanSpec/( nentries2 ); // dividing by the n of muons
	meanLost = meanLost/( nentries2 ); // dividing by the n of muons

	// calculation std dev
	for(int i=0; i < ESpec.size(); i++){
 		  sumEspec = (ESpec[i]-meanSpec)*(ESpec[i]-meanSpec) + sumEspec;
 		  sumElost = (ELost[i]-meanLost)*(ELost[i]-meanLost) + sumElost;
	}
 	stddevSpec = sqrt(sumEspec/(nentries2-1));
	stddevLost = sqrt(sumElost/(nentries2-1));
	



	// printing the mean values
	cout << "Mean energy of the spectrum : " << meanSpec << endl;
	cout << "Mean energy loss : " << meanLost << endl;
	cout << "Std dev energy of the spectrum : " << stddevSpec << endl;
	cout << "Std dev energy loss : " << stddevLost << endl;



	// create container for particles isotopes
	std::string elements [118] = {"H","He","Li","Be","B","C","N","O","F","Ne","Na","Mg","Al","Si","P","S","Cl","Ar","K","Ca","Sc","Ti","V","Cr","Mn","Fe","Co","Ni","Cu","Zn","Ga","Ge","As","Se","Br","Kr","Rb","Sr","Y","Zr","Nb","Mo","Tc","Ru","Rh","Pd","Ag","Cd","In","Sn","Sb","Te","I","Xe","Cs","Ba","La","Ce","Pr","Nd","Pm","Sm","Eu","Gd","Tb","Dy","Ho","Er","Tm","Yb","Lu","Hf","Ta","W","Re","Os","Ir","Pt","Au","Hg","Tl","Pb","Bi","Po","At","Rn","Fr","Ra","Ac","Th","Pa","U","Np","Pu","Am","Cm","Bk","Cf","Es","Fm","Md","No","Lr","Rf","Db","Sg","Bh","Hs","Mt","Ds","Rg","Uub","Uut","Uuq","Uup","Uuh","Uus","Uuo"};


	// create the data collector for isotopes list
	std::map<int, int > multiplicity [118]; // for the total
	std::map<int, bool> Isstable [118];

	// variables for the running tree
	std::map<int, int>::iterator itr2;


	// Running through the isotopes tree and filling the list maker
	for( int i =0; i < nentries3; ++i){
		DataTree->GetEntry(i);


		if(A > 7){
			multiplicity[Z][A] 	+= quantity; // the defult initializition is 0
			Isstable[Z][A] 		= stable;
		}



		
		cout << "Row " << i << " has been processed." << endl;
 	}



	//
	// PRINTING THE LIST	
	//



	cout << "Printing list in the file" << endl;
	
	// Create a new file for the list
	ofstream outfile;
  	outfile.open ("TableOfIsotopes.txt");
	outfile.precision(2);
	outfile.setf(ios::fixed);
	outfile.setf(ios::showpoint);


	// vectors to fix the order of print
	std::vector<std::vector<double>* > * stables 	= new std::vector<std::vector<double>*  >;
	std::vector<std::vector<double>* > * unstables 	= new std::vector<std::vector<double>*  >;


	map<int, int>::iterator itr;
	std::vector<double>* arrays = new std::vector<double>;


	for( int i = 0; i < 118; i++){
    		for (itr = multiplicity[i].begin(); itr != multiplicity[i].end(); ++itr) { // i = Z , itr->first = A and itr->second = quantity
			arrays = new std::vector<double>;
			arrays->push_back(i); //id 0
			arrays->push_back(itr->first); //id 1
			arrays->push_back(itr->second); //id 2
			if ( Isstable[i][itr->first] ){
				cout << elements[i] << itr->first << " is stable" << Isstable[i][itr->first] <<" and has a quantity of " << itr->second << endl ;
				stables->push_back(arrays);
			}
			else{
				cout << elements[i] << itr->first << " is stable" << Isstable[i][itr->first] <<" and has a quantity of " << itr->second << endl ;
				unstables->push_back(arrays);
			}
		}
	}

	
	
	// print stable isotopes in decreasing order
	int location =-1;
	int max = -1;
    	outfile << "\t Stable isotopes \n"; 
    	outfile << "Isotope \t Half-life (s) \t Quantity mu^{-1} g^{-1} cm^{2} x 10^{-7} \n";
	int size = stables->size();
	for( int i = 0; i < size; i++){
    		for ( int j = 0 ; j < stables->size(); j++){
    			if ( stables->at(j)->at(2) > max ){
				max = stables->at(j)->at(2);
				location = j;
			}
		}
		outfile << '\t' << "$^{"  << stables->at(location)->at(1) << "}$" << elements[(int)stables->at(location)->at(0)]
             	<< '\t' << '&' << '\t' << max/(3200.* ( nentries )) * 10000000. << '\t' << '&' << '\t' << "\\\\" <<'\n';  // dividing per the number of muons, the distance of the muon track and by the water density 1 * g * cm^{-3} and multiplyin by 10^{7} - time multiplied by e9 because in nanosecs
		stables->erase (stables->begin()+location); //removing the maximum from the vector
		max = -1;
	}


	outfile << '\n' << '\n' << '\n';


	// print unstable isotopes in decreasing order
    	outfile << "\t Unstable isotopes \n"; 
    	outfile << "Isotope \t Half-life (s) \t Quantity mu^{-1} g^{-1} cm^{2} x 10^{-7} \n";
	size = unstables->size();
	for( int i = 0; i < size; i++){
    		for ( int j = 0 ; j < unstables->size(); j++){
    			if ( unstables->at(j)->at(2) > max ){
				max = unstables->at(j)->at(2);
				location = j;
			}
		}
		outfile << '\t' << "$^{"  << unstables->at(location)->at(1) << "}$" << elements[(int)unstables->at(location)->at(0)]
             	<< '\t' << '&' << '\t' << max/(3200.* ( nentries )) * 10000000. << '\t' << '&' << '\t' << "\\\\" <<'\n';  // dividing per the number of muons, the distance(length) of the muon track and by the water density 1 * g * cm^{-3} and multiplyin by 10^{7} - time multiplied by e9 because in nanosecs
		unstables->erase (unstables->begin()+location); //removing the maximum from the vector
		max = -1;
	}

	// printing what is a one event in yield
	outfile << "\n \n\n \n"; 
    	outfile << " A single event in isotope yield  mu^{-1} g^{-1} cm^{2} x 10^{-7} = " << 1./(3200.* ( nentries )) * 10000000. << "\n";
	
	// printing the mean values
	outfile << "\n \n\n \n"; 
	outfile << "Mean energy of the spectrum : " << meanSpec << "\n";
	outfile << "Std dev energy of the spectrum : " << stddevSpec << "\n";
	outfile << "Mean energy loss : " << meanLost << "\n";
	outfile << "Std dev energy loss : " << stddevLost << "\n";


	// Closing files
	outfile.close();
	listFile->Close();

	return 1;

}
