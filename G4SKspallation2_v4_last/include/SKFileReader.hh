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
   SKFileReader( std::string fileName );
   ~SKFileReader();
   
   double 	GetOneValue	();
   void 	Read		( int numberOfEvents );
   bool 	DoAngles	(){ return fangles; }
   void 	Clear		();
   void 	Print		();

 private:
   std::ifstream inputFile; // file with the initial conditions
   std::list< double > fValues; // values read from the file
   bool angles; // Flag for cancelling the angles reader and only read energy spectrum

   // Funtion to split the lines of the file so we can take the data and put in a string vector
   void Split(const std::string& line, char divisionchart, std::vector<std::string>& words) ;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
    
