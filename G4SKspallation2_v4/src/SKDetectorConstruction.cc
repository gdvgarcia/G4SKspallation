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
/// \file SKDetectorConstruction.cc
/// \Implementation of the SKDetectorConstruction class
/// To analyse data only from the fiducial volume you have to uncomment lines
/// that create the fiducial volume and change the Sensitive Detector logical volume.

#include "SKDetectorConstruction.hh"
#include "SKSD.hh"
#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "G4SDManager.hh"
#include "G4AutoDelete.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKDetectorConstruction::SKDetectorConstruction()
: G4VUserDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKDetectorConstruction::~SKDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* SKDetectorConstruction::Construct()
{  

  // Get nist material manager to use G4 materials list
  G4NistManager* nist = G4NistManager::Instance();

  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = false;


  // Parameters of the SK tank
  G4double SKinnerRadius = 0.*m;
  G4double SKouterRadius = 19.5*m; //SK radius
  G4double SKhz = 42.*m; //SK height
  G4double SKstartAngle = 0.*deg;
  G4double SKspanningAngle = 360.*deg;
  G4Material* tank_mat = nist->FindOrBuildMaterial("G4_WATER"); //taking the material from the G4 materials list

  // Parameters of the fiducial volume
  //G4double FinnerRadius = 0.*m;
  //G4double FouterRadius = 14.9*m; //Fiducial volume radius
  //G4double Fhz = 32.*m; //Fiducial volume height
  //G4double FstartAngle = 0.*deg;
  //G4double FspanningAngle = 360.*deg;
  //G4Material* fidu_mat = tank_mat;

  // Parameters of the world
  G4double worldLength = 1.2*SKhz; //size of the world - just need to be larger than the tank
  G4Material* world_mat = tank_mat; //we set water because we do not care about the outside material and we have already created water


  //
  // World
  //
  // Solid and Logical volumes
  G4Box* worldS = new G4Box("WorldS", worldLength/2,worldLength/2,worldLength/2); 
  G4LogicalVolume* worldLV = new G4LogicalVolume(worldS, world_mat,"WorldLV"); 
  // Must place the World Physical volume unrotated at (0,0,0). 
  G4VPhysicalVolume* worldPV
    = new G4PVPlacement(
                 0,               // no rotation
                 G4ThreeVector(), // at (0,0,0)
                 worldLV,         // its logical volume
                 "World",         // its name
                 0,               // its mother  volume
                 false,           // no boolean operations
                 0,               // copy number
                 checkOverlaps); // checking overlaps 
                     
  //     
  // SK tank
  //
  //Solid and Logical volumes
  G4Tubs* solidTank = new G4Tubs("TankS", SKinnerRadius, SKouterRadius, 0.5*SKhz, SKstartAngle, SKspanningAngle);
  G4LogicalVolume* logicTank = new G4LogicalVolume(solidTank, tank_mat, "TankLV");
  // Physical volume             
  new G4PVPlacement(0, G4ThreeVector(), logicTank, "Tank",worldLV, false, 0, checkOverlaps);

  //     
  // Fiducial volume
  //
  //Solid and Logical volumes        
  //G4Tubs* solidFidu = new G4Tubs("FiducialS", FinnerRadius, FouterRadius,0.5* Fhz, FstartAngle, FspanningAngle);         
  //G4LogicalVolume* logicFidu = new G4LogicalVolume(solidFidu, fidu_mat, "FiducialLV");
  // Physical volume             
  //new G4PVPlacement(0, G4ThreeVector(), logicFidu, "Fiducial", logicTank, false, 0, checkOverlaps);

  
  // Visualization attributes
  G4VisAttributes* worldVisAtt = new G4VisAttributes(G4Colour(0.5451,0.2706,0.0745,0.35));
  G4VisAttributes* tankVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0,0.36));
  //G4VisAttributes* fiduVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0,0.365));
  worldLV        ->SetVisAttributes(worldVisAtt);
  logicTank      ->SetVisAttributes(tankVisAtt);
  //logicFidu      ->SetVisAttributes(fiduVisAtt);


  //
  //Always return the physical World
  //
  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKDetectorConstruction::ConstructSDandField()
{
  // If you want to choose the level of verbosity
  // G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

  // 
  // Creating the sensitive detector for the tank
  //
  G4VSensitiveDetector* tankSD = new SKSD("TankSD", "TankHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(tankSD);
  SetSensitiveDetector("TankLV", tankSD);

  // 
  // Creating the sensitive detector for the fiducial volume - comment the above lines
  // so we only have one detector
  //auto fiduSD = new SKSD("FiduSD", "TankHitsCollection");
  //----------------------------------------------------G4SDManager::GetSDMpointer()->AddNewDetector(fiduSD);
  //SetSensitiveDetector("FiducialLV", fiduSD);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
