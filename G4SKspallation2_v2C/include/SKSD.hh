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
/// \file SKSD.hh
/// \brief Definition of the SKSD class

#ifndef SKSD_h
#define SKSD_h 1

#include "G4VSensitiveDetector.hh"

#include "SKHit.hh"

#include <vector>
#include <map>
#include <string> 

class G4Step;
class G4HCofThisEvent;

/// SK sensitive detector class
///
/// In Initialize(), it creates one hit 
///
/// The values are accounted in hits in ProcessHits() function which is called
/// by Geant4 kernel at each step.

class SKSD : public G4VSensitiveDetector
{
  public:
    SKSD(const G4String& name, 
                     const G4String& hitsCollectionName);
    virtual ~SKSD();
  
    // methods from base class
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

  private:
    SKHitsCollection* fHitsCollection;
    G4int fEventID; // keep track of the event number
    std::map< int, std::string> fParents; // keep parents' trackIDs so we can recover parents' names
    std::map< int, double> fInitialTimes; // keep isotopes' initial time
    // std::map< int, std::map< int, double >> fParentsEnergy; // keep parents' energies so we can recover parents' energies  -- DELETE IF CAN NOT SOLVE IT

    //
    // Functions to make the code more userfriendly
    //
    void loopForSecondariesEnergies(const std::vector<const G4Track*>* Secondaries , G4int n , //G4int TrackID,  
    G4double Energy , G4String Process, G4String name ); // this is used to make a for loop in the secondaries per step vector - it fills the fParentsEnergy map - THE NTUPLE ACTUALLY
    bool loopForSecondaries( G4TrackVector* Secondaries , G4int n , G4int TrackID, G4String name ); // this is used to make a for loop in the secondaries per track vector - it fills the fParents map with the names
    void transverseFill( G4double x, G4double y , G4String name ); // transverse distance calculator and ntuple filler
    void muonFill( G4double E, G4double x , G4double y , G4double z , G4String name ); // muon ntuple filler
    void isotopeFill( G4String name , G4int A, G4int Z, G4double x , G4double y , 
    G4double z , G4int    ParentID,  G4String Process, G4double FinalTime, G4double LifeTime, G4String      
    daughters  , G4int TrackID ); // isotopes ntuple filler
    void particlesFill( G4String name , G4double Energy , G4double StepLength ); // particles ntuple filler
    G4String loopForSecondariesNames(const std::vector<const G4Track*>* Secondaries , G4int n ); // this return secondaries names
};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

