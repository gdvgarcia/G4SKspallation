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
// HERE WE GENERATE A MUON WITH A GIVEN ENERGY(AVERAGE MUON ENERGY FOR SK) AND 
// A GIVEN POSITION( ON THE TANK AXIS AND ON THE UPPER PART OF THE TANK )
//
/// \file SKFixedPrimaryGeneratorAction.cc
/// \brief Implementation of the SKFixedPrimaryGeneratorAction class

#include "SKFixedPrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKFixedPrimaryGeneratorAction::SKFixedPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0),
  fEnvelopeTub(0)
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
  // Transform the average total energy of 271 GeV to Kinetic energy
  G4double iniKineticEnergy = 271. - 0.1056583755;
  fParticleGun->SetParticleEnergy( iniKineticEnergy * GeV );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKFixedPrimaryGeneratorAction::~SKFixedPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKFixedPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //This function is called at the begining of each event
  //


  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get Envelope volume
  // from G4LogicalVolumeStore.

  //SK tank or fiducial parameters to set up the gun on top of the tank or fiducial
  G4double ThzHalf = 0.;

  // getting the volume and then getting its parameters
  if(!fEnvelopeTub){
  	G4LogicalVolume* envLV
  	  = G4LogicalVolumeStore::GetInstance()->GetVolume("FiducialLV");
  	if ( envLV ) { fEnvelopeTub = dynamic_cast<G4Tubs*>(envLV->GetSolid());}
  	else{ 
		envLV = G4LogicalVolumeStore::GetInstance()->GetVolume("TankLV");
  		if ( envLV ) {fEnvelopeTub = dynamic_cast<G4Tubs*>(envLV->GetSolid());}
  	}
  }

  if ( fEnvelopeTub ) {
    ThzHalf 		= fEnvelopeTub->GetZHalfLength 	();
  }  
  else  {
    G4ExceptionDescription msg;
    msg << "Envelope volume of tub shape not found.\n"; 
    msg << "Perhaps you have changed geometry.\n";
    msg << "The gun will be place at the center.";
    G4Exception("SKFixedPrimaryGeneratorAction::GeneratePrimaries()",
     "MyCode0002",JustWarning,msg);
  }

  //setting vertex position
  G4double x0 = 0;		//centered
  G4double y0 = 0;		//centered
  G4double z0 = ThzHalf;	//on top of the tank
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));


  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

