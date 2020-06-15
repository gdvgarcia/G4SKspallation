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
#include <vector> 
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
	vector<int>*	A 	= new vector<int>();
	vector<int>* 	Z 	= new vector<int>();
	vector<bool>* 	stable 	= new vector<bool>();
	int IsotopesSize;


	// Attaching to branches
	DataTree->SetBranchAddress("IsotopesSize",&IsotopesSize);
	DataTree->SetBranchAddress("A",&A);
	DataTree->SetBranchAddress("Z",&Z);
	DataTree->SetBranchAddress("Stable1Instable0",&stable);


	// create container for particles isotopes
	string elements [118] = {"H","He","Li","Be","B","C","N","O","F","Ne","Na","Mg","Al","Si","P","S","Cl","Ar","K","Ca","Sc","Ti","V","Cr","Mn","Fe","Co","Ni","Cu","Zn","Ga","Ge","As","Se","Br","Kr","Rb","Sr","Y","Zr","Nb","Mo","Tc","Ru","Rh","Pd","Ag","Cd","In","Sn","Sb","Te","I","Xe","Cs","Ba","La","Ce","Pr","Nd","Pm","Sm","Eu","Gd","Tb","Dy","Ho","Er","Tm","Yb","Lu","Hf","Ta","W","Re","Os","Ir","Pt","Au","Hg","Tl","Pb","Bi","Po","At","Rn","Fr","Ra","Ac","Th","Pa","U","Np","Pu","Am","Cm","Bk","Cf","Es","Fm","Md","No","Lr","Rf","Db","Sg","Bh","Hs","Mt","Ds","Rg","Uub","Uut","Uuq","Uup","Uuh","Uus","Uuo"};

	// create the data collector
	map<int, int > multiplicity [118];
	map<int, bool> Isstable [118];	

	// Create a new file
	ofstream outfile;
  	outfile.open ("TableOfIsotopes.txt");
	outfile.precision(2);
	outfile.setf(ios::fixed);
	outfile.setf(ios::showpoint);

	// Getting the number of muons simulated
	cout << "Number of muons simulated: " << nentries << endl;


	// Running through the tree and filling the histogram
	for( int i =0; i < nentries; ++i){
		DataTree->GetEntry(i);

		for( int j =0; j < IsotopesSize ; j++){//running through the vetors
			if(A->at(j) > 7){
				multiplicity[Z->at(j)-1][A->at(j)] += 1; // the defult initializition is 0
				Isstable[Z->at(j)-1][A->at(j)] = stable->at(j);
			}
		}
	}

	// vectors to fix the order of print
	std::vector<std::vector<int>* > * stables 	= new std::vector<std::vector<int>*  >;
	std::vector<std::vector<int>* > * unstables 	= new std::vector<std::vector<int>*  >;

	map<int, int>::iterator itr;
	std::vector<int>* arrays = new std::vector<int>;
	for( int i = 0; i < 118; i++){
    		for (itr = multiplicity[i].begin(); itr != multiplicity[i].end(); ++itr) { // i = Z , itr->first = A and itr->second = quantity
			arrays = new std::vector<int>;
			arrays->push_back(i);
			arrays->push_back(itr->first);
			arrays->push_back(itr->second);
			if ( Isstable[i][itr->first] ){
				cout << elements[i] << itr->first << " is " << Isstable[i][itr->first] <<" and has " << itr->second << endl ;
				stables->push_back(arrays);
			}
			else{
				cout << elements[i] << itr->first << " is " << Isstable[i][itr->first] <<" and has " << itr->second << endl ;
				unstables->push_back(arrays);
			}
		}
	}

	


	// print stable isotopes in decreasing order
	int location =-1;
	int max = -1;
    	outfile << "\t Stable isotopes \n"; 
    	outfile << "Isotope \t Quantity mu^{-1} g^{-1} cm^{2} x 10^{-7} \n";
	int size = stables->size();
	for( int i = 0; i < size; i++){
    		for ( int j = 0 ; j < stables->size(); j++){
    			if ( stables->at(j)->at(2) > max ){
				max = stables->at(j)->at(2);
				location = j;
			}
		}
		outfile << '\t' << "$^{"  << unstables->at(location)->at(1) << "}$" << elements[unstables->at(location)->at(0)]
             	<< '\t' << '&' << '\t' << max/(3200.* nentries) * 10000000. << '\t' << "\\\\" <<'\n';  // dividing per the number of muons, the distance of the muon track and by the water density 1 * g * cm^{-3} and multiplyin by 10^{7}
		stables->erase (stables->begin()+location); //removing the maximum from the vector
		max = -1;
	}

	outfile << '\n' << '\n' << '\n';

	// print unstable isotopes in decreasing order
    	outfile << "\t Unstable isotopes \n"; 
    	outfile << "Isotope \t Quantity mu^{-1} g^{-1} cm^{2} x 10^{-7} \n"; 
	size = unstables->size();
	for( int i = 0; i < size; i++){
    		for ( int j = 0 ; j < unstables->size(); j++){
    			if ( unstables->at(j)->at(2) > max ){
				max = unstables->at(j)->at(2);
				location = j;
			}
		}
		outfile << '\t' << "$^{"  << unstables->at(location)->at(1) << "}$" << elements[unstables->at(location)->at(0)]
             	<< '\t' << '&' << '\t' << max/(3200.* nentries) * 10000000. << '\t' << "\\\\" <<'\n';  // dividing per the number of muons, the distance of the muon track and by the water density 1 * g * cm^{-3} and multiplyin by 10^{7}
		unstables->erase (unstables->begin()+location); //removing the maximum from the vector
		max = -1;
	}

	// Closing hist file
	outfile.close();
	inputFile->Close();

	return 1;

}
