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
// HERE WE GENERATE A MUON WITH ENERGY AND MOMENTUM DIRECTION GIVEN BY AN ASCII FILE AND 
// A RANDOM POSITION( ON THE UPPER PART OF THE TANK )
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
#include "SKFileReader.hh"

#include "cmath" // to have the cosine and sine math functions

//to read initial conditions from a file
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <list>
#include "G4AutoLock.hh"

namespace { G4Mutex SKReadPrimarynMutex = G4MUTEX_INITIALIZER; }
SKFileReader* SKReadPrimaryGeneratorAction::fFileReader = 0;

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKReadPrimaryGeneratorAction::SKReadPrimaryGeneratorAction(SKFileReader* fileReader)
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // getting the file reader and locking it to have no issues in multi-threading mode
  G4AutoLock lock(&SKReadPrimarynMutex);
  fFileReader = fileReader;

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="mu-");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,-1.));
  // Transform the average total energy of 271 GeV to Kinetic energy
  G4double iniKineticEnergy = 271. - 0.1056583755;
  fParticleGun->SetParticleEnergy( iniKineticEnergy * GeV );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKReadPrimaryGeneratorAction::~SKReadPrimaryGeneratorAction()
{
  G4AutoLock lock(&SKReadPrimarynMutex);
  
  if( fFileReader ) { delete fFileReader; fFileReader = 0; }
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKReadPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //This function is called at the begining of each event
  //

  // Flag for cancelling the angles reader and only read energy spectrum
  bool anglesflag = false;

  // Variable for the energy from the file
  G4double KinEnergy = 1.;

   // getting the value from the file reader and anglesflag
  if(fFileReader)
  {
    	G4AutoLock lock(&SKReadPrimarynMutex);
    	anglesflag = fFileReader->DoAngles();
    	KinEnergy = fFileReader->GetOneValue();
  }
  
  fParticleGun->SetParticleEnergy( KinEnergy *GeV);

  if(anglesflag){ // check if we deactivated the angles
	// Convert the strings to numbers with the atof and saving the initial conditions variables
  	G4double theta 	= fFileReader->GetOneValue();
  	G4double phi 	= fFileReader->GetOneValue();

 		// Building the direction
  	G4double Px = cos( phi ) * sin( theta );
  	G4double Py = sin( phi ) * sin( theta );
  	G4double Pz = - cos( theta );

  	// Filling the gun
  	fParticleGun->SetParticleMomentumDirection(G4ThreeVector( Px, Py, Pz ));
  }


  //Building the GUN in the right position 

  //SK tank parameters to set up the gun in on top of the tank randomly
  G4double TouterRadius = 19.5*m; 
  G4double Thz = 42.*m;

  //setting default vertex position
  G4double x0 = 0;
  G4double y0 = 0;
  // on top of the tank
  G4double z0 = 0.5 * Thz;

  if(anglesflag){ // check if we deactivated the angles - so no need for random position
 	// Here we make the gun shoot inside the SK tank with a uniform distribution
  	const G4double 	pi = 3.14159265358979323846264338328;
  	G4double 	theta =  2* pi * G4UniformRand();
  	G4double 	r =  TouterRadius * G4UniformRand();
  	//setting vertex position
  	x0 = cos( theta ) * r;
  	y0 = sin( theta ) * r;
  }

  // Filling the gun
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
