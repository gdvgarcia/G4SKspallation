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
/// \file SKEventAction.cc
/// \brief Implementation of the SKEventAction class

#include "SKEventAction.hh"
#include "SKSD.hh"
#include "SKHit.hh"
#include "SKAnalysis.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"
#include <iomanip>
#include<string>
#include <iostream>

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKEventAction::SKEventAction()
 : G4UserEventAction(),
   fTankHCID(-1)
{  
  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in SKAnalysis.hh
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Create directories 
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  //analysisManager->SetNtupleMerging(true);
    // Note: merging ntuples is available only with Root output

  // Book histograms, ntuple
  //
  
  // Creating histograms
  // 1D histograms for energy spectra
  analysisManager->CreateH1("En_spec","Energy of neutrons", 1000, 0.1 , 500.1 , "MeV", "none", "log"
);
  analysisManager->CreateH1("Ee_spec","Energy of electrons + positrons", 1000, 0.1 , 500.1 ,"MeV", "none", "log"
);
  analysisManager->CreateH1("Epi_spec","Energy of charged pions", 1000, 0.1 , 500.1 ,"MeV", "none", "log"
);
  analysisManager->CreateH1("Epi0_spec","Energy of neutral pion", 1000, 0.1 , 500.1 ,"MeV", "none", "log"
);
  
  // 2D histogram for A and Z quantities
  analysisManager->CreateH2("Isotopes", "Ions produced; Z values; A values", 150, 0.5, 150.5, 300, 0.5, 300.5);

  // Creating the ntuple
  //
  analysisManager->CreateNtuple("isotopes_data", "A, Z, position and type of parent process");
  analysisManager->CreateNtupleDColumn("A");
  analysisManager->CreateNtupleDColumn("Z");
  analysisManager->CreateNtupleDColumn("x");
  analysisManager->CreateNtupleDColumn("y");
  analysisManager->CreateNtupleDColumn("z");
  analysisManager->CreateNtupleSColumn("Process");
  analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKEventAction::~SKEventAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKHitsCollection* 
SKEventAction::GetHitsCollection(G4int hcID,
                                  const G4Event* event) const
{
  auto hitsCollection 
    = static_cast<SKHitsCollection*>(
        event->GetHCofThisEvent()->GetHC(hcID));
  
  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID; 
    G4Exception("SKEventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }         

  return hitsCollection;
}    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKEventAction::PrintEventStatistics(
                              G4double Edep) const
{
  // print event statistics
  G4cout
     << "   Tank: total energy: " 
     << std::setw(7) << G4BestUnit(Edep, "Energy")
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKEventAction::BeginOfEventAction(const G4Event* event)
{
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file where we save the data collected
  //
  string fileName = "SK" +  to_string(event->GetEventID()); ;
  analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKEventAction::EndOfEventAction(const G4Event* event)
{  
  // Get hits collections IDs (only once)
  if ( fTankHCID == -1 ) {
    fTankHCID 
      = G4SDManager::GetSDMpointer()->GetCollectionID("TankHitsCollection"); // use the name of the hits collection chosen in the SKDetectorConstruction
  }// you might want to construct more than one sensitive detector or hits collection, so you would need to creat more fnameHCID to collect their hit collections IDs

  // Get hits collections
  auto tankHC = GetHitsCollection(fTankHCID, event);
  auto tankHit = (*tankHC)[tankHC->entries()-1]; // if you have repeated copies of a geometrical object, you might associate only one sensitive detector to all of them, so you would have more than one entry, corresponding to each geometrical copy
 
  // Print per event (modulo n)
  //
  auto eventID = event->GetEventID();
  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "---> End of event: " << eventID << G4endl;     

    PrintEventStatistics(
      tankHit->GetEdep());
  }  

  // save histograms & ntuple
  //
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
   
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
