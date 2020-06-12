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
/// \Implementation of the SKSD class

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
   fHitsCollection(nullptr)
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

  // Leave the muon vecotr empty for starting a new event
  fMuonPositions.clear();

  // Leave the maps empty for starting a new event
  fParentsCodes.clear();
  fInitialTimes.clear();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SKSD::ProcessHits(G4Step* step, 
                                     G4TouchableHistory*) // this is called every time a step is made inside the sensitive detector
{
  // Get the hit for accounting the data
  auto hit = (*fHitsCollection)[0];



  //----------------------------------------------
  //        Energy deposit
  //----------------------------------------------
  // getting the energy deposit for this step and adding it to the hit
  hit->Add(step->GetTotalEnergyDeposit());



  //----------------------------------------------
  //        Track
  //----------------------------------------------
  // getting track object to collect some data and some of its informtion about its particle
  G4Track* Track = step->GetTrack();
  G4ParticleDefinition* thePartDef = Track->GetDefinition();
  G4String partType= thePartDef->GetParticleType();
  G4String name = thePartDef->GetParticleName();
  G4int TrackID = Track->GetTrackID(); 
  //---------------------------------------------- END TRACK



  //----------------------------------------------
  //  Collect energies for isotopes' parents
  //----------------------------------------------
  // first take all the secondaries in a vector and its size
  const std::vector<const G4Track*>* Secondaries2 = step->GetSecondaryInCurrentStep();
  G4int size2 = Secondaries2->size();

  if ( size2 > 0){ // check if we have secondaries
	// we loop over secondaries and save information in the hit object
	for( int i = 0; i < size2; ++i){
		loopForSecondariesEnergies( Secondaries2 , i , // TrackID,  
			Track->GetKineticEnergy() , hit, thePartDef->GetPDGEncoding() );
	}
  } //---------------------------------------------- END isotopes' parents energies


  
  if ( ( Track->GetCurrentStepNumber() == 1) || ( step->IsFirstStepInVolume() ) ){ //- CHECK for newly produced particle. e.g. first step. or if it is an entering

	  
  //----------------------------------------------
  //  Collect information for primarly daughters of the muon
  //----------------------------------------------
  if(Track->GetParentID() == 1){ // check if its parents is our initial muon 
	// fill the first generation daughters information in the hit
	hit->FillFirstGen(Track->GetVertexKineticEnergy() , thePartDef->GetPDGEncoding());
  }



  //----------------------------------------------
  //  Collect the initial time for the isotopes
  //----------------------------------------------
  if ( partType == "nucleus" ){  //check if it is an ion
	fInitialTimes[TrackID] = Track->GetGlobalTime();  // fill the initial time in the map
  }

  } //---------------------------------------------- END check for newly produced particle 


  
  if ( ( step->IsLastStepInVolume() ) || ( Track->GetTrackStatus() == fStopAndKill ) ){ // CHECK if is the end of the track


  //----------------------------------------------
  //  Collect codes for isotopes' parents
  //----------------------------------------------
  // first take all the secondaries in a vector and its size - as it is the end of the track, we have all the secondary produced by the current particles
  G4TrackVector* Secondaries = step->GetfSecondary();
  G4int size = Secondaries->size();

  if ( size > 0){ // check if we have secondaries
	// we loop over secondaries and fill in the map
	for( int i = 0; i < size && !(loopForSecondaries( Secondaries , i ,TrackID, thePartDef->GetPDGEncoding() ) ); ++i){;}
  } //---------------------------------------------- END isotopes' parents codes



  //-----------------------------------------------------
  //  Collect information for isotopes
  //  generated from an interaction with the water tank
  //-----------------------------------------------------
  if ( partType == "nucleus" ){  //check if it is an ion
	// isotope information
	G4double 	xiso = 	Track->GetPosition().getX();
	G4double 	yiso = 	Track->GetPosition().getY();
	G4double 	ziso = 	Track->GetPosition().getZ();
	G4int 		A =	thePartDef->GetAtomicMass();
	G4int 		Z =	thePartDef->GetAtomicNumber();

	// fill the 2D histogram
  	auto analysisManager = G4AnalysisManager::Instance(); // get analysis manager
  	analysisManager->FillH2(0, Z, A);

	// filling the hit with the isotope information
	hit->FillIso( A, Z, xiso, yiso , ziso , transverseDistance( xiso, yiso, ziso ), fParentsCodes[TrackID],
 	fInitialTimes[TrackID], Track->GetGlobalTime(), thePartDef->GetPDGStable()); 
      
  	return true;
  } //---------------------------------------------- END ISOTOPE



  //----------------------------------------------
  //  Collect transverse distance for  particles
  //  generated from an interaction with the water tank
  //----------------------------------------------
  if ( partType != "nucleus" &&  TrackID != 1 ){ //check if it is a wanted particle - not isotope and not the initial muon
	// fill the transverse distance hit
	hit->FillTransverse( thePartDef->GetPDGEncoding(), transverseDistance( Track->GetPosition().getX(),
  	Track->GetPosition().getY(), Track->GetPosition().getZ() ) );
  } //---------------------------------------------- END check if it is a wanted particle  

   

  } //---------------------------------------------- END CHECK IF IS THE END OF THE STEP



  //----------------------------------------------
  //  Collect information for the muon
  //----------------------------------------------
  if ( TrackID == 1 ){  // CHECK if it is our initial muon 

	//----------------------------------------------
  	//        Path Length
  	//----------------------------------------------
  	// getting the step length and adding it to the hit
  	hit->AddL(step->GetStepLength());
	

  	if ( ( step->IsLastStepInVolume() ) || 
	( Track->GetTrackStatus() == fStopAndKill ) ){ // CHECK if it is leaving the tank or finishing the track
		//----------------------------------------------
  		//        Muon info
  		//----------------------------------------------
		G4double x = Track->GetPosition().getX();
		G4double y = Track->GetPosition().getY();
		G4double z = Track->GetPosition().getZ();
		
		// Setting muon position vector for transverse distance calculation
		fMuonPositions.push_back (x);
		fMuonPositions.push_back (y);
		fMuonPositions.push_back (z);

		// Filling the hit with the muon final conditions
		hit->FillFin( x, y, z);
		
  		return true;
  	}

  	if( ( Track->GetCurrentStepNumber() == 1 ) || ( step->IsFirstStepInVolume() ) ){ // CHECK if it is entering the tank or is the first step
		//----------------------------------------------
  		//        Muon info
  		//----------------------------------------------
		G4double x = Track->GetPosition().getX();
		G4double y = Track->GetPosition().getY();
		G4double z = Track->GetPosition().getZ();
		
		// Setting muon position vector for transverse distance calculation
		fMuonPositions.push_back (x);
		fMuonPositions.push_back (y);
		fMuonPositions.push_back (z);

        	// Filling the hit with the muon initial conditions
		hit->FillIni(Track->GetVertexKineticEnergy(), x, y, z);
		
  		return true;
	}
  } 
  //---------------------------------------------- END MUON 



  //----------------------------------------------
  //  Collect path length information for particles
  //  generated from an interaction with the water tank
  //----------------------------------------------
  if ( partType != "nucleus" &&  TrackID != 1 && ( Track->GetKineticEnergy() > 0.9 )){ //check if it is a wanted particle - not isotope or initial muon - and if its energy is greater than 0.9 MeV - because too many particles otherwise
	// fill the path length hit
	hit->FillPathLength( thePartDef->GetPDGEncoding(), Track->GetKineticEnergy(), Track->GetStepLength());
  	
  	return true;
   } //---------------------------------------------- END check if it is a wanted particle



  return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKSD::EndOfEvent(G4HCofThisEvent*)
{
  if ( verboseLevel>1 ) { // text about the event hit information only if asked - high verbosity
     auto nofHits = fHitsCollection->entries();
     G4cout
       << G4endl 
       << "-------->Hits Collection: in this event they are " << nofHits 
       << " hits in the sensitive volume / tank : " << G4endl;
     (*fHitsCollection)[0]->Print();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Extra functions
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKSD::loopForSecondariesEnergies(const std::vector<const G4Track*>* Secondaries , 
	G4int n , G4double Energy , SKHit* hit, G4int Code  ) // it fills the parents ntuple
{
  // getting the information about the secondary
  const G4Track* secondary = Secondaries->at(n);
  G4ParticleDefinition* secPartDef = secondary->GetDefinition();
  G4String secpartType = secPartDef->GetParticleType();

  // if we find a ion we save the parent information
  if ( secpartType == "nucleus"){
	//----------------------------------------------
	//   daughter isotope info
	//----------------------------------------------
	G4int 	 Z = secPartDef->GetAtomicNumber();
	G4int 	 A = secPartDef->GetAtomicMass();

	// filling the hit with the isotopes' parent data
	hit->FillIsoPar(Code, A, Z, Energy);
  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  bool SKSD::loopForSecondaries( G4TrackVector* Secondaries , G4int n , G4int TrackID, G4int Code ) // it fills the fParentsCodes map 
{
  // getting the information about the secondary
  G4Track* secondary = Secondaries->at(n);
  G4ParticleDefinition* secPartDef = secondary->GetDefinition();
  G4String secpartType = secPartDef->GetParticleType();

  // if we find an ion we save the parent information
  if ( secpartType == "nucleus"){
	// Filling the map
	fParentsCodes[TrackID] = Code;
	
	return true; // so we can exit the for loop
  }
  return false; // so we keep in the for loop
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double SKSD::transverseDistance( G4double x, G4double y, G4double z )// transverse distance to the muon track line - suppose an "infinite" line 
{
  // making the vector from initial to particle and initial to final to calculate the cross product and have the transverse distance
  G4double xp = x - fMuonPositions[0];
  G4double yp = y - fMuonPositions[1];
  G4double zp = z - fMuonPositions[2];
  G4double xm = fMuonPositions[3] - fMuonPositions[0];
  G4double ym = fMuonPositions[4] - fMuonPositions[1];
  G4double zm = fMuonPositions[5] - fMuonPositions[2];

  // taking the cross product
  G4double xc, yc, zc; 
  xc = yp * zm - zp * ym; 
  yc = zp * xm - xp * zm; 
  zc = xp * ym - yp * xm;

  // taking the norms 
  G4double normMu	= sqrt( xm*xm + ym*ym + zm*zm );
  G4double normc 	= sqrt( xc*xc + yc*yc + zc*zc );

  G4double dis = normc/normMu; //the trasverse distance

  return dis;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
