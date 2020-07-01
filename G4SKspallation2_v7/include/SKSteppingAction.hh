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
/// \file SKSteppingAction.hh
/// \brief Definition of the SKSteppingAction class

#ifndef SKSteppingAction_h
#define SKSteppingAction_h 1

#include "G4UserSteppingAction.hh"

#include <vector>
#include <map>
#include "G4Track.hh"
#include "G4TrackVector.hh"

class SKDetectorConstruction;
class SKEventAction;
class G4Step;

/// Stepping action class.
///
/// In UserSteppingAction() there are collected the information about the event
/// inside the tank or fiducial volume then saved in SKEventAction.

class SKSteppingAction : public G4UserSteppingAction
{
public:
  SKSteppingAction(const SKDetectorConstruction* detectorConstruction,
                    SKEventAction* eventAction);
  virtual ~SKSteppingAction();

  virtual void UserSteppingAction(const G4Step* step);
    
private:
  const SKDetectorConstruction* fDetConstruction; 	// so one can check if the particle step is inside the desired volume 
  SKEventAction*  fEventAction;  			// so one can handle the data to the event action objects which save the data for the entire event 

    //
    // Extra functions
    //
    // this is used to make a for loop in the secondaries per step vector - it fills the parents' vectors
    void loopForSecondariesEnergies(const std::vector<const G4Track*>* Secondaries , G4int n ,  
    G4double Energy , G4int Code); 								
    // this is used to make a for loop in the secondaries per track vector - it fills the fParentsCodes map with the parent particle codes	
    bool loopForSecondaries( const G4TrackVector* Secondaries , G4int n , G4int TrackID, G4int Code); 	
    // transverse distance to the muon track line - suppose the muon path to be an "infinite" line to do the calculation
    G4double transverseDistance( G4double x, G4double y, G4double z );					 
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
