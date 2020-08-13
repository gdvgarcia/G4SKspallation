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
#include <iomanip>
#include <map>
#include <string> 
#include "cmath" 
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"

using namespace std; // SPALLATION ADDITION

// SPALLATION ADDITION
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WCSimSteppingAction::WCSimSteppingAction(
                      const WCSimDetectorConstruction* detectorConstruction, 
			SKIntermediateAccumulator* myAccumulator)
  : G4UserSteppingAction(), fdetectorConstruction(detectorConstruction), fAccumulator(myAccumulator)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WCSimSteppingAction::~WCSimSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WCSimSteppingAction::UserSteppingAction(const G4Step* aStep)
{
  // SPALLATION CHANGES : STARTS --------------------------------------------------------------------------------------------------

  // get volume of the current step and its name
  auto volumeIn = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume();

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

  // SPALLATION ADDITION TO KILL THE PARTICLE BEFORE TRANSPORTING INSIDE COMPLEX GEOMETRY
  // Making an exception for optical photons
  //if( name != "opticalphoton"){

  // get also volume of post step, because some borders are very complex and generate some small issues and this little change wont be 
  // a problem - I guess
  auto volumeOUT = aStep->GetPostStepPoint()->GetTouchableHandle()->GetVolume();

  // Information to kill the track
  G4String       VolumeInName = volumeIn->GetName(); 
  G4String       VolumeOUTName = volumeIn->GetName(); 
  G4Material *materialIn = volumeIn->GetLogicalVolume ()->GetMaterial ();
  G4Material *materialOUT = volumeIn->GetLogicalVolume ()->GetMaterial ();

  // if name says that we are in a PMT in the Cap or in a cell we kill it 
  if (VolumeInName.contains("PMT") ||
   	     VolumeInName.contains("Cap") ||
   	     VolumeInName.contains("Cell")||
   	     VolumeOUTName.contains("PMT")||
   	     VolumeOUTName.contains("Cap")||
   	     VolumeOUTName.contains("Cell"))
      aStep->GetTrack()->SetTrackStatus(fStopAndKill);

  // Kill the track if it is not inside the water or if it will leave the water
  if ( (materialIn != G4Material::GetMaterial("Water")) || (materialOUT != G4Material::GetMaterial("Water") ))
      aStep->GetTrack()->SetTrackStatus(fStopAndKill);

  //}


  // SPALLATION ADDITION : retrieving some data ------------------------------------------------------------------------------------

  // get desired volume
  auto volumeDesired = fdetectorConstruction->GetFiduPV();

  if( volumeIn == volumeDesired ){ 	// just process if it is inside the desired volume

 //----------------------------------------------
  //        Energy deposit
  //----------------------------------------------
  // getting the energy deposit for this step and adding it to the event energy deposit
  fAccumulator->Add(aStep->GetTotalEnergyDeposit());
  //-------------------------------------------------------------------------------------------- END ENERGY DEPOSIT



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


  
  if ( ( Track->GetCurrentStepNumber() == 1) ){ //- CHECK for newly produced particle. e.g. first step. or if it is an entering the desired volume
	// || ( aStep->IsFirstStepInVolume() ) ){  FIRST STEP IS HAVING SOME ISSUES PROBABLY DUE COMPLEX GEOMETRY		



  //----------------------------------------------
  //  Collect the initial time for the isotopes
  //----------------------------------------------
  if ( partType == "nucleus" ){  				//check if it is an ion
	// fill the initial time in the isotopes' initial time map
	fAccumulator->FillInitialTimes( TrackID , Track->GetGlobalTime());  	
  } //-------------------------------------------------------------------------------------------- END initial isotopes time



  } //-------------------------------------------------------------------------------------------- END check for newly produced particle 


  
  if ( ( aStep->IsLastStepInVolume() ) || ( Track->GetTrackStatus() == fStopAndKill ) ){ 	// CHECK if is the end of the track


  //----------------------------------------------
  //  Collect codes for isotopes' parents
  //----------------------------------------------
  // first take all the secondaries in a vector and its size - as it is the end of the track, we have all the secondary produced by the current particle in the secondaries vector
  const G4TrackVector* Secondaries = aStep->GetSecondary();
  G4int size = Secondaries->size();

  if ( size > 0){ 						// check if we have secondaries
	// we loop over secondaries and fill in the parents ID map
	for( int i = 0; i < size && !(loopForSecondaries( Secondaries , i ,TrackID, thePartDef->GetPDGEncoding() ) ); ++i){;}
  } //-------------------------------------------------------------------------------------------- END isotopes' parents' codes



  //-----------------------------------------------------
  //  Collect information for isotopes
  //  generated from an interaction with the water tank
  //-----------------------------------------------------
  if ( partType == "nucleus" ){  				//check if it is an ion
	// isotope information
	G4double 	xiso = 	Track->GetPosition().getX();
	G4double 	yiso = 	Track->GetPosition().getY();
	G4double 	ziso = 	Track->GetPosition().getZ();
	G4int 		A =	thePartDef->GetAtomicMass();
	G4int 		Z =	thePartDef->GetAtomicNumber();

	// fill the 2D histogram
  	auto analysisManager = G4AnalysisManager::Instance(); 	// get analysis manager
  	analysisManager->FillH2(0, Z, A);

	// filling the isotope information
	fAccumulator->FillIso( A, Z, xiso, yiso , ziso , transverseDistance( xiso, yiso, ziso ), fAccumulator->GetParentCode(TrackID),
 	fAccumulator->GetInitialTime(TrackID), Track->GetGlobalTime(), thePartDef->GetPDGStable(), thePartDef->GetPDGLifeTime()); 
  } //-------------------------------------------------------------------------------------------- END ISOTOPE
  } //-------------------------------------------------------------------------------------------- END CHECK IF IS THE END OF THE STEP



  //----------------------------------------------
  //  Collect information for the muon
  //----------------------------------------------
  if ( TrackID == 1 ){  					// CHECK if it is our initial muon 

	//----------------------------------------------
  	//        Path Length
  	//----------------------------------------------
  	// getting the step length and adding it to the event path length
  	fAccumulator->AddL(aStep->GetStepLength());
	

  	if ( ( aStep->IsLastStepInVolume() ) || 
	( Track->GetTrackStatus() == fStopAndKill ) ){ 		// CHECK if it is leaving the tank or finishing the track
		//----------------------------------------------
  		//        Muon info
  		//----------------------------------------------
		G4double x = Track->GetPosition().getX();
		G4double y = Track->GetPosition().getY();
		G4double z = Track->GetPosition().getZ();

		// Filling the muon final conditions
		fAccumulator->FillFin( x, y, z);
  	}

  	if( ( Track->GetCurrentStepNumber() == 1 ) )	{	// CHECK if it is entering the tank or is the first step
	//|| ( aStep->IsFirstStepInVolume() ) ){ FIRST STEP IS HAVING SOME ISSUES PROBABLY DUE COMPLEX GEOMETRY	

		//----------------------------------------------
  		//        Muon info
  		//----------------------------------------------
		G4double x = Track->GetPosition().getX();
		G4double y = Track->GetPosition().getY();
		G4double z = Track->GetPosition().getZ();

  		// fill histogram
		auto analysisManager = G4AnalysisManager::Instance();
  		analysisManager->FillH1(1, CurrentE);

        	// Filling the muon initial conditions
		fAccumulator->FillIni(Track->GetVertexKineticEnergy(), x, y, z);
	}
  } //-------------------------------------------------------------------------------------------- END MUON 
  } // ------- END VOLUME

  // SPALLATION END : retrieving some data ------------------------------------------------------------------------------------
  // SPALLATION CHANGES : END ------------------------------------------------------------------------------------


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
// SPALLATION ADDITION : EXTRA FUNCTIONS
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WCSimSteppingAction::loopForSecondariesEnergies(const std::vector<const G4Track*>* Secondaries , 
	G4int n , G4double Energy , G4int Code  ) 	// it fills the parents ntuple
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
	fAccumulator->FillIsoPar(Code, A, Z, Energy);
  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  bool WCSimSteppingAction::loopForSecondaries( const G4TrackVector* Secondaries , G4int n , 
  	G4int TrackID, G4int Code ) 			// it fills the fParentsCodes map 
{
  // getting the information about the secondary
  G4Track* secondary = Secondaries->at(n);
  G4ParticleDefinition* secPartDef = secondary->GetDefinition();
  G4String secpartType = secPartDef->GetParticleType();

  // if we find an ion we save the parent information
  if ( secpartType == "nucleus"){
	// Filling the map
	fAccumulator->FillParentsCodes(TrackID, Code);
	
	return true;		// so we can exit the for loop
  }
  return false; 		// so we keep in the for loop
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double WCSimSteppingAction::transverseDistance( G4double x, G4double y, G4double z )// transverse distance to the muon track line - suppose an "infinite" line for the muon path line
{
  // making the vector from initial to particle and initial to final to calculate the cross product and have the transverse distance
  G4double xp = x - fAccumulator->GetxIni();
  G4double yp = y - fAccumulator->GetyIni();
  G4double zp = z - fAccumulator->GetzIni();
  G4double xm = fAccumulator->GetxFin() - fAccumulator->GetxIni() ;
  G4double ym = fAccumulator->GetyFin() - fAccumulator->GetyIni() ;
  G4double zm = fAccumulator->GetzFin() - fAccumulator->GetzIni() ;

  // taking the cross product
  G4double xc, yc, zc; 
  xc = yp * zm - zp * ym; 
  yc = zp * xm - xp * zm; 
  zc = xp * ym - yp * xm;

  // taking the norms 
  G4double normMu	= sqrt( xm*xm + ym*ym + zm*zm );
  G4double normc 	= sqrt( xc*xc + yc*yc + zc*zc );

  G4double dis = normc/normMu; 	//the trasverse distance

  return dis;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
