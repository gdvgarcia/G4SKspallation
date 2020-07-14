// Read a tree from a given root file - passed by argument - into histograms and a ntuple for isotopes list

// how to call 
// root -l -q "histos_tree.C(\"/home/user/path_to_file/filename.root\")"

#include "TROOT.h"
#include "TH1.h"
#include "TTree.h"
#include "TFile.h"


#include <iostream> 
#include <list>
#include <vector>
#include <fstream>

int histos_tree(char argv  []){ // the argument should be the full_path_to_the_root_file

	// getting the full_path_to_the_root_file string
	std::string inputName = argv;
	cout << "This will read histograms from the file named : " << inputName << endl;

	// Opening the file with the tree
	TFile* inputFile = new TFile(inputName.c_str());
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

	
	// Create a new file where we gonna save the histograms
	// cutting the name in '.' to remove the .root
	char divisionchart = '.'; 
	vector<string> words;
	int i = 0;
   	int j = inputName.find(divisionchart);

   	while (j >= 0) {
      		words.push_back(inputName.substr(i, j-i));
      		i = ++j;
      		j = inputName.find(divisionchart, j);

      		if (j < 0) {
         		words.push_back(inputName.substr(i, inputName.length()));
		}
	}

	
	std::cout << "Out file name = " << words.at(0) << "_histos_tree.root" << std::endl;
	
	// Create a new file for the output
	std::string outFilename =  words.at(0) + "_histos_tree.root";
	TFile *outputFile = new TFile(outFilename.c_str(),"RECREATE");
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
	TH1* h1 = new TH1D("TransverseDistanceIsotopes", "Transverse Distance for all the isotopes", 40000,  -50., 35000.0);
	TH1* h2 = new TH1D("TransverseDistanceNitrogen", "Transverse Distance for N16", 40000, -50., 35000.0);
	TH1* h3 = new TH1D("TransverseDistanceBoron", "Transverse Distance for B8", 40000, -50., 35000.0);
	//Declaring the histogram for muons daughters
	TH1* hm[4];



	// variables for naming the hists
	string histtitle, histname;


	// naming the histograms for trans distance
	for( int j =0; j < 6 ; ++j){
		//Creating the histogram
		histtitle = "Transverse distance for " + particles[j][2];
		histname = "trans_h" + particles[j][2];
		h[0][j] = new TH1D(histname.c_str(), histtitle.c_str(), 45000, -50, 40000.0);
			
	}


	// naming the histograms for path length
	for( int j =0; j < 6 ; ++j){
		//Creating the histogram
		histtitle = "Path Length spectra for " + particles[j][2];
		histname = "path_h" + particles[j][2];
		h[1][j] = new TH1D(histname.c_str(), histtitle.c_str(), 60000, 2.5, 60200.5);;
			
	}


	// Building log binning for muon daug hists
	const Int_t nbins = 1000000;
   	Double_t xmin = 1.5e-3;
   	Double_t xmax = 1.5e4;
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
		histname = "daug_h" + particles[j+6][2];
		hm[j] = new TH1D(histname.c_str(), histtitle.c_str(),nbins,xbins);

	}


	// naming the histograms for the parents
	for( int j =0; j < 6 ; ++j){
		//Creating the histogram
		histtitle = "Parents spectra for " + particles[j][2];
		histname = "parents_h" + particles[j][2];
		h[2][j] = new TH1D(histname.c_str(), histtitle.c_str(), 60000, -50., 55000.0);
	}


	// tree to collect initial muons data and isotopes
	TTree* MuonT = new TTree("MuonT_iso","Tree for initial muon data and isotopes");
	double mEnergy, mEdep;
	std::vector<int> mA;
	std::vector<int> mZ;
	std::vector<int> mStable;
	MuonT->Branch("Energy", &mEnergy, "Energy/D");
	MuonT->Branch("Edep", &mEdep, "Edep/D");
	MuonT->Branch("A", "std::vector<int>", &mA);
	MuonT->Branch("Z", "std::vector<int>", &mZ);
	MuonT->Branch("Stable", "std::vector<int>", &mStable);



	// variables for the running tree
	double distance, Weight;

	// Running through the tree and filling the histograms
	for( int i =0; i < nentries; ++i){
		// getting data from the try for the next event
		DataTree->GetEntry(i);

		// clear vectors for each event
		mA.clear(); mZ.clear();
		

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
			if(A->at(j) == 16 && Z->at(j) == 7){h2->Fill(distance);}	// N16 - Z = 7 
			if(A->at(j) == 8 && Z->at(j) == 5){h3->Fill(distance);}		// B8 - Z = 5
			// running for the iso list
			if(A->at(j) > 7){
				mA.push_back(A->at(j));
				mZ.push_back(Z->at(j));
				mStable.push_back(stable->at(j));
			}
		}




		for( int k =0; k < PathLengthSize ; k++){//running through the vetors for the path length

			for( int j =0; j < 6 ; ++j){
				if( (stoi(particles[j][0]) == PathLengthCode->at(k)) || (stoi(particles[j][1])  == PathLengthCode->at(k))  ){ // if desired particle		
					Weight = (StepLength->at(k))/10.; // divided by ten to get the result in cm and in the other macro it should be divided by the number of muons simulated	
					h[1][j]->Fill(PathLengthEnergy->at(k), Weight); // fill with path lenght and weight 
				}
			}
 		}		



		for( int k =0; k < FirstGenSize ; k++){//running through the vetors

			for( int j =0; j < 4 ; ++j){
				if( (stoi(particles[j+6][0]) == FirstGenParticle->at(k)) || (stoi(particles[j+6][1])  == FirstGenParticle->at(k)) ){ // if desired particle		
					hm[j]->Fill(FirstGenEnergy->at(k)/1000., Weight); // divided by 1000 to convert to GeV fill with energy and remember to normalize in the other macro divided by the number of muons simulated to normalize	
				}
			}
		}
	
	
		mEnergy = Energy/1000.; // divide per 1000 to convert to GeV - remember to normalized by the size of the simulation in the other macro
		if (Energy < 0.){ 
			cerr << "Negative Kinetic Energy Value found : " << Energy/1000. << "GeV" << endl; 
    			return 0;
		}
		mEdep = Edep/1000.;	// divide per 1000 to convert to GeV


		for( int k =0; k < ParentsSize ; k++){//running through the vetors

			for( int j =0; j < 6 ; ++j){
				if( (stoi(particles[j][0]) == ParentsCode->at(k)) || (stoi(particles[j][1])  == ParentsCode->at(k)) ){ // if desired particle		
					h[2][j]->Fill(ParentsEnergy->at(k)); // remember to divided by the number of muons simulated to normalize in the other macro
				}
			}
		}

		MuonT->Fill();
		
		cout << "Event " << i << " has been processed." << endl;
 	}

	// Closing initial tree file
	inputFile->Close();
	cout << "Data taken from the input file. Input file closed and now saving hists and trees in the output file." << endl;


	outputFile->Write();
	outputFile->Close();

	return 1;

}
