#ifndef WCSimSteppingAction_h
#define WCSimSteppingAction_h 1

#include "G4Event.hh"
#include "G4UserSteppingAction.hh"
#include "G4ThreeVector.hh"

// SPALLATION ADDITION 
#include "G4Track.hh"
#include "G4TrackVector.hh"
#include "WCSimDetectorConstruction.hh"

class G4HCofThisEvent;
class G4Event;
// SPALLATION ADDITION
class G4Step;
class WCSimDetectorConstruction;


class WCSimSteppingAction : public G4UserSteppingAction
{

public:
  WCSimSteppingAction(const WCSimDetectorConstruction* detectorConstruction);

  ~WCSimSteppingAction();

  void UserSteppingAction(const G4Step*);

  G4int G4ThreeVectorToWireTime(G4ThreeVector *pos3d,
				G4ThreeVector lArPos,
				G4ThreeVector start,
				G4int i);
  
  void Distortion(G4double x,
		  G4double y);

  G4double FieldLines(G4double x,
		      G4double y,
		      G4int xy);

private:

  G4double ret[2];


  // SPALLATION ADDITION: for data retrieving
  const WCSimDetectorConstruction* fdetectorConstruction; 	// so one can check if the particle step is inside the desired volume 

  // this is used to make a for loop in the secondaries per step vector - it fills the parents' vectors
  void loopForSecondariesEnergies(const std::vector<const G4Track*>* Secondaries , G4int n ,  
  G4double Energy , G4int Code); 

};


#endif
