// Read the tree and make transverse distance histos plot

#include "TROOT.h"

#include <iostream> 
#include <list>
#include <vector>
#include <fstream>

int CovertMUSICtotxt( char argv  []){

	// Opening the file with the data in dat
	std::string inFilename = argv;
	std::ifstream inputFile;
	inputFile.open (inFilename);
	// checking if the file is open correctly
	if (inputFile.is_open()) {
    		std::cout << "Reading file: " << inFilename << std::endl;
  	}
  	else {
    		std::cerr << "Error opening input file!" << std::endl;
    		return 0;
  	}

 	// variables used to read the file
  	std::list<std::string>* lines = new std::list<std::string>();
  	std::string line;

	// reading the file and saving its lines in a string list
 	while( !inputFile.eof() ){ // while file not ended
  		getline(inputFile, line, '\n'); // putting the line into the line string
  		lines->push_back(line); 	// adding the line to the list
  		// cleaning the strings
  		line.clear();
	}

	// Closing in file
	//inputFile.close();	

	//
	// MAKING THE TEXT FILE
	//
	std::cout << "Printing the information from dat into a txt file" << std::endl;

	// cutting the name in '.' to remove the dat
	char divisionchart = '.'; 
	vector<string> words;
	int i = 0;
   	int j = inFilename.find(divisionchart);

   	while (j >= 0) {
      		words.push_back(inFilename.substr(i, j-i));
      		i = ++j;
      		j = inFilename.find(divisionchart, j);

      		if (j < 0) {
         		words.push_back(inFilename.substr(i, inFilename.length()));
		}
	}

	
	std::cout << "Out file name = " << words.at(0) << std::endl;
	
	// Create a new file for the list
	std::string outFilename =  words.at(0) + ".txt";
	std::ofstream outfile;
  	outfile.open (outFilename);
	//outfile.precision(2);
	//outfile.setf(ios::fixed);
	//outfile.setf(ios::showpoint);
	
	// printing the list and the headers
    	outfile << "# MUON INITIAL CONDITION GENERATOR\n"; 
    	outfile << "# angleTheta # anglePhi # Initial kinetic energy [GeV]\n";
	int size = lines->size();
	for( int i = 0; i < size; i++){
		line = lines->front();
  		lines->pop_front();
		outfile << line <<'\n';  // printingthe line 		
  		line.clear();		 // clearing the string
	}


	// Closing out file
	outfile.close();

	return 1;

}
