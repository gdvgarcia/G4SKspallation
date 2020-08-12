#include <stdlib.h>
#include <stdio.h>

#include "WCSimSteppingAction.hh"

#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4VParticleChange.hh"
#include "G4SteppingVerbose.hh"
#include "G4SteppingManager.hh"
#include "G4PVParameterised.hh"
#include "G4PVReplica.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"

// SPALATION ADDITION 
#include "G4TrackVector.hh"
#include "g4root.hh"
#include "WCSimDetectorConstruction.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "cmath" 
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

// SPALLATION ADDITION
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WCSimSteppingAction::WCSimSteppingAction(
                      const WCSimDetectorConstruction* detectorConstruction)
  : G4UserSteppingAction(), fdetectorConstruction(detectorConstruction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WCSimSteppingAction::~WCSimSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WCSimSteppingAction::UserSteppingAction(const G4Step* aStep)
{
  // SPALLATION ADDITION : retrieving some data

  // get volume of the current step
  auto volumeIn = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  // get desired volume
  auto volumeDesired = fdetectorConstruction->GetFiduPV();

  if( volumeIn == volumeDesired ){ 	// just process if it is inside the desired volume

  //----------------------------------------------
  //        Track
  //----------------------------------------------
  // getting track object to collect some data and some of its information about its particle
  G4Track* Track = aStep->GetTrack();
  G4ParticleDefinition* thePartDef = Track->GetDefinition();
  G4String partType= thePartDef->GetParticleType();
  G4String name = thePartDef->GetParticleName();
  G4int TrackID = Track->GetTrackID(); 
  G4double CurrentE = aStep->GetPreStepPoint()->GetKineticEnergy(); // current energy of the particle - before the interaction step
  //-------------------------------------------------------------------------------------------- END TRACK

  //----------------------------------------------
  //  Collect energies for isotopes' parents
  //----------------------------------------------
  // first take all the secondaries in a vector and its size
  const std::vector<const G4Track*>* Secondaries2 = aStep->GetSecondaryInCurrentStep();
  G4int size2 = Secondaries2->size();

  if ( size2 > 0){ 						// check if we have secondaries
	// we loop over secondaries and save information
	for( int i = 0; i < size2; ++i){
		loopForSecondariesEnergies( Secondaries2 , i , 
			CurrentE , thePartDef->GetPDGEncoding() );
	}
  } //-------------------------------------------------------------------------------------------- END isotopes' parents energies

  //----------------------------------------------
  //  Collect information for the muon
  //----------------------------------------------
  if ( TrackID == 1 ){  					// CHECK if it is our initial muon 
  	if ( ( aStep->IsLastStepInVolume() ) || 
	( Track->GetTrackStatus() == fStopAndKill ) ){ 		// CHECK if it is leaving the tank or finishing the track
		G4double mudE = Track->GetVertexKineticEnergy() - CurrentE;
		// Fill the histograms and the ntuple
  		//
  		// get analysis manager
  		auto analysisManager = G4AnalysisManager::Instance();
  		// fill histogram
  		analysisManager->FillH1(0, mudE);

 		G4cout << " Energy deposited of the muon is " << mudE << G4endl;
  	}

  	if( ( Track->GetCurrentStepNumber() == 1 ) // || ( aStep->IsFirstStepInVolume() ) PROBLEM WITH GEOMETRY, SAYING FIRST STEP FOR TOO MANY
	){ 			// CHECK if it is entering the tank or is the first step
  		// Fill the histograms and the ntuple
  		//
  		// get analysis manager
  		auto analysisManager = G4AnalysisManager::Instance();
  		// fill histogram
  		analysisManager->FillH1(1, CurrentE);

 		G4cout << " Initial muon energy is " << CurrentE << G4endl;
	}
  } //-------------------------------------------------------------------------------------------- END MUON 
  } // ------- END VOLUME


  //DISTORTION must be used ONLY if INNERTUBE or INNERTUBEBIG has been defined in BidoneDetectorConstruction.cc
  
  const G4Event* evt = G4RunManager::GetRunManager()->GetCurrentEvent();

  const G4Track* track       = aStep->GetTrack();
  G4VPhysicalVolume* volume  = track->GetVolume();


  G4SDManager* SDman   = G4SDManager::GetSDMpointer();
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();

  //debugging 
//  G4Track* theTrack = aStep->GetTrack();
//  const G4DynamicParticle* aParticle = theTrack->GetDynamicParticle();
//  G4ThreeVector aMomentum = aParticle->GetMomentumDirection();
//  G4double vx = aMomentum.x();
//  G4int ix = std::isnan(vx);
//  if(ix != 0){
//    G4cout << " PROBLEM! " << theTrack->GetCreatorProcess()->GetProcessName() <<
//  std::flush << G4endl;
//  }
  
}


G4int WCSimSteppingAction::G4ThreeVectorToWireTime(G4ThreeVector *pos3d,
						    G4ThreeVector lArPos,
						    G4ThreeVector start,
						    G4int i)
{
  G4double x0 = start[0]-lArPos[0];
  G4double y0 = start[1]-lArPos[1];
//   G4double y0 = 2121.3;//mm
  G4double z0 = start[2]-lArPos[2];

  G4double dt=0.8;//mm
//   G4double midt = 2651.625;
  G4double pitch = 3;//mm
//   G4double midwir = 1207.10;
  G4double c45 = 0.707106781;
  G4double s45 = 0.707106781;

  G4double w1;
  G4double w2;
  G4double t;

//   G4double xField(0.);
//   G4double yField(0.);
//   G4double zField(0.);

//   if(detector->getElectricFieldDistortion())
//     {
//       Distortion(pos3d->getX(),
// 		 pos3d->getY());
      
//       xField = ret[0];
//       yField = ret[1];
//       zField = pos3d->getZ();
      
//       w1 = (int)(((zField+z0)*c45 + (x0-xField)*s45)/pitch); 
//       w2 = (int)(((zField+z0)*c45 + (x0+xField)*s45)/pitch); 
//       t = (int)(yField+1);

//       //G4cout<<" x orig "<<pos3d->getX()<<" y orig "<<(pos3d->getY()+y0)/dt<<G4endl;
//       //G4cout<<" x new "<<xField<<" y new "<<yField<<G4endl;
//     }
//   else 
//     {
      
  w1 = (int) (((pos3d->getZ()+z0)*c45 + (x0-pos3d->getX())*s45)/pitch); 
  w2 = (int)(((pos3d->getZ()+z0)*c45 + (x0+pos3d->getX())*s45)/pitch); 
  t  = (int)((pos3d->getY()+y0)/dt +1);
//     }

  if (i==0)
    return (int)w1;
  else if (i==1)
    return (int)w2;
  else if (i==2)
    return (int)t;
  else return 0;
} 


void WCSimSteppingAction::Distortion(G4double /*x*/,G4double /*y*/)
{
 
//   G4double theta,steps,yy,y0,EvGx,EvGy,EField,velocity,tSample,dt;
//   y0=2121.3;//mm
//   steps=0;//1 mm steps
//   tSample=0.4; //micros
//   dt=0.8;//mm
//   LiquidArgonMedium medium;  
//   yy=y;
//   while(y<y0 && y>-y0 )
//     {
//       EvGx=FieldLines(x,y,1);
//       EvGy=FieldLines(x,y,2);
//       theta=atan(EvGx/EvGy);
//       if(EvGy>0)
// 	{
// 	  x+=sin(theta);
// 	  y+=cos(theta);
// 	}
//       else
// 	{
// 	  y-=cos(theta);
// 	  x-=sin(theta);
// 	}
//       EField=sqrt(EvGx*EvGx+EvGy*EvGy);//kV/mm
//       velocity=medium.DriftVelocity(EField*10);// mm/microsec
//       steps+=1/(tSample*velocity);

//       //G4cout<<" step "<<steps<<" x "<<x<<" y "<<y<<" theta "<<theta<<" Gx "<<eventaction->Gx->Eval(x,y)<<" Gy "<<eventaction->Gy->Eval(x,y)<<" EField "<<EField<<" velocity "<<velocity<<G4endl;
//     }

//   //numbers
//   //EvGx=FieldLines(0,1000,1);
//   //EvGy=FieldLines(0,1000,2);
//   //EField=sqrt(EvGx*EvGx+EvGy*EvGy);//kV/mm
//   //velocity=medium.DriftVelocity(EField*10);// mm/microsec
//   //G4double quenching;
//   //quenching=medium.QuenchingFactor(2.1,EField*10);
//   //G4cout<<" Gx "<<EvGx<<" Gy "<<EvGy<<" EField "<<EField<<" velocity "<<velocity<<" quenching "<<quenching<<G4endl;


//   ret[0]=x;
//   if(yy>0)
//     ret[1]=2*y0/dt -steps;
//   else
//     ret[1]=steps; 
}


double WCSimSteppingAction::FieldLines(G4double /*x*/,G4double /*y*/,G4int /*coord*/)
{ //0.1 kV/mm = field
  //G4double Radius=302;//mm
//   G4double Radius=602;//mm
//   if(coord==1) //x coordinate
//     return  (0.1*(2*Radius*Radius*x*abs(y)/((x*x+y*y)*(x*x+y*y))));
//   else //y coordinate
//     return 0.1*((abs(y)/y)*(1-Radius*Radius/((x*x+y*y)*(x*x+y*y))) + abs(y)*(2*Radius*Radius*y/((x*x+y*y)*(x*x+y*y))));
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// SPALLATION ADDITION : functions for parents data
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WCSimSteppingAction::loopForSecondariesEnergies(const std::vector<const G4Track*>* Secondaries , 
	G4int n , G4double Energy , G4int Code  ) 	// it fills the parents ntuple
{
  // getting the information about the secondary
  const G4Track* secondary = Secondaries->at(n);
  G4ParticleDefinition* secPartDef = secondary->GetDefinition();
  G4String secpartType = secPartDef->GetParticleType();
  G4String Secname = secPartDef->GetParticleName();


  // if we find a ion we save the parent information
  if ( secpartType == "nucleus" ){
  	// Fill the histograms and the ntuple
  	//
  	// get analysis manager
  	auto analysisManager = G4AnalysisManager::Instance();
 
	// fill the ntuple
  	analysisManager->FillNtupleIColumn( 0, 	Code);
 	analysisManager->FillNtupleDColumn( 1, 	Energy); 
  	// adding the Row
  	analysisManager->AddNtupleRow();
  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
