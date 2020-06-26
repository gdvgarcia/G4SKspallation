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


void ntuple_maker(){ // change the eDep in the code to be your <scoring_attribute>

   gROOT->Reset();


   // create a new ROOT file
   TFile *f = new TFile("eDep_run1.root","RECREATE");
   // Initialise the TNtuple
   TNtuple eDep_data("eDep_data","eDep simulated data","X_position:Y_position:Z_position:dE_value:entry");

   //create variables to containg the data
   double x, y,z,E, entry;


   // open the ASCII file
   ifstream fp("eDep_run1.txt");

 
   //  Fill the ntuple with the data from the ASCII file

	vector<string>* p = NULL;
   	string tmp;

	cout << endl << "Started" ;

	getline(fp, tmp, '\n');
	getline(fp, tmp, '\n');
	getline(fp, tmp, '\n');

	cout << endl << "Jumped comments" << endl << endl ;


   	while (!fp.eof()) {
		//first we break the data into strings with the right information
      		getline(fp, tmp, '\n');                    // Grab the next line

		p = new vector<string>();

		//cout << endl << tmp ;

		
		if (tmp.compare("") != 0){ //avoid having one last loop at the end of the file

      			split(tmp, ',', *p);     

			//if (p->size() == 1){ //check if it did split so we can exclude comments that start with #
			//	split(tmp, ' ', *p);
			//	cout << endl << tmp << "we split a comment" ;
			//}
			//if (  p->at(0).compare("#") != 0 ) {  //check if a data line or a header  

				//cout << endl << tmp << "we enter a the data collection" ;      

				//convert the string to numbers with the atof
				entry = atoi(p->at(5).c_str());
				x = atof(p->at(0).c_str());
				y = atof(p->at(1).c_str());
				z = atof(p->at(2).c_str());
				E = atof(p->at(3).c_str());

				//debug things
				//cout << endl << tmp ;
				//for ( int i = 0; i < p->size(); i++){
				//cout << atof(p->at(i).c_str()) << ' ';
				//}
				//cout << endl;

				eDep_data.Fill(x,y,z,E,entry);
			//}
		}

      		tmp.clear();
	}


	// Save the ntuple and close the file
	//fclose(fp);
	//eDep_data.Write();
	f->Write();
	//     ofile.Close();
}




