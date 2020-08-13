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

SKFileReader::SKFileReader( string fileName, int n , int id, int nevents, bool anglesFlag)
{   
  std::cout << "\nOpening "<< fileName << " ... \n";
  fnThreads 	= n;
  fThreadID 	= id;
  fangles 	= anglesFlag; 
  inputFile.open (fileName); 
  if (inputFile.is_open()) { 
  	std::cout << '\n';
  	std::cout << "Initial conditions file is opened.";
  	std::cout << '\n';
	std::cout << "\nRead the initial conditions file into a list for " << nevents  << " events in the thread " << id << "." << std::endl;
	Read(nevents);
    	std::cout << '\n';
  }
  else {
    	std::cout << '\n';
  	std::cout << "Could not find initial conditions file with the path/name = " << fileName << ". Opening ini_cond.txt ...";
  	std::cout << '\n';
	inputFile.open ("ini_cond.txt"); 
	if (inputFile.is_open()) { 
  		std::cout << '\n';
  		std::cout << "Initial conditions file ini_cond.txt is opened.";
  		std::cout << '\n';
		std::cout << "\nRead the initial conditions file into a list for " << nevents  << " events in the thread " << id << "." << std::endl;
		Read(nevents);
		std::cout << '\n';
  	}
  	else {
    		std::cout << '\n';
  		std::cout << "\nCould not open ini_cond.txt.";
  		std::cout << '\n'; 
  	}
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKFileReader::~SKFileReader()
{
  fValues.clear();
  if (inputFile.is_open()) {  
	inputFile.close(); 
  	std::cout << '\n';
  	std::cout << "Initial conditions file has been closed.";
  	std::cout << '\n';
  	std::cout << '\n';
  }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

double SKFileReader::GetOneValue()
{
  if(!fValues.empty()){			//if it is not empty
  double value = fValues.front();
  fValues.pop_front(); 			// remove the value from the list
  return value;
  }
  else{
  std::cout << '\n';
  std::cout << "File reader is empty. Returning 1 in place of the expected value from the initial condition file.";
  std::cout << '\n';
  return 1.;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bool SKFileReader::Read( int numberOfEvents )
{
 if (!inputFile.is_open()) { return false;} // checking if file is open

 // variables used to read the file
  vector<string>* words = new vector<string>();
  string line;
  double value;

  // jumping the head lines
  getline(inputFile, line, '\n');
  getline(inputFile, line, '\n');

  
  // jumping the lines that are read by the other threads
  for( int j = 0; j < fThreadID; j++ ){ getline(inputFile, line, '\n'); }

  // int to represent the line we are at
  int l = fThreadID;

  //for-loop to collect the data into the list
  for ( int i = 1 ; ( l < fnThreads * numberOfEvents + fThreadID ) && ( !inputFile.eof() ); i++ ){ // get the total number of events, because if one thread goes too slow the others run more events so we must be sure to have enough events for each

  	// cleaning the strings
  	line.clear();
  	words->clear();

	getline(inputFile, line, '\n');  				// Grab the line

  	// first we break the data into strings. Each string containing a piece of information                 
  	if ( line.compare("") != 0 ){ 					//check if the line is not empty
  		Split(line, ' ', *words); 				// Splitting the line into words
  		// Convert the strings to numbers with the atof and saving the initial conditions variables
  		value = atof(words->at(2).c_str()) - 0.1056583755; 	// Transform the total energy to Kinetic energy
    		// Filling the list
  		fValues.push_back(value);

  		if(fangles){ 						// check if we deactivated the angles
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

	// jumping the lines that are read by the other threads
  	for( int j = 1; j < fnThreads; j++ ){ getline(inputFile, line, '\n'); }


	// updating int line
	l = fnThreads * i + fThreadID ;
  }

  delete words;

  if (inputFile.is_open()) {  // closing the file after reading
	inputFile.close(); 
  	std::cout << '\n';
  	std::cout << "Initial conditions file is closed.";
  	std::cout << '\n';
  	std::cout << '\n';
  }
  else {
    	std::cout << '\n';
  	std::cout << "Initial conditions  file is already closed.";
  	std::cout << '\n';
  	std::cout << '\n';
  }

  return true;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bool SKFileReader::ReadPublic( int numberOfEvents, int id )
{
 if (!inputFile.is_open()) {  // checking if file is open
 std::cout << '\n';
 std::cout << "Could not read the events in the thread " << fThreadID << " because the initial condition file is not open." << std::endl;
 return false;
 } 

 // setting ID
 fThreadID = id;

 // variables used to read the file
  vector<string>* words = new vector<string>();
  string line;
  double value;

  // jumping the head lines
  getline(inputFile, line, '\n');
  getline(inputFile, line, '\n');

  
  // jumping the lines that are read by the other threads
  for( int j = 0; j < fThreadID; j++ ){ getline(inputFile, line, '\n'); }


  // int to represent the line we are at
  int l = fThreadID;

  //for-loop to collect the data into the list
  for ( int i = 1 ; ( l < fnThreads * numberOfEvents + fThreadID ) && ( !inputFile.eof() ); i++ ){

  	// cleaning the strings
  	line.clear();
  	words->clear();

	getline(inputFile, line, '\n');  				// Grab the line

  	// first we break the data into strings. Each string containing a piece of information                 
  	if ( line.compare("") != 0 ){ 					//check if the line is not empty
  		Split(line, ' ', *words); 				// Splitting the line into words
  		// Convert the strings to numbers with the atof and saving the initial conditions variables
  		value = atof(words->at(2).c_str()) - 0.1056583755; 	// Transform the total energy to Kinetic energy
    		// Filling the list
  		fValues.push_back(value);

  		if(fangles){ 						// check if we deactivated the angles
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

	// jumping the lines that are read by the other threads
  	for( int j = 1; j < fnThreads; j++ ){ getline(inputFile, line, '\n'); }


	// updating int line
	l = fnThreads * i + fThreadID ;
  }


  // reseting the get line position - returning to the start of the file
  inputFile.seekg(0,std::ios::beg);

  delete words;

  std::cout << '\n';
  std::cout << "Read the initial conditions file into a list for " << numberOfEvents  << " events in the thread " << fThreadID << "." << std::endl;
  std::cout << '\n';
  return true;
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
  fangles = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKFileReader::Print()
{ 
  if(!fValues.empty()){	//if it is not empty
  std::cout << '\n';
  int size = 0;
  for (std::list< double >::iterator it=fValues.begin(); it != fValues.end(); ++it){
  	std::cout << ' ' << *it;
  	size++;
  }
  std::cout << '\n';
  std::cout << " Size of the file reader vector " << size << '\n';
  }
  else{
  std::cout << '\n';
  std::cout << "File reader empty";
  std::cout << '\n';
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bool SKFileReader::IsEmpty()
{ 
  if(!fValues.empty()){	//if it is not empty
  	return false;
  }
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
