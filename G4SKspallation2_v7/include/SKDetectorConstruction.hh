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
/// \file SKDetectorConstruction.hh
/// \brief Definition of the SKDetectorConstruction class

#ifndef SKDetectorConstruction_h
#define SKDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;


/// Detector construction class to define materials and geometry.
/// The tank is a cylinder cointaining another cylinder inside which is the fiducial volume.
///
/// Four parameters define the geometry of the this simulation :
///
/// - the radius of the tank container,
/// - the height of the tank container,
/// - the radius of the fiducial volume,
/// - the height of the fiducial volume.

class SKDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    SKDetectorConstruction();
    virtual ~SKDetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();

    // get methods
    //
    const G4VPhysicalVolume* GetTankPV() 	const 	{ return fTankPV; 	};
    const G4VPhysicalVolume* GetFiducialPV() 	const 	{ return fFiducialPV; 	};
     
  private:
    // data members
    //
     
    G4VPhysicalVolume*   	fTankPV; 		// the tank physical volume
    G4VPhysicalVolume*   	fFiducialPV;      	// the fiducial physical volume
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

