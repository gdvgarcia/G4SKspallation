// Read the tree and make transverse distance histos plot

#include "TCanvas.h"
#include "TROOT.h"
#include "TH1.h"
#include "TTree.h"
#include "TFile.h"
#include "TLegend.h"

#include <iostream> 
#include <vector> 

int merging(){


	// Opening the file1 with the tree
	TFile* inputFile1 = new TFile("SK1.root");
	// checking if the file is open correctly
	if (inputFile1->IsOpen()) {
    		cout << "Reading file: " << inputFile1->GetName() << endl;
  	}
  	else {
    		cerr << "Error opening input file!" << endl;
    		return 0;
  	}


	// Getting the tree2
	TTree* DataTree1;
	inputFile1->GetObject("event_data",DataTree1);
	int nentries1 = (int)DataTree1->GetEntries();
	cout << "Number of entries in the Tree1: " << nentries1 << endl; // remember that the try has one entry per event so nentries = n events
	if( nentries1 < 1 ){
		cerr << "No data to analyze." << endl;
		return 0;
	}


	// Opening the file2 with the tree
	TFile* inputFile2 = new TFile("SK.root");
	// checking if the file is open correctly
	if (inputFile2->IsOpen()) {
    		cout << "Reading file: " << inputFile2->GetName() << endl;
  	}
  	else {
    		cerr << "Error opening input file!" << endl;
    		return 0;
  	}


	// Getting the tree2
	TTree* DataTree2;
	inputFile2->GetObject("event_data",DataTree2);
	int nentries2 = (int)DataTree2->GetEntries();
	cout << "Number of entries in the Tree2: " << nentries2 << endl; // remember that the try has one entry per event so nentries = n events
	if( nentries2 < 1 ){
		cerr << "No data to analyze." << endl;
		return 0;
	}

	
	// Create a new file where we gonna save the tree
	string filename = "merged.root";
	TFile *outputFile = new TFile(filename.c_str(),"RECREATE");
  	if (outputFile->IsOpen()) {
    		cout << "Everything is fine!" << endl;
  	}
  	else {
    		cerr << "That should not happen." << endl;
		return 0;
  	}


	// list used to merge trees
  	TList *list = new TList;
	cout << "List created." << endl;

	// adding trees to the list  
	list->Add(DataTree1);
	cout << "First tree added." << endl;
  	list->Add(DataTree2);
	cout << "Second tree added." << endl;

	// mergin trees
  	TTree *event_data_merged = TTree::MergeTrees(list);
  	event_data_merged->SetName("event_data_merged");
	cout << "Trees merged." << endl;
	
	// saving the merged tree
  	event_data_merged->Write();
	cout << "Merged tree saved." << endl;



	// Closing hist file
	inputFile1->Close();
	inputFile2->Close();

	return 1;

}
