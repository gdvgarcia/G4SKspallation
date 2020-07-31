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
    std::map< int, int > fParentsCodes; 	// keep parents' trackIDs so we can recover parents' codes
    std::map< int, double> fInitialTimes; 	// keep isotopes' initial time
    std::vector< double > fMuonPositions; 	// keep the muon initial and final position so we can calculate the transverse distance of particles - ID 0 = x_ini , ID 1 = y_ini , ID 2 = z_ini , ID 3 = x_fin , ID 4 = y_fin , ID 5 = z_fin


    //
    // Extra functions
    //
    void loopForSecondariesEnergies(const std::vector<const G4Track*>* Secondaries , G4int n ,  
    G4double Energy , SKHit* hit, G4int Code); // this is used to make a for loop in the secondaries per step vector - it fills the parents hit
    bool loopForSecondaries( G4TrackVector* Secondaries , G4int n , G4int TrackID, G4int Code); // this is used to make a for loop in the secondaries per track vector - it fills the fParentsCodes map with the parent particle codes
    G4double transverseDistance( G4double x, G4double y, G4double z ); // transverse distance to the muon track line - suppose an "infinite" line to do the calculation

};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

