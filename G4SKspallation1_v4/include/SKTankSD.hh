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
/// \file SKTankSD.hh
/// \brief Definition of the SKTankSD class

#ifndef SKTankSD_h
#define SKTankSD_h 1

#include "G4VSensitiveDetector.hh"

#include "SKTankHit.hh"
#include "globals.hh"
#include <vector>

class G4Step;
class G4HCofThisEvent;
class G4Track;

/// Tank sensitive detector class
///
/// In Initialize(), it creates the histograms and the ntuples used for collecting data
/// 
/// The values are accounted in hits in ProcessHits() function which is called
/// by Geant4 kernel at each step.
///
/// EndOfEvent(), it saves the histograms and the ntuples used for collecting data

class SKTankSD : public G4VSensitiveDetector
{
  public:
    SKTankSD(const G4String& name, 
                     const G4String& hitsCollectionName);
    virtual ~SKTankSD();
  
    // methods from base class
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

