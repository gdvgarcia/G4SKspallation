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
/// \file G4SKspallationTankSD.cc
/// \brief Implementation of the G4SKspallationTankSD class

#include "G4SKspallationTankSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//added later
#include "G4SKspallationAnalysis.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SteppingManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4TrackVector.hh"
// maybe remove it
#include "G4DecayTable.hh"
#include "G4VDecayChannel.hh"
#include "G4RadioactiveDecay.hh"
#include "G4VProcess.hh"
#include "G4Tubs.hh"
#include <map>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4SKspallationTankSD::G4SKspallationTankSD(
                            const G4String& name, 
                            const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name),
   fHitsCollection(nullptr)
{
  collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4SKspallationTankSD::~G4SKspallationTankSD() 
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4SKspallationTankSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection 
    = new G4SKspallationTankHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce
  auto hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 

  // Create hits
  // tank
  fHitsCollection->insert(new G4SKspallationTankHit());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool G4SKspallationTankSD::ProcessHits(G4Step* step, 
                                     G4TouchableHistory*)
{ 

  //- check for newly produced particle. e.g. first step.
  if ( step->GetTrack()->GetCurrentStepNumber() != 1) return FALSE;      
  
  // Get hit accounting data for the tank
  auto hit = (*fHitsCollection)[0];
  if ( ! hit ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hit " ; 
    G4Exception("G4SKspallationTankSD::ProcessHits()",
      "MyCode0004", FatalException, msg);
  }         

  //----------------------------------------------
  //        Track
  //----------------------------------------------
  // get track to collect the data
  G4Track* Track = step->GetTrack();

  G4ParticleDefinition* thePartDef = Track->GetDefinition();
  G4String partType= thePartDef->GetParticleType();
  G4String name = thePartDef->GetParticleName();
  //const G4VProcess* process = Track->GetCreatorProcess();


  //----------------------------------------------
  //  Collect of information for unstable isotopes
  //  generated from an interaction with the target
  //----------------------------------------------

  //check if it is an ion
   if ( partType == "nucleus"){

  	

	
	//----------------------------------------------
	//   isotopes info
	//----------------------------------------------
	G4double Z = thePartDef->GetAtomicNumber();
	G4double A = thePartDef->GetAtomicMass();
	G4double x = Track->GetPosition().getX();
	G4double y = Track->GetPosition().getY();
	G4double z = Track->GetPosition().getZ();
	//G4String Process = process->GetProcessName();

  	// get analysis manager
  	auto analysisManager = G4AnalysisManager::Instance();
 
  	// fill histograms
  	analysisManager->FillH1(0, A);
  	analysisManager->FillH1(1, Z);
  	analysisManager->FillH2(0, Z, A);
  
  	// fill ntuple
  	analysisManager->FillNtupleDColumn(0, A);
  	analysisManager->FillNtupleDColumn(1, Z);
  	analysisManager->FillNtupleDColumn(2, x);
  	analysisManager->FillNtupleDColumn(3, y);
  	analysisManager->FillNtupleDColumn(4, z);
  	analysisManager->AddNtupleRow(); 
      
  	return true;
	}

	//check if it is a new particle and if it is a neutron
   if ( name == "neutron" ){

	
	//----------------------------------------------
	//   neutron info
	//----------------------------------------------
	G4double E = Track->GetVertexKineticEnergy();

  	// get analysis manager
  	auto analysisManager = G4AnalysisManager::Instance();
 
  	// fill histogram
  	analysisManager->FillH1(2, E);
 
      
  	return true;
	}

   //check if it is a electron
   if (name == "e-" ){
	//----------------------------------------------
	//   isotopes count
	//----------------------------------------------
  	auto N = 1.;
  	// Add values
  	hit->Add(N);
  	hitTotal->Add(N); 
	
	//----------------------------------------------
	//   neutron info
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

void G4SKspallationTankSD::EndOfEvent(G4HCofThisEvent*)
{
  if ( verboseLevel>1 ) { 
     auto nofHits = fHitsCollection->entries();
     G4cout
       << G4endl 
       << "-------->Hits Collection: in this event they are " << nofHits 
       << " hits in the tracker chambers: " << G4endl;
     for ( std::size_t i=0; i<nofHits; ++i ) (*fHitsCollection)[i]->Print();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
