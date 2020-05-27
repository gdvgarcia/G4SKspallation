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
#include "G4TrackVector.hh"
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
#include "cmath" 

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"


using namespace std;

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

  // Leave the maps empty for starting a new event
  fParents.clear();
  //fParentsEnergy.clear(); - DELETE IF CAN NOT SOLVE IT
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SKSD::ProcessHits(G4Step* step, 
                                     G4TouchableHistory*)
{
  // Get hit for accounting the data
  auto hit = (*fHitsCollection)[0];



  //----------------------------------------------
  //        Energy deposit
  //----------------------------------------------
  // getting the energy deposit for this step and adding it to the hit
  hit->Add(step->GetTotalEnergyDeposit());



  //----------------------------------------------
  //        Track
  //----------------------------------------------
  // getting track to collect the data and some of its informtion about the current particle
  G4Track* Track = step->GetTrack();
  G4ParticleDefinition* thePartDef = Track->GetDefinition();
  G4String partType= thePartDef->GetParticleType();
  G4String name = thePartDef->GetParticleName();
  G4int TrackID = Track->GetTrackID();
  G4String Process = "None"; // initialize
  if ( TrackID != 1 ){ // avoiding getting the process for the initial muon
	const G4VProcess* process = Track->GetCreatorProcess();
  	Process = process->GetProcessName();
  } //---------------------------------------------- END TRACK



  //----------------------------------------------
  //  Collect energies for isotopes' parents
  //----------------------------------------------
  // first take all the secondaries in a vector and its size
  const std::vector<const G4Track*>* Secondaries2 = step->GetSecondaryInCurrentStep();
  G4int size2 = Secondaries2->size();

  if ( size2 > 0){ // check if we have secondaries
	// we loop over secondaries and save the information in a ntuple
	for( int i = 0; i < size2; ++i){
		loopForSecondariesEnergies( Secondaries2 , i , // TrackID,  
			Track->GetKineticEnergy() , Process , name );
	}
  } //---------------------------------------------- END isotopes' parents energies


  
  if ( ( step->IsLastStepInVolume() ) || 
( Track->GetTrackStatus() == fStopAndKill ) ){ // CHECK if is the end of the track

  //----------------------------------------------
  //  Collect names for isotopes' parents
  //----------------------------------------------
  // first take all the secondaries in a vector and its size
  G4TrackVector* Secondaries = step->GetfSecondary();
  G4int size = Secondaries->size();

  if ( size > 0){ // check if we have secondaries
	// we loop over secondaries and fill in the map
	for( int i = 0; i < size && !(loopForSecondaries( Secondaries , i ,TrackID, name ) ); ++i){;}
  } //---------------------------------------------- END isotopes' parents names



  //----------------------------------------------
  //  Collect information for other particles
  //  generated from an interaction with the water tank
  //----------------------------------------------
  if ( name == "neutron" || name == "gamma" || name == "e-" || name == "e+" || name == "pi+" || name == "pi-" || name == "pi0" ){ //check if it is a wanted particle
	transverseFill( Track->GetPosition().getX(), Track->GetPosition().getY() , name ); // calculate transverse distance and fill it in the ntuple
  } //---------------------------------------------- END check if it is a wanted particle  

  } //---------------------------------------------- END CHECK IF IS THE END OF THE STEP



  //----------------------------------------------
  //  Collect information for the muon
  //----------------------------------------------
  if ( TrackID == 1 ){  // CHECK if it is our initial muon 

  	if ( ( step->IsLastStepInVolume() ) || 
	( Track->GetTrackStatus() == fStopAndKill ) ){ // CHECK if it is leaving the tank or finishing the track
		muonFill( Track->GetKineticEnergy(), Track->GetPosition().getX() , 
		Track->GetPosition().getY() , Track->GetPosition().getZ() , 
		name ); // fill muon data in the muon ntuple
  	}

  	if( ( Track->GetCurrentStepNumber() == 1 ) || ( step->IsFirstStepInVolume() ) ){ // CHECK if it is entering the tank or is the first step
		muonFill( Track->GetKineticEnergy(), Track->GetPosition().getX() , 
		Track->GetPosition().getY() , Track->GetPosition().getZ() , 
		name ); // fill muon data in the muon ntuple

        	// Filling the hit with the muon initial energy
		hit->Fill(Track->GetVertexKineticEnergy());
	}

  	return true;
  } 
  //---------------------------------------------- END MUON 



  
  if ( ( Track->GetCurrentStepNumber() == 1) || ( step->IsFirstStepInVolume() ) ){ //- CHECK for newly produced particle. e.g. first step. or if it is an entering
  
  //----------------------------------------------
  //  Collect information for isotopes
  //  generated from an interaction with the water tank
  //----------------------------------------------
  if ( partType == "nucleus" ){  //check if it is an ion
  	isotopeFill( name ,thePartDef->GetAtomicMass(), thePartDef->GetAtomicNumber(),
	Track->GetPosition().getX() , Track->GetPosition().getY() , 
    	Track->GetPosition().getZ() , Track->GetParentID(), Process ); // fill isotope data in the isotope ntuple
      
  	return true;
  } //---------------------------------------------- END ISOTOPE 

  } //---------------------------------------------- END check for newly produced particle 



  //----------------------------------------------
  //  Collect information for other particles
  //  generated from an interaction with the water tank
  //----------------------------------------------
  if ( ( name == "neutron" || name == "gamma" || name == "e-" || 
  name == "e+" || name == "pi+" || name == "pi-" || name == "pi0" ) 
  && ( Track->GetKineticEnergy() > 0.9 )){ //check if it is a wanted particle and if its energy is greater than 0.9 MeV
	particlesFill( name , Track->GetKineticEnergy(), Track->GetStepLength() ); // fill particles data in the particles ntuple
  	
  	return true;
   } //---------------------------------------------- END check if it is a wanted particle



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
// FUNCTIONS TO CLEAN THE CODE
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKSD::loopForSecondariesEnergies(
             const std::vector<const G4Track*>* Secondaries , G4int n , //G4int TrackID,  
G4double Energy , G4String Process, G4String name ) // it fills the fParentsEnergy map - ACTUALLY THE PARENTS NTUPLE
{
  // getting the information about the secondary
  const G4Track* secondary = Secondaries->at(n);
  G4ParticleDefinition* secPartDef = secondary->GetDefinition();
  G4String secpartType = secPartDef->GetParticleType();
  G4String secname = secPartDef->GetParticleName();
  //G4int secID = secondary->GetTrackID(); - DELETE IF CAN NOT SOLVE IT

  // if we find a ion we save the parent information
  if ( secpartType == "nucleus"){
	// Filling the map
	//fParentsEnergy[TrackID][secID] = Energy; - DELETE IF CAN NOT SOLVE IT

	//----------------------------------------------
	//   daughter isotope info
	//----------------------------------------------
	G4int 	 Z = secPartDef->GetAtomicNumber();
	G4int 	 A = secPartDef->GetAtomicMass();

  	// get analysis manager
  	auto analysisManager = G4AnalysisManager::Instance();
 
  	// fill ntuple
  	analysisManager->FillNtupleSColumn(4, 0, name);
  	analysisManager->FillNtupleIColumn(4, 1, A);
  	analysisManager->FillNtupleIColumn(4, 2, Z);
  	analysisManager->FillNtupleSColumn(4, 3, Process);
  	analysisManager->FillNtupleDColumn(4, 4, Energy);
	analysisManager->FillNtupleIColumn(4, 5, fEventID);
  	analysisManager->AddNtupleRow(4); 
  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  bool SKSD::loopForSecondaries( G4TrackVector* Secondaries , G4int n , G4int TrackID, 
G4String name ) // it fills the fParents map with the names
{
  // getting the information about the secondary
  G4Track* secondary = Secondaries->at(n);
  G4ParticleDefinition* secPartDef = secondary->GetDefinition();
  G4String secpartType = secPartDef->GetParticleType();

  // if we find a ion we save the parent information
  if ( secpartType == "nucleus"){
	// Filling the map
	fParents[TrackID] = name;
	
	return true; // so we can exit the for loop
  }
  return false; // so we keep in the for loop
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void SKSD::transverseFill( G4double x, G4double y , G4String name ) // transverse distance calculator and ntuple filler
{
  // for now we calculate the transverse as the distance from cilinder axis
  G4double dis = sqrt(x*x + y*y);	

  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
 
  // fill ntuple
  analysisManager->FillNtupleSColumn(3, 0, name);
  analysisManager->FillNtupleDColumn(3, 1, dis);
  analysisManager->FillNtupleIColumn(3, 2, fEventID);
  analysisManager->AddNtupleRow(3); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void SKSD::muonFill( G4double E, G4double x , G4double y , G4double z , G4String name ) // muon ntuple filler
{
  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
 
  // fill ntuple
  analysisManager->FillNtupleSColumn(1, 0, name);
  analysisManager->FillNtupleDColumn(1, 1, E);
  analysisManager->FillNtupleDColumn(1, 2, x);
  analysisManager->FillNtupleDColumn(1, 3, y);
  analysisManager->FillNtupleDColumn(1, 4, z);
  analysisManager->FillNtupleIColumn(1, 5, fEventID);
  analysisManager->AddNtupleRow(1); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void SKSD::isotopeFill( G4String name , G4int A, G4int Z, G4double x , G4double y , 
    G4double z , G4int    ParentID,  G4String Process ) // isotopes ntuple filler
{
  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
 
  // fill histograms
  analysisManager->FillH2(0, Z, A);
  
  // fill ntuple
  analysisManager->FillNtupleSColumn(0, 0, name);
  analysisManager->FillNtupleIColumn(0, 1, A);
  analysisManager->FillNtupleIColumn(0, 2, Z);
  analysisManager->FillNtupleDColumn(0, 3, x);
  analysisManager->FillNtupleDColumn(0, 4, y);
  analysisManager->FillNtupleDColumn(0, 5, sqrt(x*x + y*y)); // remember to put z as Z again WARNING
  analysisManager->FillNtupleSColumn(0, 6, Process);
  analysisManager->FillNtupleSColumn(0, 7, fParents[ParentID]);
  //analysisManager->FillNtupleDColumn(0, 8, fParentsEnergy[ParentID][TrackID]); - DELETE IF CAN NOT SOLVE IT
  //analysisManager->FillNtupleDColumn(0, 7, E);
  //analysisManager->FillNtupleDColumn(0, 8, Px);
  //analysisManager->FillNtupleDColumn(0, 9, Py);
  //analysisManager->FillNtupleDColumn(0, 10, Pz);
  analysisManager->FillNtupleIColumn(0, 8, fEventID);
  analysisManager->AddNtupleRow(0); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void SKSD::particlesFill( G4String name , G4double Energy , G4double StepLength )  // particles ntuple filler
{
  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
 
  // fill ntuple
  analysisManager->FillNtupleSColumn(2, 0, name);
  analysisManager->FillNtupleDColumn(2, 1, Energy);
  analysisManager->FillNtupleDColumn(2, 2, StepLength);
  analysisManager->FillNtupleIColumn(2, 3, fEventID);
  analysisManager->AddNtupleRow(2); 
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

