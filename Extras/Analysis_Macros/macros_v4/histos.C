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

int histos(){
	// Opening the file with the tree
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
	cout << "Number of entries in the Tree: " << nentries << endl; // remember that the try has one entry per event so nentries = n events
	if( nentries < 1 ){
		cerr << "No data to analyze." << endl;
		return 0;
	}



	// creating variables for the branches of the trans distance
	vector<double>* TransParticlesDistance = new vector<double>();
	vector<int>* TransParticlesCode = new vector<int>();
	int TransParticlesSize;
	// creating variables for the branches of the isotopes
	vector<double>* TransverseDistance  	= new vector<double>();
	vector<double>* LifeTime	  	= new vector<double>();
	vector<int>* 	A 			= new vector<int>();
	vector<int>* 	Z 			= new vector<int>();
	vector<bool>* 	stable 	= new vector<bool>(); // for the iso list
	int 		IsotopesSize;
	// creating variables for the branches of the path length
	vector<double>* StepLength  		= new vector<double>();
	vector<double>* PathLengthEnergy  	= new vector<double>();
	vector<int>* 	PathLengthCode 		= new vector<int>();
	int 		PathLengthSize;
	// creating variables for the branches of the muon daughters
	vector<double>* FirstGenEnergy 		= new vector<double>();
	vector<int>* 	FirstGenParticle 	= new vector<int>();
	int 		FirstGenSize;
	// creating variables for the branches of the muon
	double Energy, Edep;
	// creating variables for the branches for the parents
	vector<double>* ParentsEnergy 	= new vector<double>();
	vector<int>* 	ParentsCode 	= new vector<int>();
	int 		ParentsSize;



	// Attaching to branches
	DataTree->SetBranchAddress("TransParticlesSize",&TransParticlesSize);
	DataTree->SetBranchAddress("TransParticlesCode",&TransParticlesCode);
	DataTree->SetBranchAddress("TransParticlesDistance",&TransParticlesDistance);
	DataTree->SetBranchAddress("IsotopesSize",&IsotopesSize);
	DataTree->SetBranchAddress("A",&A);
	DataTree->SetBranchAddress("Z",&Z);
	//DataTree->SetBranchAddress("LifeTime",&LifeTime);
	DataTree->SetBranchAddress("Stable1Instable0",&stable);
	DataTree->SetBranchAddress("TransverseDistance",&TransverseDistance);
	DataTree->SetBranchAddress("PathLengthSize",&PathLengthSize);
	DataTree->SetBranchAddress("StepLength",&StepLength);
	DataTree->SetBranchAddress("PathLengthEnergy",&PathLengthEnergy);
	DataTree->SetBranchAddress("PathLengthCode",&PathLengthCode);
	DataTree->SetBranchAddress("FirstGenSize",	&FirstGenSize);
	DataTree->SetBranchAddress("FirstGenParticle",	&FirstGenParticle);
	DataTree->SetBranchAddress("FirstGenEnergy",	&FirstGenEnergy);
	DataTree->SetBranchAddress("Energy",&Energy);
	DataTree->SetBranchAddress("Edep",&Edep);
	DataTree->SetBranchAddress("ParentsSize",&ParentsSize);
	DataTree->SetBranchAddress("ParentsCode",&ParentsCode);
	DataTree->SetBranchAddress("ParentsEnergy",&ParentsEnergy);



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



	// create container for particles isotopes
	string elements [118] = {"H","He","Li","Be","B","C","N","O","F","Ne","Na","Mg","Al","Si","P","S","Cl","Ar","K","Ca","Sc","Ti","V","Cr","Mn","Fe","Co","Ni","Cu","Zn","Ga","Ge","As","Se","Br","Kr","Rb","Sr","Y","Zr","Nb","Mo","Tc","Ru","Rh","Pd","Ag","Cd","In","Sn","Sb","Te","I","Xe","Cs","Ba","La","Ce","Pr","Nd","Pm","Sm","Eu","Gd","Tb","Dy","Ho","Er","Tm","Yb","Lu","Hf","Ta","W","Re","Os","Ir","Pt","Au","Hg","Tl","Pb","Bi","Po","At","Rn","Fr","Ra","Ac","Th","Pa","U","Np","Pu","Am","Cm","Bk","Cf","Es","Fm","Md","No","Lr","Rf","Db","Sg","Bh","Hs","Mt","Ds","Rg","Uub","Uut","Uuq","Uup","Uuh","Uus","Uuo"};


	// create the data collector for isotopes list
	map<int, int > multiplicity [118]; // for the total
	map<int, double > LifeTimes [118]; // for the total
	map<int, int > multiplicityEvent [118]; // to collect per event data 
	map<int, vector<int> > multiplicityEvents [118]; // to collect many events data in the vector
	map<int, double > multiplicityError [118]; // to save the erros
	map<int, bool> Isstable [118];


	
	// variables for the mean
	double meanSpec = 0.;
	double meanLost = 0.;



	// Create a new file where we gonna save the histograms
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
	TH1* h1 = new TH1D("TransverseDistanceIsotopes", "Transverse Distance for all the isotopes", 400,  -50., 350.0);
	TH1* h2 = new TH1D("TransverseDistanceNitrogen", "Transverse Distance for N16", 400, -50., 350.0);
	TH1* h3 = new TH1D("TransverseDistanceBoron", "Transverse Distance for B8", 400, -50., 350.0);
	//Declaring the histogram for muons daughters
	TH1* hm[4];
	//Declaring the histograms for the muon
	TH1* hm1;
	int nbinhm2 = 100; // number of bins for the energy loss
	TH1* hm2 = new TH1D("hdep", "Muon energy loss", nbinhm2, 0, 50);



	// Making the canvas we will draw in
	auto c1 = new TCanvas("c1","c1",10,10,650,600);


	// Build the legends - x1,y1,x2,y2 
	TLegend* leg[4] = {0,0,0,0};
	// build legends to pathlength and trans distance
	for( int i = 0; i < 2 ; i++){
	leg[i] = new TLegend(.7 ,.65 ,.9 ,.9);
	leg[i]->SetFillColor(0);
	leg[i]->SetNColumns(2);
	}
	// Build a legend - x1,y1,x2,y2  for isodista
	leg[2] = new TLegend(.675 ,.2 ,.9 ,.45);
	leg[2]->SetFillColor(0);
	// Build a legend - x1,y1,x2,y2 for muon daughters
	leg[3] = new TLegend(.7 ,.725 ,.9 ,.9);
	leg[3]->SetFillColor(0);
	leg[3]->SetNColumns(2);



	// variables for naming the hists
	string histtitle, histname;


	// naming the histograms for trans distance
	for( int j =0; j < 6 ; ++j){
		//Creating the histogram
		histtitle = "Transverse distance for " + particles[j][2];
		histname = "h" + particles[j][2] + "0";
		h[0][j] = new TH1D(histname.c_str(), histtitle.c_str(), 450, -50, 400.0);
			
	}


	// naming the histograms for path length
	for( int j =0; j < 6 ; ++j){
		//Creating the histogram
		histtitle = "Path Length spectra for " + particles[j][2];
		histname = "h" + particles[j][2]  + "1";
		h[1][j] = new TH1D(histname.c_str(), histtitle.c_str(), 650, -50, 600.0);;
			
	}


	// Building log binning for muon daug hists
	const Int_t nbins = 1000;
   	Double_t xmin = 1.5e-2;
   	Double_t xmax = 1.5e2;
   	Double_t logxmin = TMath::Log10(xmin);
   	Double_t logxmax = TMath::Log10(xmax);
   	Double_t binwidth = (logxmax-logxmin)/nbins;
   	Double_t xbins[nbins+1];
   	xbins[0] = xmin;
   	for (Int_t i=1;i<=nbins;i++) {
      		xbins[i] = xmin + TMath::Power(10,logxmin+i*binwidth);
   	}


	// naming the histograms for muon daug hists
	for( int j =0; j < 4 ; ++j){
		//Creating the histogram
		histtitle = "Spectrum   for " + particles[j+6][2];
		histname = "h" + particles[j+6][2];
		hm[j] = new TH1D(histname.c_str(), histtitle.c_str(),nbins,xbins);

	}


	// naming the histograms for the parents
	for( int j =0; j < 6 ; ++j){
		//Creating the histogram
		histtitle = "Parents spectra for " + particles[j][2];
		histname = "h" + particles[j][2] + "2";
		h[2][j] = new TH1D(histname.c_str(), histtitle.c_str(), 600, -50., 550.0);
	}


	// Building log binning for the muon spec
	const Int_t nbins2= 1000;
   	Double_t xmin2 = 1.5e-2;
   	Double_t xmax2 = 1.5e4;
   	Double_t logxmin2 = TMath::Log10(xmin2);
   	Double_t logxmax2 = TMath::Log10(xmax2);
   	Double_t binwidth2 = (logxmax2-logxmin2)/nbins2;
   	Double_t xbins2[nbins2+1];
   	xbins2[0] = xmin2;
   	for (Int_t i=1;i<=nbins2;i++) {
      		xbins2[i] = xmin2 + TMath::Power(10,logxmin2+i*binwidth2);
   	}


	//Creating the histogram for the muon
	hm1 = new TH1D("hspec", "Muon spectrum",nbins2,xbins2);




	// variables for the running tree
	double distance, Weight;
	map<int, int>::iterator itr2;


	// Running through the tree and filling the histograms
	for( int i =0; i < nentries; ++i){
		DataTree->GetEntry(i);

		

		for( int k =0; k < TransParticlesSize ; k++){//running through the vetors for the transverse distance

			for( int j =0; j < 6 ; ++j){	
				if( (stoi(particles[j][0]) == TransParticlesCode->at(k)) || (stoi(particles[j][1])  == TransParticlesCode->at(k)) ){ // if desired particles	
				distance = TransParticlesDistance->at(k) / 10.; // to convert mm to cm
				h[0][j]->Fill(distance);
				}
			}
		}



		for( int j =0; j < IsotopesSize ; j++){//running through the vetors for the isotopes distance
			distance = TransverseDistance->at(j) / 10.; // to convert mm to cm
			h1->Fill(distance);
			if(A->at(j) == 16 && Z->at(j) == 7){h2->Fill(distance);}//N16 - Z = 7 
			if(A->at(j) == 8 && Z->at(j) == 5){h3->Fill(distance);}//B8 - Z = 5
			// running for the iso list
			if(A->at(j) > 7){
				multiplicity[Z->at(j)-1][A->at(j)] 	+= 1; // the defult initializition is 0
				//LifeTimes[Z->at(j)-1][A->at(j)] 	= LifeTime->at(j); // getting the isosopes life time
				multiplicityEvent[Z->at(j)-1][A->at(j)] += 1; // the defult initializition is 0
				Isstable[Z->at(j)-1][A->at(j)] 		= stable->at(j);
			}
		}

		// filling the multiplicityEvents
		for( int i = 0; i < 118; i++){
    		for (itr2 = multiplicityEvent[i].begin(); itr2 != multiplicityEvent[i].end(); ++itr2) { // i = Z , itr->first = A and itr->second = quantity
			multiplicityEvents[i][itr2->first].push_back(itr2->second);
		}
		multiplicityEvent[i].clear();
		}



		for( int k =0; k < PathLengthSize ; k++){//running through the vetors for the path length

			for( int j =0; j < 6 ; ++j){
				if( (stoi(particles[j][0]) == PathLengthCode->at(k)) || (stoi(particles[j][1])  == PathLengthCode->at(k))  ){ // if desired particle		
					Weight = (StepLength->at(k)/( nentries ))/10.; // divided by ten to get the result in cm and divided by the number of muons simulated	
					h[1][j]->Fill(PathLengthEnergy->at(k), Weight); // fill with path lenght and weight 
				}
			}
 		}		



		for( int k =0; k < FirstGenSize ; k++){//running through the vetors

			for( int j =0; j < 4 ; ++j){
				if( (stoi(particles[j+6][0]) == FirstGenParticle->at(k)) || (stoi(particles[j+6][1])  == FirstGenParticle->at(k)) ){ // if desired particle		
					Weight = (1./( nentries )); // divided by the number of muons simulated to normalize	
					hm[j]->Fill(FirstGenEnergy->at(k)/1000., Weight); // divided by 1000 to convert to GeV fill with energy and weight divided by the number of events
				}
			}
		}
	
	
		hm1->Fill(Energy/1000., 1./(1.*nentries)); // divide per 1000 to convert to GeV - normalized by the size of the simulation
		meanSpec = Energy/1000. + meanSpec; // somming up for the mean
		if (Energy < 0.){ 
			cerr << "Negative Kinetic Energy Value found : " << Energy/1000. << "GeV" << endl; 
    			return 0;
		}
		hm2->Fill((Edep)/1000.);//, 15./(1.*nbinhm2)); // divide per 1000 to convert to GeV and normalize by the bin size- normalized by the size of the simulation
		meanLost = Edep/1000. + meanLost; // somming up for the mean



		for( int k =0; k < ParentsSize ; k++){//running through the vetors

			for( int j =0; j < 6 ; ++j){
				if( (stoi(particles[j][0]) == ParentsCode->at(k)) || (stoi(particles[j][1])  == ParentsCode->at(k)) ){ // if desired particle		
					Weight = (1./( nentries )); // divided by the number of muons simulated	to normalize + arbitrary normalization
					h[2][j]->Fill(ParentsEnergy->at(k), Weight); // fill with energy weight divided by the number of events
				}
			}
		}
		
		cout << "Event " << i << " has been processed." << endl;
 	}

	// Closing hist file
	inputFile->Close();



	//
	// MAKING THE TRANS DISTANCE FIGURE
	//
	// Checking if the histogram was filled for the trans distance
	for( int j =0; j < 6; ++j){
		cout << "Number of entries in the trans dist hist " << particles[j][2]  << " : " << h[0][j]->GetEntries() << endl;

			
		// Histogram attributes
   		h[0][j]->SetLineColor(colors[j]);
    		h[0][j]->SetLineWidth(3);
    		h[0][j]->SetLineStyle(2);
		h[0][j]->GetXaxis()->SetTitle("Distance to muon track  [cm]");
		h[0][j]->GetYaxis()->SetTitle("Normalized probability");
		//h[0][j]->SetTitleFont(62,"xyz"); // font option
		//h[0][j]->SetLabelFont(62,"xyz"); // font option
		h[0][j]->GetYaxis()->SetTitleOffset(1.25);
		h[0][j]->GetXaxis()->SetTitleOffset(1.25);
		h[0][j]->GetXaxis()->SetLabelSize(.0465);
		h[0][j]->GetYaxis()->SetLabelSize(.0465);
		h[0][j]->GetXaxis()->SetTitleSize(.055);
		h[0][j]->GetYaxis()->SetTitleSize(.055);
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
	gPad->SetLogy(1);
	gPad->SetMargin(0.15, 0.05, 0.15, 0.05); // (left, right, bottom, top)
	gPad->SetTickx(1); 
	gPad->SetTicky(1);
	c1->SaveAs("TrasverseDistance.png");



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
	hc->GetXaxis()->SetTitle("Isotope distance to muon track  [cm]");
	hc->GetYaxis()->SetTitle("Probability");
	hc->GetYaxis()->SetTitleOffset(1.2);
	hc->GetXaxis()->SetTitleOffset(1.25);
	hc->GetXaxis()->SetLabelSize(.0465);
	hc->GetYaxis()->SetLabelSize(.0465);
	hc->GetXaxis()->SetTitleSize(.055);
	hc->GetYaxis()->SetTitleSize(.055);
	hc->SetStats(kFALSE);
	hc->Scale(1/h1->GetEntries()); //normalizing
	hc->SetAxisRange(0., 1.,"Y");
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
    	hc->SetLineStyle(2);
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
	c1->SaveAs("TrasverseDistanceIsotopes.png");

	

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

			
		// Histogram attributes
   		h[1][j]->SetLineColor(colors[j]);
    		h[1][j]->SetLineWidth(3);
    		h[1][j]->SetLineStyle(2);
		h[1][j]->GetXaxis()->SetTitle("Energy [MeV]");
		h[1][j]->GetYaxis()->SetTitle("Path  length [cm MeV^{-1} ]");
		h[1][j]->GetYaxis()->SetTitleOffset(1.25);
		h[1][j]->GetXaxis()->SetTitleOffset(1.25);
		h[1][j]->GetXaxis()->SetLabelSize(.0465);
		h[1][j]->GetYaxis()->SetLabelSize(.0465);
		h[1][j]->GetXaxis()->SetTitleSize(.055);
		h[1][j]->GetYaxis()->SetTitleSize(.055);
		h[1][j]->SetAxisRange(0.001, 1000.,"Y");
		h[1][j]->SetAxisRange(0., 500.,"X");
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
	c1->SaveAs("PathLength.png");



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

			
		// Histogram attributes
   		hm[j]->SetLineColor(colors[j+6]);
    		hm[j]->SetLineWidth(3);
    		hm[j]->SetLineStyle(2);
		hm[j]->GetXaxis()->SetTitle("Energy [GeV]");
		hm[j]->GetYaxis()->SetTitle("dN/dlog_{10}E");
		hm[j]->GetXaxis()->SetTitleOffset(1.25);
		hm[j]->GetYaxis()->SetTitleOffset(1.1);
		hm[j]->GetXaxis()->SetLabelSize(.0465);
		hm[j]->GetYaxis()->SetLabelSize(.0465);
		hm[j]->GetXaxis()->SetTitleSize(.055);
		hm[j]->GetYaxis()->SetTitleSize(.055);
		hm[j]->SetAxisRange(0.00001, 1.,"Y");
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
	c1->SaveAs("MuonDaugthersSpec.png");



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

	
	// calculating the mean
	meanSpec = meanSpec/( nentries ); // dividing by the n of muons
	meanLost = meanLost/( nentries ); // dividing by the n of muons


	// printing the mean values
	cout << "Mean energy of the spectrum : " << meanSpec << endl;
	cout << "Mean energy loss : " << meanLost << endl;
				

	// Histograms attributes 
   	hm1->SetLineColor(4);
    	hm1->SetLineWidth(3);
    	hm1->SetLineStyle(1);
	hm1->GetXaxis()->SetTitle("Energy [GeV]");
	hm1->GetYaxis()->SetTitle("dN/dlog_{10}E_{#mu} per muon ");// [cm^{-2} s^{-1} ]");
	hm1->GetYaxis()->SetTitleOffset(1.25);
	hm1->GetXaxis()->SetTitleOffset(1.25);
	hm1->GetXaxis()->SetLabelSize(.0465);
	hm1->GetYaxis()->SetLabelSize(.0465);
	hm1->GetXaxis()->SetTitleSize(.055);
	hm1->GetYaxis()->SetTitleSize(.055);
	hm1->SetAxisRange(0.00001, 0.01,"Y");
	hm1->SetAxisRange(0.1, 10000.,"X");
	hm1->SetStats(kFALSE);
	
	hm2->SetLineColor(4);
    	hm2->SetLineWidth(3);
    	hm2->SetLineStyle(1);
	hm2->GetXaxis()->SetTitle("Muon energy loss [GeV]");
	hm2->GetYaxis()->SetTitle("Normalized probability");
	hm2->GetYaxis()->SetTitleOffset(1.25);
	hm2->GetXaxis()->SetTitleOffset(1.25);
	hm2->GetXaxis()->SetLabelSize(.0465);
	hm2->GetYaxis()->SetLabelSize(.0465);
	hm2->GetXaxis()->SetTitleSize(.055);
	hm2->GetYaxis()->SetTitleSize(.055);
	hm2->SetAxisRange(6., 20.,"X");
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
	c1->SaveAs("MuonEnergyLost.png");


	c1->Clear();
	hm1->SetTitle("");
	hm1->DrawClone("HIST C 9");


	// finilizing the figure and saving it
	gPad->SetLogx();
	gPad->SetLogy();
	c1->SaveAs("MuonSpectra.png");



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

			
		// Histogram attributes
   		h[2][j]->SetLineColor(colors[j]);
    		h[2][j]->SetLineWidth(3);
    		h[2][j]->SetLineStyle(2);
		h[2][j]->GetXaxis()->SetTitle("Energy [MeV]");
		h[2][j]->GetYaxis()->SetTitle("Number of events per muon");
		h[2][j]->GetYaxis()->SetTitleOffset(1.25);
		h[2][j]->GetXaxis()->SetTitleOffset(1.25);
		h[2][j]->GetXaxis()->SetLabelSize(.0465);
		h[2][j]->GetYaxis()->SetLabelSize(.0465);
		h[2][j]->GetXaxis()->SetTitleSize(.055);
		h[2][j]->GetYaxis()->SetTitleSize(.055);
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
	c1->SaveAs("ParentsSpectra.png");



	//
	// MAKING THE ISOTOPES LIST
	//
	cout << "Printing list in the file" << endl;
	
	// Create a new file for the list
	ofstream outfile;
  	outfile.open ("TableOfIsotopes.txt");
	outfile.precision(2);
	outfile.setf(ios::fixed);
	outfile.setf(ios::showpoint);



	// calculating the errors
	map<int, int>::iterator itr3;
	for( int i = 0; i < 118; i++){
    	for (itr3 = multiplicity[i].begin(); itr3 != multiplicity[i].end(); ++itr3) { // i = Z , itr3->first = A and itr3->second = quantity
		multiplicityEvents[i][itr3->first].push_back(itr3->second);
		for(int j =0; j < nentries ; j++){//running through events
			if( j < multiplicityEvents[i][itr3->first].size() ){
			multiplicityError[i][itr3->first] += (
(multiplicityEvents[i][itr3->first].at(j)-((itr3->second*1.)/(1.*( nentries ))))*
(multiplicityEvents[i][itr3->first].at(j)-((itr3->second*1.)/(1.*( nentries )))
) / ( ( nentries ) -1.) );
			}
			else{
				multiplicityError[i][itr3->first] += (((itr3->second*1.)/(1.*( nentries )))*
((itr3->second*1.)/(1.*( nentries )))
) / ( ( nentries ) -1.) ;
			}
		}
	}
	}



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
			arrays->push_back(sqrt(multiplicityError[i][itr->first])/sqrt(nentries)); //id 3
			//arrays->push_back(LifeTimes[i][itr->first]); //id 4
			if ( Isstable[i][itr->first] ){
				cout << elements[i] << itr->first << " is " << Isstable[i][itr->first] <<" and has " << itr->second << " \\pm " << multiplicityError[i][itr->first] << " with a life time of " << LifeTimes[i][itr->first] << endl ;
				stables->push_back(arrays);
			}
			else{
				cout << elements[i] << itr->first << " is " << Isstable[i][itr->first] <<" and has " << itr->second << " \\pm " << multiplicityError[i][itr->first] << " with a life time of " << LifeTimes[i][itr->first] << endl ;
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
             	<< '\t' << '&' // << '\t' << stables->at(location)->at(4) * 1000000000. << '\t' << '&'
		<< '\t' << max/(3200.* ( nentries )) * 10000000. << '\t' << '&' << '\t' << " \\pm "
		<< stables->at(location)->at(3)/(3200.) * 10000000.  << '\t' << "\\\\" <<'\n';  // dividing per the number of muons, the distance of the muon track and by the water density 1 * g * cm^{-3} and multiplyin by 10^{7} - time multiplied by e9 because in nanosecs
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
             	<< '\t' << '&' // << '\t' << unstables->at(location)->at(4) * 1000000000. << '\t' << '&'
		<< '\t' << max/(3200.* ( nentries )) * 10000000. << '\t' << '&' << '\t' << " \\pm " << unstables->at(location)->at(3)/(3200.) * 10000000.  << '\t' << "\\\\" <<'\n';  // dividing per the number of muons, the distance of the muon track and by the water density 1 * g * cm^{-3} and multiplyin by 10^{7} - time multiplied by e9 because in nanosecs
		unstables->erase (unstables->begin()+location); //removing the maximum from the vector
		max = -1;
	}


	// Closing hist file
	outfile.close();



	
	//outputFile->Close();

	return 1;

}
