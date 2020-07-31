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
// THIS IS THE ACTION INITIALIZATION TO THE GENERATION OF A MUON WITH A GIVEN ENERGY(AVERAGE MUON ENERGY FOR SK) AND 
// A GIVEN POSITION( ON THE TANK AXIS AND ON THE UPPER PART OF THE TANK )
//
/// \file SKFixedActionInitialization.cc
/// \brief Implementation of the SKFixedActionInitialization class

#include "SKFixedActionInitialization.hh"
#include "SKFixedPrimaryGeneratorAction.hh"
#include "SKFixedRunAction.hh"
#include "SKEventAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKFixedActionInitialization::SKFixedActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKFixedActionInitialization::~SKFixedActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKFixedActionInitialization::BuildForMaster() const
{ // setting user's actions for the master in multi-threading mode
   SKEventAction* eventAction = new SKEventAction;
   SetUserAction(new SKFixedRunAction(eventAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKFixedActionInitialization::Build() const
{// setting user's actions for the workers in multi-threading mode or for the full run in sequential mode
  SetUserAction(new SKFixedPrimaryGeneratorAction);

  SKEventAction* eventAction = new SKEventAction;
  SetUserAction(eventAction);

  
  SetUserAction(new SKFixedRunAction(eventAction));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
