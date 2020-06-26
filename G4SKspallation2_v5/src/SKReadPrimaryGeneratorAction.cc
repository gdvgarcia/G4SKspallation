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
// HERE WE GENERATE A MUON WITH ENERGY AND MOMENTUM DIRECTION GIVEN BY AN ASCII FILE AND CROSSING 
// A RANDOM POSITION INSIDE THE TANK/FIDUCIAL
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
  fThzHalf(-1.),
  fTouterRadius(-1.)
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
  if( fFileReader ) { fFileReader = 0; }
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKReadPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //This function is called at the begining of each event
  //


  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get cylinder parameters
  // from G4LogicalVolumeStore.

  // getting the volume and then getting its parameters
  if(fThzHalf < 0.){
  	G4Tubs* EnvelopeTub = 0;
  		G4LogicalVolume* envLV
  	  	  = G4LogicalVolumeStore::GetInstance()->GetVolume("FiducialLV");
  		if ( envLV ) { 
			EnvelopeTub = dynamic_cast<G4Tubs*>(envLV->GetSolid());
			G4cout << "\n";
			G4cout << "Fiducial volume activated. \n";
			G4cout << "\n";
		}
  		else{ 
			envLV = G4LogicalVolumeStore::GetInstance()->GetVolume("TankLV");
  			if ( envLV ) {
				EnvelopeTub = dynamic_cast<G4Tubs*>(envLV->GetSolid());
				G4cout << "\n";
				G4cout << "Fiducial volume deactivated. \n";
				G4cout << "\n";
			}
  		}
  

  	if ( EnvelopeTub ) {
    		fTouterRadius 	= EnvelopeTub->GetRMax 		();
    		fThzHalf 	= EnvelopeTub->GetZHalfLength 	();
  	}  
  	else  {
    		G4ExceptionDescription msg;
    		msg << "Envelope volume of tub shape not found.\n"; 
    		msg << "Perhaps you have changed geometry.\n";
    		msg << "The gun will be place at the center.";
    		G4Exception("SKFixedPrimaryGeneratorAction::GeneratePrimaries()",
     		"MyCode0002",JustWarning,msg);
  	}
  }



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

	// second the initial location

 	// Here we choose a point inside the SK tank/fiducial with a uniform distribution, this point will be in the muon path
  	const G4double 	pi = 3.14159265358979323846264338328;
  	G4double 	theta2 =  2* pi * G4UniformRand();
  	G4double 	r =  fTouterRadius * G4UniformRand();
  	// setting the point
  	G4double x1 	= cos( theta2 ) * r;
  	G4double y1 	= sin( theta2 ) * r;
	G4double z1 	= fThzHalf * 2 * (0.5 - G4UniformRand());

	// initialize
	G4double D 	= 0. *m;

	// projecting our point into the cylinder surface in the inverse direction of its momentum

	// we break it down in 2 possibilities to find the distance from the point to the cylinder along its past trajectory

	// first in Z 
	if( Pz < 0. || Pz > 0. ){ // so cos(theta) is not zero
		D =  (  fThzHalf - z1 ) / cos( theta );
		// setting the vertex
		x0 = x1 - Px * D;
		y0 = y1 - Py * D;
		z0 = z1 - Pz * D;
	}

	// second in radius
	if ( x0*x0 + y0*y0 > fTouterRadius*fTouterRadius  + 10 && Pz > -1.){ // 10 mm precision
		G4double delta = ( Px * x1 + Py * y1 )* ( Px * x1 + Py * y1 ) - (Px * Px + Py * Py) * ( x1 * x1 + y1 * y1 - fTouterRadius * fTouterRadius);
		if ( delta > 0.){
			D = (  ( Px * x1 + Py * y1 )  + sqrt( delta )  )/ (   sqrt(Px * Px + Py * Py) * sin( theta ) )  ;
			// setting the vertex
			x0 = x1 - Px * D;
			y0 = y1 - Py * D;
			z0 = z1 - Pz * D;
		}
	}


	if ( z0 > fThzHalf + 10) {// 10 mm precision
		G4cerr << "Error in finding the projection. Z error. Z = " << z0 << " and Zmax = " << fThzHalf << " and R = " << sqrt(x0*x0 + y0*y0) << " and Rmax = " << fTouterRadius << "\n";
		x0 = 0;
		y0 = 0;
		z0 = 0;
	}
	
	if ( x0*x0 + y0*y0 > fTouterRadius*fTouterRadius  + 10){// 10 mm precision
		G4cerr << "Error in finding the projection. Radius error. R = " << sqrt(x0*x0 + y0*y0) << " and Rmax = " << fTouterRadius << " and Z = " << z0 << " and Zmax = " << fThzHalf << "\n";
		x0 = 0;
		y0 = 0;
		z0 = 0;
	}
  }

	
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
