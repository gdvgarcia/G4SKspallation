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
//
// THIS IS THE ACTION THE CLASS USED TO READ THE INITIAL CONDITIONS TO A LIST OF DOUBLES
//
/// \file SKFileReader.hh
/// \brief Definition of the SKFileReader class
///
/// The FileReader class, in multi-threading mode, reads the number of threads times the events required per run, 
/// because each thread gets an initial condition when it starts a new event and the speed that each thread ends a 
/// work is not always equal to the other threads, mainly because they have different initial conditions, so one 
/// thread can read much more initial conditions than others. So as the filereader is thread local, each working 
/// thread must have enough conditions for the entire run.

#ifndef SKFileReader_h
#define SKFileReader_h 1

#include <list>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>     

/// File reader class.

using namespace std;

class SKFileReader
{
 public:
   SKFileReader( std::string fileName , int n, int id, int nevents, bool anglesFlag );
   ~SKFileReader();
   
   double 	GetOneValue	();
   bool 	ReadPublic	( int numberOfEvents , int id );
   void 	Clear		();
   void 	Print		();
   bool 	IsEmpty		();
   int	 	Size		(){ return fValues.size(); }

 private:
   std::ifstream 	inputFile; 	// file with the initial conditions
   std::list< double > 	fValues; 	// values read from the file
   bool 		fangles; 	// flag for cancelling the angles reader and only read energy spectrum
   int 			fThreadID;	// ID of the current thread 
   int 			fnThreads;	// number of threads for the run 

   // function to split the lines of the file so we can take the data and put in a string vector
   void Split	(const std::string& line, char divisionchart, std::vector<std::string>& words) ;
   // function to read the text file information
   bool Read	( int numberOfEvents );
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
    
