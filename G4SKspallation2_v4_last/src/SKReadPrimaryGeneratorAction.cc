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

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "SKFileReader.hh"

#include "cmath" // to have the cosine and sine math functions

#include "G4AutoLock.hh"

namespace { G4Mutex SKReadPrimarynMutex = G4MUTEX_INITIALIZER; }
SKFileReader* SKReadPrimaryGeneratorAction::fFileReader = 0;

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKReadPrimaryGeneratorAction::SKReadPrimaryGeneratorAction(SKFileReader* fileReader)
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0), 
  //fThzHalf(-1.),
  //fTouterRadius(-1.)
  //DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
  fThzHalf(16. *m),
  fTouterRadius(14.9 *m)
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


  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get Envelope volume
  // from G4LogicalVolumeStore.


  // getting the volume and then getting its parameters
  /*if(fThzHalf < 0){
  G4Tubs* EnvelopeTub = 0;
  	G4LogicalVolume* envLV
  	  = G4LogicalVolumeStore::GetInstance()->GetVolume("FiducialLV");
  	if ( envLV ) {EnvelopeTub = dynamic_cast<G4Tubs*>(envLV->GetSolid());}
  	else{ 
		envLV = G4LogicalVolumeStore::GetInstance()->GetVolume("TankLV");
		if ( envLV ) {EnvelopeTub = dynamic_cast<G4Tubs*>(envLV->GetSolid());}
  	}
  

  if ( EnvelopeTub ) {
    fTouterRadius 	= EnvelopeTub->GetRMax 		();
    fThzHalf 		= EnvelopeTub->GetZHalfLength 	();
  }  
  else  {
    G4ExceptionDescription msg;
    msg << "Envelope volume of tub shape not found.\n"; 
    msg << "Perhaps you have changed geometry.\n";
    msg << "The gun will be place at the center.";
    G4Exception("SKReadPrimaryGeneratorAction::GeneratePrimaries()",
     "MyCode0002",JustWarning,msg);
  }
  }*/


  // Flag for cancelling the angles reader and only read energy spectrum
  bool anglesflag = false;

  // Variable for the energy from the file
  G4double KinEnergy =  271. - 0.1056583755;

   // getting the value from the file reader and anglesflag
  if(fFileReader)
  {
    	G4AutoLock 	lock(&SKReadPrimarynMutex);
    	anglesflag 	= fFileReader->DoAngles(); 	// getting the angles flag
    	KinEnergy 	= fFileReader->GetOneValue();	// getting the energy value
  }
  
  fParticleGun->SetParticleEnergy( KinEnergy *GeV);	// adding the right energy

  //Building the GUN in the right position 

  //setting default vertex position
  G4double x0 = 0. *m;
  G4double y0 = 0. *m;
  // on top of the tank
  G4double z0 = fThzHalf;

  if(anglesflag){ // check if we deactivated the angles
	// first the direction

	// Convert the strings to numbers with the atof and saving the initial conditions variables
  	G4double theta 	= fFileReader->GetOneValue();
  	G4double phi 	= fFileReader->GetOneValue();

 	// Building the direction
  	G4double Px = cos( phi ) * sin( theta );
  	G4double Py = sin( phi ) * sin( theta );
  	G4double Pz = - cos( theta );

  	// Filling the gun with the right direction
  	fParticleGun->SetParticleMomentumDirection(G4ThreeVector( Px, Py, Pz ));
  }

	
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
