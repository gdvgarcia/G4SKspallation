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
/// \file SKSD.cc
/// \brief Implementation of the SKSD class

#include "SKSD.hh"
#include "SKAnalysis.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"
#include "SKHit.hh"

#include "Randomize.hh"
#include <iomanip>
#include <map>
#include <string> 

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKSD::SKSD(
                            const G4String& name, 
                            const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name),
   fHitsCollection(nullptr),
   fEventID(0)
{
  collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKSD::~SKSD() 
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection 
    = new SKHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce
  auto hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 

  // Create hits
  fHitsCollection->insert(new SKHit());

  // keeping track of the event 
  fEventID = fEventID + 1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SKSD::ProcessHits(G4Step* step, 
                                     G4TouchableHistory*)
{
  // energy deposit
  auto edep = step->GetTotalEnergyDeposit();  

  // Get hit accounting data
  auto hit = (*fHitsCollection)[0];

  // Add values
  hit->Add(edep);

  //----------------------------------------------
  //        Track
  //----------------------------------------------
  // get track to collect the data
  G4Track* Track = step->GetTrack();

  //- check for newly produced particle. e.g. first step.
  if ( Track->GetCurrentStepNumber() != 1){return false;}

  G4ParticleDefinition* thePartDef = Track->GetDefinition();
  G4String partType= thePartDef->GetParticleType();
  G4String name = thePartDef->GetParticleName();
  const G4VProcess* process = Track->GetCreatorProcess();

  
  //----------------------------------------------
  //  Collect information for isotopes
  //  generated from an interaction with the water tank
  //----------------------------------------------
  //check if it is an ion
  if ( partType == "nucleus" ){
	//----------------------------------------------
	//   isotopes info
	//----------------------------------------------
	G4double Z = thePartDef->GetAtomicNumber();
	G4double A = thePartDef->GetAtomicMass();
	G4double x = Track->GetPosition().getX();
	G4double y = Track->GetPosition().getY();
	G4double z = Track->GetPosition().getZ();
	G4String Process = process->GetProcessName();

  	// get analysis manager
  	auto analysisManager = G4AnalysisManager::Instance();
 
  	// fill histograms
  	analysisManager->FillH2(0, Z, A);
  
  	// fill ntuple
  	analysisManager->FillNtupleDColumn(0, A);
  	analysisManager->FillNtupleDColumn(1, Z);
  	analysisManager->FillNtupleDColumn(2, x);
  	analysisManager->FillNtupleDColumn(3, y);
  	analysisManager->FillNtupleDColumn(4, z);
  	analysisManager->FillNtupleSColumn(5, Process);
	analysisManager->FillNtupleIColumn(6, fEventID);
  	analysisManager->AddNtupleRow(); 
      
  	return true;
  }

  //----------------------------------------------
  //  Collect information for other particles
  //  generated from an interaction with the water tank
  //----------------------------------------------

  //check if it is a neutron
  if ( name == "neutron" ){
	//----------------------------------------------
	//   neutron info
	//----------------------------------------------
	G4double E = Track->GetVertexKineticEnergy();

  	// get analysis manager
  	auto analysisManager = G4AnalysisManager::Instance();
 
  	// fill histogram
  	analysisManager->FillH1(0, E);
      
  	return true;
  }

  //check if it is a electron or a positron
  if (name == "e-" || name == "e+" ){
	//----------------------------------------------
	//   electron/positron info
	//----------------------------------------------
	G4double E = Track->GetVertexKineticEnergy();

  	// get analysis manager
  	auto analysisManager = G4AnalysisManager::Instance();
 
  	// fill histogram
  	analysisManager->FillH1(1, E);
 
  	return true;
   }

     //check if it is a pion + or a pion -
  if (name == "pi+" || name == "pi-" ){
	//----------------------------------------------
	//   charged pion info
	//----------------------------------------------
	G4double E = Track->GetVertexKineticEnergy();

  	// get analysis manager
  	auto analysisManager = G4AnalysisManager::Instance();
 
  	// fill histogram
  	analysisManager->FillH1(2, E);
 
  	return true;
   } 

     //check if it is a neutral pion
  if (name == "pi0" ){
	//----------------------------------------------
	//    neutral pion info
	//----------------------------------------------
	G4double E = Track->GetVertexKineticEnergy();

  	// get analysis manager
  	auto analysisManager = G4AnalysisManager::Instance();
 
  	// fill histogram
  	analysisManager->FillH1(3, E);
 
  	return true;
   } 



  return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKSD::EndOfEvent(G4HCofThisEvent*)
{
  if ( verboseLevel>1 ) { 
     auto nofHits = fHitsCollection->entries();
     G4cout
       << G4endl 
       << "-------->Hits Collection: in this event they are " << nofHits 
       << " hits in the sensitive volume / tank : " << G4endl;
     (*fHitsCollection)[0]->Print();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
