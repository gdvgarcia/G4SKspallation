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
/// \Implementation of the SKEventAction class

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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKEventAction::SKEventAction()
 : G4UserEventAction(),
   fTankHCID(-1)
{
  // vectors for first generation daugthers of the muon data
  fvectorFirstGenCode.clear();
  fvectorEfisrtGen.clear();
  // vectors for the isotopes data
  fvectorA.clear();
  fvectorZ.clear();
  fvectorx.clear();
  fvectory.clear();
  fvectorz.clear();
  fvectorIsoDist.clear();
  fvectorIsoParentCode.clear();
  fvectorIniTime.clear();
  fvectorFinTime.clear();
  fvectorStable.clear();
  // vectors for the isotopes' parents data
  fvectorParentsCode.clear();
  fvectorAdaug.clear();
  fvectorZdaug.clear();
  fvectorEparent.clear();
  // vectors for the path length data
  fvectorPathLengthCode.clear();
  fvectorPathLengthE.clear();
  fvectorStepLength.clear();
  // vectors for the transverse distance data
  fvectorTransParticlesCode.clear();
  fvectorTransParticlesDist.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKEventAction::~SKEventAction()
{}

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
                              G4double Edep, G4double Emu, G4double Lmu ) const
{
  // print event statistics
  G4cout
     << "   Tank: total energy deposited : " 
     << std::setw(7) << G4BestUnit(Edep, "Energy")
     << G4endl;
  G4cout
     << "   Tank: muon path length : " 
     << std::setw(7) << G4BestUnit(Lmu, "Length")
     << G4endl;

  // print the initial conditions
  G4cout
     << "   Muon initial kinetic energy : " 
     << std::setw(7) << G4BestUnit(Emu, "Energy")
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKEventAction::BeginOfEventAction(const G4Event* /*event*/)
{}

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
      tankHit->GetEdep(), tankHit->GetEmu(), tankHit->GetLmu());
  }  

  // Get the data from the hits collection to the variables of the ntuple
  //
  // vectors for first generation daugthers of the muon data
  fvectorFirstGenCode 		= tankHit->GetFirstGenCode();
  fvectorEfisrtGen		= tankHit->GetEfisrtGen();
  // vectors for the isotopes data
  fvectorA 			= tankHit->GetA();
  fvectorZ 			= tankHit->GetZ();
  fvectorx 			= tankHit->GetxIso();
  fvectory 			= tankHit->GetyIso();
  fvectorz			= tankHit->GetzIso();
  fvectorIsoDist		= tankHit->GetIsoDist();
  fvectorIsoParentCode		= tankHit->GetIsoParentCode();
  fvectorIniTime		= tankHit->GetIniTime();
  fvectorFinTime		= tankHit->GetFinTime();
  fvectorStable			= tankHit->GetStable();
  // vectors for the isotopes' parents data
  fvectorParentsCode		= tankHit->GetParentsCode();
  fvectorAdaug			= tankHit->GetAdaug();
  fvectorZdaug			= tankHit->GetZdaug();
  fvectorEparent		= tankHit->GetEparent();
  // vectors for the path length data
  fvectorPathLengthCode		= tankHit->GetPathLengthCode();
  fvectorPathLengthE		= tankHit->GetPathLengthE();
  fvectorStepLength		= tankHit->GetStepLength();
  // vectors for the transverse distance data
  fvectorTransParticlesCode	= tankHit->GetTransParticlesCode();
  fvectorTransParticlesDist	= tankHit->GetTransParticlesDist();
  
  // Fill the histograms and the ntuple
  //
  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
  // fill histograms
  analysisManager->FillH1(0, tankHit->GetEdep());
  analysisManager->FillH1(1, tankHit->GetEmu());
  analysisManager->FillH1(2, tankHit->GetLmu());
  // fill the ntuple single variables
  analysisManager->FillNtupleDColumn( 0, 	tankHit->GetEmu());
  analysisManager->FillNtupleDColumn( 1, 	tankHit->GetxIni()); 
  analysisManager->FillNtupleDColumn( 2, 	tankHit->GetyIni());
  analysisManager->FillNtupleDColumn( 3, 	tankHit->GetzIni());
  analysisManager->FillNtupleDColumn( 4, 	tankHit->GetEdep());
  analysisManager->FillNtupleDColumn( 5, 	tankHit->GetxFin());
  analysisManager->FillNtupleDColumn( 6,	tankHit->GetyFin());
  analysisManager->FillNtupleDColumn( 7, 	tankHit->GetzFin());
  analysisManager->FillNtupleDColumn( 8, 	tankHit->GetLmu());
  analysisManager->FillNtupleIColumn( 9, 	fvectorFirstGenCode.size());
  analysisManager->FillNtupleIColumn(12, 	fvectorA.size());
  analysisManager->FillNtupleIColumn(23, 	fvectorParentsCode.size());
  analysisManager->FillNtupleIColumn(28, 	fvectorPathLengthCode.size());
  analysisManager->FillNtupleIColumn(32, 	fvectorTransParticlesCode.size());
  // adding the Row
  analysisManager->AddNtupleRow(); 


  // cleaning the vectors
  // vectors for first generation daugthers of the muon data
  fvectorFirstGenCode.clear();
  fvectorEfisrtGen.clear();
  // vectors for the isotopes data
  fvectorA.clear();
  fvectorZ.clear();
  fvectorx.clear();
  fvectory.clear();
  fvectorz.clear();
  fvectorIsoDist.clear();
  fvectorIsoParentCode.clear();
  fvectorIniTime.clear();
  fvectorFinTime.clear();
  fvectorStable.clear();
  // vectors for the isotopes' parents data
  fvectorParentsCode.clear();
  fvectorAdaug.clear();
  fvectorZdaug.clear();
  fvectorEparent.clear();
  // vectors for the path length data
  fvectorPathLengthCode.clear();
  fvectorPathLengthE.clear();
  fvectorStepLength.clear();
  // vectors for the transverse distance data
  fvectorTransParticlesCode.clear();
  fvectorTransParticlesDist.clear();
   
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
