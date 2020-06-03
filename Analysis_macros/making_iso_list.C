// Read the tree and make transverse distance histos plot

#include "TCanvas.h"
#include "TROOT.h"
#include "TH1.h"
#include "TTree.h"
#include "TFile.h"
#include "TLegend.h"
#include <iostream>
#include <fstream>
#include <map>
using namespace std;

int making_iso_list(){
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
	inputFile->GetObject("isotopes_data",DataTree);
	int nentries = (int)DataTree->GetEntries();
	cout << "Number of entries in the Tree: " << nentries << endl;
	if( nentries < 1 ){
		cerr << "No data to analyze." << endl;
		return 0;
	}

	// creating variables for the branches
	int EventID, A, Z;
	// Attaching to branches
	DataTree->SetBranchAddress("A",&A);
	DataTree->SetBranchAddress("Z",&Z);
	DataTree->SetBranchAddress("EventID",&EventID);

	// create container for particles isotopes
	string elements [118] = {"H","He","Li","Be","B","C","N","O","F","Ne","Na","Mg","Al","Si","P","S","Cl","Ar","K","Ca","Sc","Ti","V","Cr","Mn","Fe","Co","Ni","Cu","Zn","Ga","Ge","As","Se","Br","Kr","Rb","Sr","Y","Zr","Nb","Mo","Tc","Ru","Rh","Pd","Ag","Cd","In","Sn","Sb","Te","I","Xe","Cs","Ba","La","Ce","Pr","Nd","Pm","Sm","Eu","Gd","Tb","Dy","Ho","Er","Tm","Yb","Lu","Hf","Ta","W","Re","Os","Ir","Pt","Au","Hg","Tl","Pb","Bi","Po","At","Rn","Fr","Ra","Ac","Th","Pa","U","Np","Pu","Am","Cm","Bk","Cf","Es","Fm","Md","No","Lr","Rf","Db","Sg","Bh","Hs","Mt","Ds","Rg","Uub","Uut","Uuq","Uup","Uuh","Uus","Uuo"};

	// create the data collector
	map<int, bool> appeared [118];
	map<int, int > multiplicity [118];	

	// Create a new file
	ofstream outfile;
  	outfile.open ("TableOfIsotopes.txt");

	// Getting the number of muons simulated
	DataTree->GetEntry( nentries - 1 );
	int nmuons = EventID;
	cout << "Number of muons simulated: " << nmuons << endl;


	// Running through the tree and filling the histogram
	for( int i =0; i < nentries; ++i){
		DataTree->GetEntry(i);
		if(A>7){
			appeared[Z-1][A] = true;
			if(appeared[Z-1][A]){
				multiplicity[Z-1][A] += 1;
			}
			else{
				multiplicity[Z-1][A] = 1;
			}
		}
	}


	// print all elements of the map 
    	outfile << "Isotope \t Quantity mu^{-1} g^{-1} cm^{2} x 10^{-7} \n"; 
	map<int, int>::iterator itr;
	for( int i = 0; i < 118; i++){
    		for (itr = multiplicity[i].begin(); itr != multiplicity[i].end(); ++itr) { 
        	outfile << '\t' << elements[i] << itr->first
             	<< '\t' << itr->second/(3200.* nmuons) * 10000000 << '\n';  // dividing per the number of muons, the distance of the muon track and by the water density 1 * g * cm^{-3} and multiplyin by 10^{7}
		}
	}

	// Closing hist file
	outfile.close();
	inputFile->Close();

	return 1;

}
