// merges all files in the given directory

// how to call 
// root -l -q "mergeall.C(\"/home/user/path_to_root_files_directory\")"

#include "TChain.h"
#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"

#include <iostream> 
#include <list>
#include <vector>
#include <fstream>

int mergeall(char argv  []){

	// setting tree big
	TTree::SetMaxTreeSize( 10000000000000LL ); // 10 TB
	cout << "Maximum size of the tree is " << TTree::GetMaxTreeSize()/1000000000. << "Gb." << endl;

	// getting the directory name
	std::string dirName = argv;
	dirName = dirName + "/*.root";
	cout << "Merging all tree files in " << dirName << " ." << endl;

	// making the TChain object which merges all the trees
	TChain *myChain = new TChain("event_data");
	myChain->Add(dirName.c_str());
	cout << "Trees merged. Final tree has "<< myChain->GetEntries() << " entries." << endl;

	
	// Create a new file where we gonna save the tree
	string filename = "alldata2.root";
	// marging tree and saving it
	cout << "Creating a file named " << filename << " for the merged tree.";
	myChain->Merge(filename.c_str());


	return 1;
}
