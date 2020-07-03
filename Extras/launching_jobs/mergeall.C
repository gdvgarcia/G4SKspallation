// merges all files in the given directory

// how to call 
// root -l -q "mergeall.C(\"/home/ggarcia/Analysis/many_small\")"

#include "TChain.h"
#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"

#include <iostream> 
#include <list>
#include <vector>
#include <fstream>

int mergeall(char argv  []){

	// getting the directory name
	std::string dirName = argv;
	dirName = dirName + "/*.root";
	cout << "Merging all tree files in " << dirName << " ." << endl;

	// making the TChain object which merges all the trees
	TChain *myChain = new TChain("event_data");
	myChain->Add(dirName.c_str());
	cout << "Trees merged. Final tree has "<< myChain->GetEntries() << " entries." << endl;


	
	// Create a new file where we gonna save the tree
	string filename = "fulldata.root";
	// marging tree and saving it
	myChain->Merge(filename.c_str());


	return 1;
}
