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
/// \file SKSteppingAction.cc
/// \brief Implementation of the SKSteppingAction class

#include "SKSteppingAction.hh"
#include "SKEventAction.hh"
#include "SKDetectorConstruction.hh"
#include "SKAnalysis.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4TrackVector.hh"
#include "G4RunManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

#include <iomanip>
#include <map>
#include <string> 
#include "cmath" 

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKSteppingAction::SKSteppingAction(
                      const SKDetectorConstruction* detectorConstruction,
                      SKEventAction* eventAction)
  : G4UserSteppingAction(),
    fDetConstruction(detectorConstruction),
    fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKSteppingAction::~SKSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKSteppingAction::UserSteppingAction(const G4Step* step)
{
  // Collect information at each step if inside the volume
  //

  // get volume of the current step
  auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  // get desired volume
  auto volume2 = fDetConstruction->GetFiducialPV();;
  if( !volume2 ){ // if fiducial does not exist we have a zero
	volume2 = fDetConstruction->GetTankPV();
  }

  if( volume == volume2 ){ // just process if it is inside the desired volume

  //----------------------------------------------
  //        Energy deposit
  //----------------------------------------------
  // getting the energy deposit for this step and adding it to the event energy deposit
  fEventAction->Add(step->GetTotalEnergyDeposit());



  //----------------------------------------------
  //        Track
  //----------------------------------------------
  // getting track object to collect some data and some of its informtion about its particle
  G4Track* Track = step->GetTrack();
  G4ParticleDefinition* thePartDef = Track->GetDefinition();
  G4String partType= thePartDef->GetParticleType();
  G4String name = thePartDef->GetParticleName();
  G4int TrackID = Track->GetTrackID(); 
  G4double CurrentE = step->GetPreStepPoint()->GetKineticEnergy(); // current energy of the particle
  //---------------------------------------------- END TRACK



  //----------------------------------------------
  //  Collect energies for isotopes' parents
  //----------------------------------------------
  // first take all the secondaries in a vector and its size
  const std::vector<const G4Track*>* Secondaries2 = step->GetSecondaryInCurrentStep();
  G4int size2 = Secondaries2->size();

  if ( size2 > 0){ // check if we have secondaries
	// we loop over secondaries and save information
	for( int i = 0; i < size2; ++i){
		loopForSecondariesEnergies( Secondaries2 , i , 
			CurrentE , thePartDef->GetPDGEncoding() );
	}
  } //---------------------------------------------- END isotopes' parents energies


  
  if ( ( Track->GetCurrentStepNumber() == 1) || ( step->IsFirstStepInVolume() ) ){ //- CHECK for newly produced particle. e.g. first step. or if it is an entering

	  
  //----------------------------------------------
  //  Collect information for primarly daughters of the muon
  //----------------------------------------------
  if(Track->GetParentID() == 1){ // check if its parents is our initial muon 
	// fill the first generation daughters information
	fEventAction->FillFirstGen(Track->GetVertexKineticEnergy() , thePartDef->GetPDGEncoding());
  }



  //----------------------------------------------
  //  Collect the initial time for the isotopes
  //----------------------------------------------
  if ( partType == "nucleus" ){  //check if it is an ion
	fEventAction->FillInitialTimes( TrackID , Track->GetGlobalTime());  // fill the initial time in the map
  }

  } //---------------------------------------------- END check for newly produced particle 


  
  if ( ( step->IsLastStepInVolume() ) || ( Track->GetTrackStatus() == fStopAndKill ) ){ // CHECK if is the end of the track


  //----------------------------------------------
  //  Collect codes for isotopes' parents
  //----------------------------------------------
  // first take all the secondaries in a vector and its size - as it is the end of the track, we have all the secondary produced by the current particles
  const G4TrackVector* Secondaries = step->GetSecondary();
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

	// filling the isotope information
	fEventAction->FillIso( A, Z, xiso, yiso , ziso , transverseDistance( xiso, yiso, ziso ), fEventAction->GetParentCode(TrackID),
 	fEventAction->GetInitialTime(TrackID), Track->GetGlobalTime(), thePartDef->GetPDGStable(), thePartDef->GetPDGLifeTime()); 
      
  	
  } //---------------------------------------------- END ISOTOPE



  //----------------------------------------------
  //  Collect transverse distance for  particles
  //  generated from an interaction with the water tank
  //----------------------------------------------
  if ( partType != "nucleus" &&  TrackID != 1 ){ //check if it is a wanted particle - not isotope and not the initial muon
	// fill the transverse distance
	fEventAction->FillTransverse( thePartDef->GetPDGEncoding(), transverseDistance( Track->GetPosition().getX(),
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
  	// getting the step length and adding it to the event path length
  	fEventAction->AddL(step->GetStepLength());
	

  	if ( ( step->IsLastStepInVolume() ) || 
	( Track->GetTrackStatus() == fStopAndKill ) ){ // CHECK if it is leaving the tank or finishing the track
		//----------------------------------------------
  		//        Muon info
  		//----------------------------------------------
		G4double x = Track->GetPosition().getX();
		G4double y = Track->GetPosition().getY();
		G4double z = Track->GetPosition().getZ();

		// Filling the muon final conditions
		fEventAction->FillFin( x, y, z);
		
  		
  	}

  	if( ( Track->GetCurrentStepNumber() == 1 ) || ( step->IsFirstStepInVolume() ) ){ // CHECK if it is entering the tank or is the first step
		//----------------------------------------------
  		//        Muon info
  		//----------------------------------------------
		G4double x = Track->GetPosition().getX();
		G4double y = Track->GetPosition().getY();
		G4double z = Track->GetPosition().getZ();

        	// Filling the muon initial conditions
		fEventAction->FillIni(Track->GetVertexKineticEnergy(), x, y, z);
		
  		
	}
  } 
  //---------------------------------------------- END MUON 



  //----------------------------------------------
  //  Collect path length information for particles
  //  generated from an interaction with the water tank
  //----------------------------------------------
  if ( partType != "nucleus" &&  TrackID != 1 && ( CurrentE > 0.9 )){ //check if it is a wanted particle - not isotope or initial muon - and if its energy is greater than 0.9 MeV - because too many particles otherwise
	// fill the path length
	fEventAction->FillPathLength( thePartDef->GetPDGEncoding(), CurrentE, Track->GetStepLength());
  	
  	
   } //---------------------------------------------- END check if it is a wanted particle



  
  }

  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Extra functions
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKSteppingAction::loopForSecondariesEnergies(const std::vector<const G4Track*>* Secondaries , 
	G4int n , G4double Energy , G4int Code  ) // it fills the parents ntuple
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

	// filling with the isotopes' parent data
	fEventAction->FillIsoPar(Code, A, Z, Energy);
  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  bool SKSteppingAction::loopForSecondaries( const G4TrackVector* Secondaries , G4int n , G4int TrackID, G4int Code ) // it fills the fParentsCodes map 
{
  // getting the information about the secondary
  G4Track* secondary = Secondaries->at(n);
  G4ParticleDefinition* secPartDef = secondary->GetDefinition();
  G4String secpartType = secPartDef->GetParticleType();

  // if we find an ion we save the parent information
  if ( secpartType == "nucleus"){
	// Filling the map
	fEventAction->FillParentsCodes(TrackID, Code);
	
	 return true;// so we can exit the for loop
  }
  return false; // so we keep in the for loop
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double SKSteppingAction::transverseDistance( G4double x, G4double y, G4double z )// transverse distance to the muon track line - suppose an "infinite" line 
{
  // making the vector from initial to particle and initial to final to calculate the cross product and have the transverse distance
  G4double xp = x - fEventAction->GetxIni();
  G4double yp = y - fEventAction->GetyIni();
  G4double zp = z - fEventAction->GetzIni();
  G4double xm = fEventAction->GetxFin() - fEventAction->GetxIni();
  G4double ym = fEventAction->GetyFin() - fEventAction->GetyIni();
  G4double zm = fEventAction->GetzFin() - fEventAction->GetzIni();

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
