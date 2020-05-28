// Read the tree and make a pathlenght histo plot

#include "TCanvas.h"
#include "TROOT.h"
#include "TH1.h"
#include "TTree.h"
#include "TFile.h"

void making_pathlenght_histo(){
	// Open a file, get the tree and clone it
	TFile tree_file("SK.root");
	TTree* DataTree;
	tree_file.GetObject("particles_data",DataTree);
	TTree* DataTreeClone = (TTree*)DataTree->Clone();

	// creating variables for the branches
	Char_t Particle [25]; // weird particle type Chart_t so we have to make an array big enought to contain the name
	double Energy, PathLenght;
	int EventID;
	// Attaching to branches
	DataTreeClone->SetBranchAddress("Particle",&Particle);
	DataTreeClone->SetBranchAddress("Energy",&Energy);
	DataTreeClone->SetBranchAddress("PathLenght",&PathLenght);
	DataTreeClone->SetBranchAddress("EventID",&EventID);

	// selecting the particles
	string particlesname1 = "e-";
	string particlesname2 = "e+";
	//TCut eminus = "particlesname1.compare(Particle) == 0";
	//TCut eplus  = "particlesname2.compare(Particle) == 0";
	//TCut particles =  eminus || eplus;

	//Creating the histogram
	TH1* h1 = new TH1D("h1", "PathLenght", 500, 0.0, 500.0);

	// Create a new file
	string filename = "PathLength";
	filename.append( "_" );// 
	filename.append(particlesname1);
	filename.append( "_" );
	filename.append(particlesname2);
	filename.append( ".root" ); // weird proble;s with the file name
	TFile hist_file("PathLength.root", "recreate"); 

	// Running through the tree and filling the histogram
	int nentries = (int)DataTreeClone->GetEntries();
	for( int i =0; i < 100; ++i){
	DataTreeClone->GetEntry(i);
	if( (particlesname1.compare(Particle) == 0) || (particlesname2.compare(Particle) == 0) ){ // if desired particle		
		h1->Fill(Energy, PathLenght);} // fill with path lenght weight devided by the number of events
 	}

	// Making the canvas we will draw in
	TCanvas *c1 = new TCanvas();
	// Adding histogram attributes and drawing it
   	h1->SetLineColor(1);
    	h1->SetLineWidth(2);
	h1->Draw("HIST"); // weird problems with the drawing

	// Saving the histogram	
	h1->Write();

	// Closing hist file
	tree_file.Close();
	hist_file.Close();

}

int main(){
    making_pathlenght_histo();
    }
