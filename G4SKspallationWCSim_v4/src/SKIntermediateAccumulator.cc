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
// THIS IS THE ACTION THE CLASS USED TO ACCUMULATE INFORMATION OF AN EVENT AND PASS IT TO THE RUN ACTION
// SPALLATION ADDITION OF THIS CLASS
//
/// \file SKIntermediateAccumulator.cc
/// \brief Implementation of the SKIntermediateAccumulator class
///
/// The SKIntermediateAccumulator class is necessary, because WCSim EventAction requires the RunAction 
/// object as a variable to its creation, so we can not pass an EventAction object for the creation of 
/// the RunAction object


#include "SKIntermediateAccumulator.hh"

#include "g4root.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"
#include <iomanip>
#include <vector>
#include <map>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKIntermediateAccumulator::SKIntermediateAccumulator()
 : fSaveDataFlag(false),
   fEdep(0.),     					///< Energy deposit in the sensitive volume
   fEmu(0.),      					///< Initial kinetic energy of the muon
   fLmu(0.),      					///< Path lenght of the muon
   fxIni(0.), 						///< x initial position  of the muon
   fyIni(0.),						///< y initial position  of the muon
   fzIni(0.),						///< z initial position  of the muon
   fxFin(0.),						///< x initial position  of the muon
   fyFin(0.),						///< y initial position  of the muon
   fzFin(0.)						///< z initial position  of the muon 
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKIntermediateAccumulator::~SKIntermediateAccumulator()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKIntermediateAccumulator::PrintEventStatistics() const
{
  // print event statistics
  G4cout
     << "   Tank: total energy deposited : " 
     << std::setw(7) << G4BestUnit(fEdep, "Energy")
     << G4endl;
  G4cout
     << "   Tank: muon path length : " 
     << std::setw(7) << G4BestUnit(fLmu, "Length")
     << G4endl;

  // print the initial conditions
  G4cout
     << "   Muon initial kinetic energy : " 
     << std::setw(7) << G4BestUnit(fEmu, "Energy")
     << G4endl << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKIntermediateAccumulator::SaveAndPrintEndOfEvent(const G4Event* event) 
{
  // Print per event (modulo n)
  //
  auto eventID = event->GetEventID();
  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "---> End of event: " << eventID << G4endl;     

    PrintEventStatistics();
  }  
  
  // Fill the histograms and the ntuple
  //
  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
  // fill histograms
  analysisManager->FillH1(0, fEdep);
  analysisManager->FillH1(1, fEmu);
  analysisManager->FillH1(2, fLmu);
  // fill the ntuple single variables - vectors are filled with the vector from this class directly
  analysisManager->FillNtupleDColumn( 0, 	fEmu);
  analysisManager->FillNtupleDColumn( 1, 	fxIni); 
  analysisManager->FillNtupleDColumn( 2, 	fyIni);
  analysisManager->FillNtupleDColumn( 3, 	fzIni);
  analysisManager->FillNtupleDColumn( 4, 	fEdep);
  analysisManager->FillNtupleDColumn( 5, 	fxFin);
  analysisManager->FillNtupleDColumn( 6,	fyFin);
  analysisManager->FillNtupleDColumn( 7, 	fzFin);
  analysisManager->FillNtupleDColumn( 8, 	fLmu);
  analysisManager->FillNtupleIColumn(9, 	fvectorA.size());
  analysisManager->FillNtupleIColumn(21, 	fvectorParentsCode.size());
  // adding the Row
  analysisManager->AddNtupleRow();

  // clean everything for the next event
  ClearAccumulator();
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// only necessary to the stepAction - maps for the entire event
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SKIntermediateAccumulator::FillParentsCodes( G4int TrackID, G4int 		ParentCode ){
  fParentsCodes[TrackID] = ParentCode; 
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SKIntermediateAccumulator::FillInitialTimes( G4int TrackID, G4double 	InitialTime ){
  fInitialTimes[TrackID] = InitialTime; 
} 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
