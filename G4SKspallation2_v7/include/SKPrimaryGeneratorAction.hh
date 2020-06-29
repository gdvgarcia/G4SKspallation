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
/// \file SKPrimaryGeneratorAction.hh
/// \brief Definition of the SKPrimaryGeneratorAction class

#ifndef SKPrimaryGeneratorAction_h
#define SKPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "SKFileReader.hh"

#include <string>

using namespace std;

class G4ParticleGun;
class G4GenericMessenger;
class G4Event;

/// The primary generator action class with particle gun.
///
/// It reads the initial energy and momentum direction from a ASCII file
/// User can select 
/// - the initial conditions file name

class SKFileReader;

class SKPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    SKPrimaryGeneratorAction();    
    virtual ~SKPrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);         
  
    // method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
  
  private:
    void DefineCommands(); // method where we define our commands that set fileReader properties

    G4ParticleGun*  		fParticleGun; 	// pointer a to G4 gun class
    G4double 			fThzHalf;	// height of either the fiducial or the tank volume
    G4double 			fTouterRadius;	// radius of either the fiducial or the tank volume
    G4GenericMessenger* 	fMessenger;	// messenger which sets the commands
    G4String 			fFileName;	// initial conditions file name
    G4int 			fNofCores;	// number of cores for multithreading
    G4int 			fCoreID;	// ID of the current thread 
    G4int 			fNEvents;	// number of events for the current thread 
    SKFileReader* 		fFileReader; 	// file reader object used to read data from the file of initial conditions
    G4bool	 		fClear; 	// flag for cleaning the file reader
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
