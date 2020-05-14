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
/// \file G4SKspallationEventAction.cc
/// \brief Implementation of the G4SKspallationEventAction class

#include "G4SKspallationEventAction.hh"
#include "G4SKspallationTankSD.hh"
#include "G4SKspallationTankHit.hh"
//#include "G4SKspallationAnalysis.hh" just add if you will fill histograms or ntuples

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4SKspallationEventAction::G4SKspallationEventAction()
 : G4UserEventAction(),
   fTankHCID(-1)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4SKspallationEventAction::~G4SKspallationEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4SKspallationTankHitsCollection* 
G4SKspallationEventAction::GetHitsCollection(G4int hcID,
                                  const G4Event* event) const
{
  auto hitsCollection 
    = static_cast<G4SKspallationTankHitsCollection*>(
        event->GetHCofThisEvent()->GetHC(hcID));
  
  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID; 
    G4Exception("G4SKspallationEventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }         

  return hitsCollection;
}    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4SKspallationEventAction::PrintEventStatistics(
                              G4double N) const
{
  // print event statistics
  G4cout
     << "   Tank: N : " 
     << std::setw(7) << N
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4SKspallationEventAction::BeginOfEventAction(const G4Event* /*event*/)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4SKspallationEventAction::EndOfEventAction(const G4Event* event)
{  
  // Get hits collections IDs (only once)
  if ( fTankHCID == -1 ) {
    fTankHCID 
      = G4SDManager::GetSDMpointer()->GetCollectionID("TankHitsCollection");
  }

  // Get hits collections
  auto tankHC = GetHitsCollection(fTankHCID, event);

  // Get hit with total values
  auto tankHit = (*tankHC)[tankHC->entries()-1];
 
  // Print per event (modulo n)
  //
  auto eventID = event->GetEventID();
  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "---> End of event: " << eventID << G4endl;     

    PrintEventStatistics(
      tankHit->GetN());
  }  
  
  // Fill histograms, ntuple if something that we want to sum up during an event like Edep
  //

  // get analysis manager
  //auto analysisManager = G4AnalysisManager::Instance();
 
  // fill histograms
  //analysisManager->FillH1(0, tankHit->GetA());
  
  // fill ntuple
  //analysisManager->FillNtupleDColumn(0, tankHit->GetA());
  //analysisManager->AddNtupleRow();  
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
