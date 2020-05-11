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
/// \file G4SKspallationPrimaryGeneratorAction.cc
/// \brief Implementation of the G4SKspallationPrimaryGeneratorAction class

#include "G4SKspallationPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "cmath" // to have the cosine and sine math functions

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4SKspallationPrimaryGeneratorAction::G4SKspallationPrimaryGeneratorAction()
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
  fParticleGun->SetParticleEnergy(50.*GeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4SKspallationPrimaryGeneratorAction::~G4SKspallationPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4SKspallationPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of each event
  //
  

  //SK tank parameters to set up the gun in the right position and surface
  G4double TouterRadius = 19.5*m; 
  G4double Thz = 42.*m;


  // Here we make the gun shoot inside the SK tank
  const G4double 	pi = 3.14159265358979323846264338328;
  G4double 	theta =  2* pi * G4UniformRand();
  G4double 	r =  TouterRadius * G4UniformRand();

  G4double x0 = cos( theta ) * r;
  G4double y0 = sin( theta ) * r;
  G4double z0 = 0.5 * Thz+ 2.*m;//2 meters of water to describe the rock
  
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

  fParticleGun->GeneratePrimaryVertex(anEvent);

	//randon energy following the muon spectrum
	//creating the spectrum 
	const int n_points=10;
	double x_vals[n_points]={0.1,1,2,3,10,100,200,1000,2000,4000};
	double y_vals[n_points]={2e-09,1e-09,1e-09,1e-09,1e-09,4e-10,2.25e-10,1e-11,1.5e-12,1.75e-13};

	double rnd = 1.;
	double y_r, x_r;
	int pos;

	//generate a random pair
	rnd = G4UniformRand();
	y_r = (1e-08 - 1e-14)*rnd+1e-14;
	rnd = G4UniformRand();
	pos = int(10*rnd);

	//to find a good pair
	bool stop = TRUE;
	
	while(stop){
		if(y_r < y_vals[pos]){
		// create a random smearing
		rnd = G4UniformRand();
		if (pos==0){
			rnd = 0.1*G4UniformRand();
			}
		x_r = rnd + x_vals[pos];
		fParticleGun->SetParticleEnergy(x_r *GeV);
		stop = FALSE;
		}
	//generate a random pair
	rnd = G4UniformRand();
	y_r = (1e-08 - 1e-14)*rnd+1e-14;
	rnd = G4UniformRand();
	pos = int(10*rnd);
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

