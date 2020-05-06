// Fill an n-tuple with the data from a text file and write it to a file

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
using namespace std;


//function used to split the line so we can take the data and put in the vector
void split(const string& s, char c, vector<string>& v) {
   int i = 0;
   int j = s.find(c);

   while (j >= 0) {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);

      if (j < 0) {
         v.push_back(s.substr(i, s.length()));
      }
   }
}


void ntuple_maker_particles(){ // change the eDep in the code to be your <scoring_attribute>

   gROOT->Reset();


   // create a new ROOT file
   TFile *f = new TFile("part_spec_run2.root","RECREATE");
   // Initialise the TNtuple
   TNtuple part_spec_data("part_spec_data","part_spec simulated data","energy:number");

   //create variables to containg the data
   double E, n, tot;


   // open the ASCII file
   ifstream fp("particles_spec_run2.txt");

 
   //  Fill the ntuple with the data from the ASCII file

	vector<string>* p = NULL;
   	string tmp;

	cout << endl << "Started" ;

	getline(fp, tmp, '\n');
	getline(fp, tmp, '\n');
	getline(fp, tmp, '\n');

	cout << endl << "Jumped comments" << endl << endl ;

	// getting the total number of particles
	p = new vector<string>();
	getline(fp, tmp, '\n');                    // Grab the next line
	split(tmp, ',', *p);     
	//convert the string to numbers with the atof
	tot = atof(p->at(3).c_str());

   	while (!fp.eof()) {
		//first we break the data into strings with the right information
      		getline(fp, tmp, '\n');                    // Grab the next line

		p = new vector<string>();

		//cout << endl << tmp ;

		
		if (tmp.compare("") != 0){ //avoid having one last loop at the end of the file

			//grab the line with the energy
			split(tmp, '_', *p); 

			E = atof(p->at(1).c_str());
			
			//jumping comments
			getline(fp, tmp, '\n');                    // Grab the next line

			// cout << endl << "Jumped comments" << endl << endl ;

			//grab the line with the data
			getline(fp, tmp, '\n');                    // Grab the next line 

			p = new vector<string>();

      			split(tmp, ',', *p);     

			//cout << endl << tmp << "we enter a the data collection" ;      

			//convert the string to numbers with the atof
			n = atof(p->at(3).c_str());

			//debug things
			//cout << endl << tmp << endl;
			//for ( int i = 0; i < p->size(); i++){
			//cout << atof(p->at(i).c_str()) << ' ';
			//}
			//cout << endl;

			n = n/tot;

			part_spec_data.Fill(E,n);
			
		}

      		tmp.clear();
	}
	// Save the ntuple and close the file
	//fclose(fp);
	//eDep_data.Write();
	f->Write();
	//     ofile.Close();
}




