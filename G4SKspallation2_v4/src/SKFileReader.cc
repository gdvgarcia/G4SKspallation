//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// THIS IS THE ACTION THE CLASS USED TO READ THE INITIAL CONDITIONS TO A LIST OF DOUBLES
//
/// \file SKFileReader.cc
/// \brief Implementation of the SKFileReader class

#include "SKFileReader.hh"

#include <list>
#include <fstream>

//to read initial conditions from a file
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKFileReader::SKFileReader( string fileName)
{ inputFile.open (fileName); angles = true; fValues.clear();}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKFileReader::~SKFileReader()
{ inputFile.close(); 
			std::cout << '\n';
  		std::cout << "File closed";
  		std::cout << '\n';
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

double SKFileReader::GetOneValue()
{
  double value = fValues.front();
  fValues.pop_front();
  return value;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKFileReader::Read( int numberOfEvents )
{

 // variables used to read the file
  vector<string>* words = new vector<string>();
  string line;
  double value;

  // jumping the head lines
  getline(inputFile, line, '\n');
  // check if we deactivated the angles with a 0 in the start of the text file
  if ( line.compare("") != 0 ){ //check if the line is not empty
  	Split(line, '#', *words);
	for( unsigned int k = 0; k < words->at(0).size(); k++){
		if(words->at(0)[k] == '0'){ // check if we find a zero
			angles = false; // desactivate angles reader
		}
	}
  }
  // keep jumping the head lines
  getline(inputFile, line, '\n');

  // cleaning the strings
  line.clear();
  words->clear();
	
  //for-loop to collect the data into the list
  for ( int i = 0; ( i < numberOfEvents) && ( !inputFile.eof() ); i++){
	getline(inputFile, line, '\n');  // Grab the line

  	// first we break the data into strings. Each string containing a piece of information                 
  	if ( line.compare("") != 0 ){ //check if the line is not empty
  		Split(line, ' ', *words); 			// Splitting the line into words
  		// Convert the strings to numbers with the atof and saving the initial conditions variables
  		value = atof(words->at(2).c_str()) - 0.1056583755; // Transform the total energy to Kinetic energy
    		// Filling the list
  		fValues.push_back(value);

  		if(angles){ // check if we deactivated the angles
			// Convert the strings to numbers with the atof and saving the initial conditions variables
  			value =  atof(words->at(0).c_str());
			// Filling the list
  			fValues.push_back(value);
			// Convert the strings to numbers with the atof and saving the initial conditions variables
  			value =  atof(words->at(1).c_str());
			// Filling the list
  			fValues.push_back(value);
  		}
  	}

	// cleaning the strings
  	line.clear();
  	words->clear();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//function used to split the line so we can take the data and put in the vector of strings
void SKFileReader::Split(const string& line, char divisionchart, vector<string>& words) {
   int i = 0;
   int j = line.find(divisionchart);

   while (j >= 0) {
      words.push_back(line.substr(i, j-i));
      i = ++j;
      j = line.find(divisionchart, j);

      if (j < 0) {
         words.push_back(line.substr(i, line.length()));
      }
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKFileReader::Clear()
{
  fValues.clear();
  angles = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKFileReader::Print()
{ 
  if(!fValues.empty()){//if it is not empty
  std::cout << '\n';
  for (std::list< double >::iterator it=fValues.begin(); it != fValues.end(); ++it)
  std::cout << ' ' << *it;
  std::cout << '\n';
  }
  else{
  std::cout << "File reader empty";
  std::cout << '\n';
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
