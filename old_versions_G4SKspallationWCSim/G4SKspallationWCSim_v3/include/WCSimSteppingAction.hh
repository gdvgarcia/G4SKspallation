#ifndef WCSimSteppingAction_h
#define WCSimSteppingAction_h 1

#include "G4Event.hh"
#include "G4UserSteppingAction.hh"
#include "G4ThreeVector.hh"

// SPALLATION ADDITION 
#include <vector>
#include <map>
#include "G4Track.hh"
#include "G4TrackVector.hh"

#include "WCSimDetectorConstruction.hh"
#include "SKIntermediateAccumulator.hh"

class G4HCofThisEvent;
class G4Event;
// SPALLATION ADDITION
class G4Step;
class WCSimDetectorConstruction;
class SKIntermediateAccumulator;


class WCSimSteppingAction : public G4UserSteppingAction
{

public:
  WCSimSteppingAction(const WCSimDetectorConstruction*, SKIntermediateAccumulator*); // addition of geometry and accumulator

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
  SKIntermediateAccumulator*  fAccumulator;  			// so one can handle the data to the event accumulator object which save the data for the entire event 
    //
    // Extra functions
    //
    // this is used to make a for loop in the secondaries per step vector - it fills the parents' vectors
    void loopForSecondariesEnergies(const std::vector<const G4Track*>* Secondaries , G4int n ,  
    G4double Energy , G4int Code); 								
    // this is used to make a for loop in the secondaries per track vector - it fills the fParentsCodes map with the parent particle codes	
    bool loopForSecondaries( const G4TrackVector* Secondaries , G4int n , G4int TrackID, G4int Code); 	
    // transverse distance to the muon track line - suppose the muon path to be an "infinite" line to do the calculation
    G4double transverseDistance( G4double x, G4double y, G4double z );	

};


#endif
