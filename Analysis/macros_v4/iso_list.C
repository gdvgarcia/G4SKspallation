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

int iso_list(){
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
	vector<int>* A =0; 
	vector<int>* Z =0;
	int IsotopesSize;

	// brench to attach
	TBranch *bIso = 0;


	// Attaching to branches
	DataTree->SetBranchAddress("IsotopesSize",&IsotopesSize);
	DataTree->SetBranchAddress("A",&A, &bIso);
	DataTree->SetBranchAddress("Z",&Z, &bIso);


	// create container for particles isotopes
	string elements [118] = {"H","He","Li","Be","B","C","N","O","F","Ne","Na","Mg","Al","Si","P","S","Cl","Ar","K","Ca","Sc","Ti","V","Cr","Mn","Fe","Co","Ni","Cu","Zn","Ga","Ge","As","Se","Br","Kr","Rb","Sr","Y","Zr","Nb","Mo","Tc","Ru","Rh","Pd","Ag","Cd","In","Sn","Sb","Te","I","Xe","Cs","Ba","La","Ce","Pr","Nd","Pm","Sm","Eu","Gd","Tb","Dy","Ho","Er","Tm","Yb","Lu","Hf","Ta","W","Re","Os","Ir","Pt","Au","Hg","Tl","Pb","Bi","Po","At","Rn","Fr","Ra","Ac","Th","Pa","U","Np","Pu","Am","Cm","Bk","Cf","Es","Fm","Md","No","Lr","Rf","Db","Sg","Bh","Hs","Mt","Ds","Rg","Uub","Uut","Uuq","Uup","Uuh","Uus","Uuo"};

	// create the data collector
	map<int, bool> appeared [118];
	map<int, int > multiplicity [118];	

	// Create a new file
	ofstream outfile;
  	outfile.open ("TableOfIsotopes.txt");

	// Getting the number of muons simulated
	cout << "Number of muons simulated: " << nentries << endl;


	// Running through the tree and filling the histogram
	for( int i =0; i < nentries; ++i){
		DataTree->GetEntry(i);
		// getting the branch i
		Long64_t tentry = DataTree->LoadTree(i);
		bIso->GetEntry(tentry);

		for( int j =0; j < IsotopesSize ; j++){//running through the vetors
			if(A->at(j) > 7){
				appeared[Z->at(j)-1][A->at(j)] = true;
				if(appeared[Z->at(j)-1][A->at(j)]){
					multiplicity[Z->at(j)-1][A->at(j)] += 1;
				}
				else{
					multiplicity[Z->at(j)-1][A->at(j)] = 1;
				}
			}
		}
	}


	// print all elements of the map 
    	outfile << "Isotope \t Quantity mu^{-1} g^{-1} cm^{2} x 10^{-7} \n"; 
	map<int, int>::iterator itr;
	for( int i = 0; i < 118; i++){
    		for (itr = multiplicity[i].begin(); itr != multiplicity[i].end(); ++itr) { 
        	outfile << '\t' << elements[i] << itr->first
             	<< '\t' << itr->second/(3200.* nentries) * 10000000 << '\n';  // dividing per the number of muons, the distance of the muon track and by the water density 1 * g * cm^{-3} and multiplyin by 10^{7}
		}
	}

	// Closing hist file
	outfile.close();
	inputFile->Close();

	return 1;

}
