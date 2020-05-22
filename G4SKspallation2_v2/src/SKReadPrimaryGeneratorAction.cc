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
// HERE WE GENERATE A MUON WITH ENERGY GIVEN BY AN ASCII FILE AND 
// A GIVEN POSITION( ON THE TANK AXIS AND ON THE UPPER PART OF THE TANK )
//
/// \file SKReadPrimaryGeneratorAction.cc
/// \brief Implementation of the SKReadPrimaryGeneratorAction class

#include "SKReadPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

#include "cmath" // to have the cosine and sine math functions

//to read initial conditions from a file
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKReadPrimaryGeneratorAction::SKReadPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="mu-");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,-1.));
  fParticleGun->SetParticleEnergy(271.*GeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKReadPrimaryGeneratorAction::~SKReadPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKReadPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //This function is called at the begining of each event
  //
  
  // getting the event ID to choose the right line
  int eventID = anEvent->GetEventID();
  
  // open the ASCII file
  ifstream file;
  file.open ("ini_cond.txt"); //make sure to have a large file, so you do not run out of events

  //  Fill the Particle Gun with the data from the ASCII file
  // variables used to read the file
  vector<string>* words = new vector<string>();
  string line;

  // jumping the head lines
  getline(file, line, '\n');
  getline(file, line, '\n');
  //for-loop to jump to the right line (other lines already used in previous events)
  for ( int i = 0; ( i < eventID) && ( !file.eof() ); i++){
	getline(file, line, '\n');  // Grab the line
  }

  // cleaning the string
  line.clear();
  
  if( !file.eof() ){//check if we still have lines
	getline(file, line, '\n');  // Grab the line
  }
 
  // We are in the right line so we can collect the data
  // first we break the data into strings. Each string containing a piece of information                 
  if ( line.compare("") != 0 ){ //check if the line is not empty
  Split(line, ' ', *words); 			// Splitting the line into words
  // Convert the strings to numbers with the atof and saving the initial conditions variables
  G4double E = atof(words->at(2).c_str());
  //G4double theta =  atof(words->at(1).c_str());
  //G4double phi =  atof(words->at(0).c_str());

  // Building the direction
  //G4double Px = cos( theta ) * cos( phi );
  //G4double Py = sin( theta ) * cos( phi );
  //G4double Pz = - sin( phi );

  // Filling the gun
  fParticleGun->SetParticleEnergy(E *GeV);
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector( Px, Py, Pz ));
  }

  // cleaning the string and closing file
  file.close();

  //Building the GUN in the right position 

  //SK tank parameters to set up the gun in the right height
  G4double Thz = 42.*m;

  //setting vertex position
  G4double x0 = 0;//centered
  G4double y0 = 0;//centered
  G4double z0 = 0.5 * Thz;//on top of the tank
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//function used to split the line so we can take the data and put in the vector
void SKReadPrimaryGeneratorAction::Split(const string& line, char divisionchart, vector<string>& words) {
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

