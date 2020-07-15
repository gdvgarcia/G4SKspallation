// Create a file for the muon and isotope tree and remove the tree so we have only histograms

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


#include <string>
#include "TFile.h"


int remove_tree(char argv  []){ // the argument should be the full_path_to_the_root_file

	// getting the full_path_to_the_root_file string
	std::string inputName = argv;
	cout << "This will read histograms from the file named : " << inputName << endl;

	// Opening the file with the tree
	TFile* inputFile = new TFile(inputName.c_str(),"update");
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
	inputFile->GetObject("MuonT_iso",DataTree);
	int nentries = (int)DataTree->GetEntries();
	cout << "Number of entries in the Tree: " << nentries << endl; // remember that the try has one entry per event so nentries = n events
	if( nentries < 1 ){
		cerr << "No data to analyze." << endl;
		return 0;
	}

  	std::string object_to_remove="MuonT_iso;*";
  	//the object can be a tree, a histogram, etc, in this case "MuonT_iso" is a TTree


	vector<int>* 	A 			= new vector<int>();
	vector<int>* 	Z 			= new vector<int>();
	vector<bool>* 	stable 	= new vector<bool>(); // for the iso list
	double		Energy, Edep;


	DataTree->SetBranchAddress("A",&A);
	DataTree->SetBranchAddress("Z",&Z);
	DataTree->SetBranchAddress("Stable",&stable);
	DataTree->SetBranchAddress("Energy",&Energy);
	DataTree->SetBranchAddress("Edep",&Edep);


/DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
think what to do with the isotopes


	//Declaring the histograms for the muon
	TH1* hm1;
	int nbinhm2 = 100; // number of bins for the energy loss
	TH1* hm2 = new TH1D("hdep", "Muon energy loss", nbinhm2, 0, 50);

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


	// create the data collector for isotopes list
	std::map<int, int > multiplicity [118]; // for the total
	std::map<int, bool> Isstable [118];



	// variables for the running tree
	double distance, Weight;

	// variables for the running tree
	std::map<int, int>::iterator itr2;


	std::cout << "Out file name = " << words.at(0) << "_isotree.root" << std::endl;
	
	// Create a new file for the output
	std::string outFilename =  words.at(0) + "_isotree.root";
	TFile *outputFile = new TFile(outFilename.c_str(),"RECREATE");
  	if (outputFile->IsOpen()) {
    		cout << "Everything is fine!" << endl;
  	}
  	else {
    		cerr << "That should not happen." << endl;
		return 0;
  	}


	// tree to collect isotopes data
	TTree* IsoT = new TTree("iso_list","Tree for the isotopes");
	int mQuantity;
	int mA;
	int mZ;
	int mStable;
	IsoT->Branch("Quantity", &mQuantity, "Quantity/I");
	IsoT->Branch("A", &mA, "A/I");
	IsoT->Branch("Z", &mZ, "Z/I");
	IsoT->Branch("Stable", &mStable, "Stable/I");

	// tree to collect muon data
	TTree* MuonT = new TTree("muon_mean","Tree for mean infor from muon");
	double mEnergy, mEdep;
	MuonT->Branch("meanEnergy", &mEnergy, "meanEnergy/D");
	MuonT->Branch("meanEdep", &mEdep, "meanEdep/D");


	// Running through the tree and filling the histograms
	for( int i =0; i < nentries; ++i){
		DataTree->GetEntry(i);


		for( int j =0; j < A->size(); j++){//running through the vetors for the isotopes distance
			// running for the iso list
			if(A->at(j) > 7){
				multiplicity[Z->at(j)-1][A->at(j)] 	+= 1; // the defult initializition is 0
				Isstable[Z->at(j)-1][A->at(j)] 		= stable->at(j);
			}
		}
	
	
		hm1->Fill(Energy, 1./(1.*nentries)); //  normalized by the size of the simulation
		meanSpec = Energy + meanSpec; // somming up for the mean
		if (Energy < 0.){ 
			cerr << "Negative Kinetic Energy Value found : " << Energy/1000. << "GeV" << endl; 
    			return 0;
		}
		hm2->Fill((Edep));//, 15./(1.*nbinhm2)); // normalize by the bin size- normalized by the size of the simulation
		meanLost = Edep + meanLost; // somming up for the mean

		
		cout << "Event " << i << " has been processed." << endl;
 	}


	// making iso tree
	map<int, int>::iterator itr;
	for( int i = 0; i < 118; i++){
    		for (itr = multiplicity[i].begin(); itr != multiplicity[i].end(); ++itr) { // i = Z , itr->first = A and itr->second = quantity
			mZ = i; //id 0
			mA = itr->first; //id 1
			mQuantity = itr->second; //id 2
			mStable = Isstable[i][itr->first];	
			IsoT->Fill();
		}
	}

	// calculating the mean
	mEnergy = meanSpec/( nentries ); // dividing by the n of muons
	mEdep = meanLost/( nentries ); // dividing by the n of muons
	MuonT->Fill();

	// printing the mean values
	cout << "Mean energy of the spectrum : " << meanSpec << endl;
	cout << "Std dev energy of the spectrum : " << stddevSpec << endl;



	// saving the tree in the file
	cout << "Data taken from the input file. Input file closed and now saving the tree in the output file." << endl;
	MuonT->Write();
	IsoT->Write();
	outputFile->Close();

	// deleting tree
  	//if your object is not at the top directory, but in a directory in the .root file, called foo
  	// you do first
  	//file->cd("foo");
  	//then continue with the Delete command which is only applied to the current gDirectory
  	gDirectory->Delete(object_to_remove.c_str());

	// saving histos
	hm1->Write();
	hm1->Write();
	// Closing initial tree file
	inputFile->Close();



	return 1;

}
