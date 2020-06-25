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
/// \file SKEventAction.hh
/// \Definition of the SKEventAction class

#ifndef SKEventAction_h
#define SKEventAction_h 1

#include "G4UserEventAction.hh"

#include "SKHit.hh"

#include "globals.hh"

/// Event action class
///
/// In EndOfEventAction(), it prints the accumulated quantities of the energy 
/// deposit of all particles in the SK tank
/// stored in the hits collections. It also add new rolls to the ntuple and some
/// data to the histograms.

class SKEventAction : public G4UserEventAction
{
public:
  SKEventAction();
  virtual ~SKEventAction();

  virtual void  BeginOfEventAction(const G4Event* event);
  virtual void    EndOfEventAction(const G4Event* event);

  // vectors for first generation daugthers of the muon data
  std::vector<G4int>& 		GetvectorFirstGenCode()			{ return fvectorFirstGenCode; }
  std::vector<G4double>& 	GetvectorEfisrtGen()			{ return fvectorEfisrtGen; }
  // vectors for the isotopes data
  std::vector<G4int>& 		GetvectorA() 				{ return fvectorA; }
  std::vector<G4int>& 		GetvectorZ() 				{ return fvectorZ; }
  std::vector<G4double>& 	Getvectorx() 				{ return fvectorx; }
  std::vector<G4double>& 	Getvectory() 				{ return fvectory; }
  std::vector<G4double>& 	Getvectorz() 				{ return fvectorz; }
  std::vector<G4double>& 	GetvectorIsoDist() 			{ return fvectorIsoDist; }
  std::vector<G4int>& 		GetvectorIsoParentCode() 		{ return fvectorIsoParentCode; }
  std::vector<G4double>& 	GetvectorIniTime()			{ return fvectorIniTime; }
  std::vector<G4double>& 	GetvectorFinTime()			{ return fvectorFinTime; }
  std::vector<G4int>& 		GetvectorStable()			{ return fvectorStable; }
  std::vector<G4double>& 	GetvectorLifeTime()			{ return fvectorLifeTime; }
  // vectors for the isotopes' parents data
  std::vector<G4int>& 		GetvectorParentsCode() 			{ return fvectorParentsCode; }
  std::vector<G4int>& 		GetvectorAdaug() 			{ return fvectorAdaug; }
  std::vector<G4int>& 		GetvectorZdaug() 			{ return fvectorZdaug; }
  std::vector<G4double>& 	GetvectorEparent() 			{ return fvectorEparent; }
  // vectors for the path length data
  std::vector<G4int>& 		GetvectorPathLengthCode()		{ return fvectorPathLengthCode; }
  std::vector<G4double>& 	GetvectorPathLengthE()			{ return fvectorPathLengthE; }
  std::vector<G4double>& 	GetvectorStepLength()			{ return fvectorStepLength; }
  // vectors for the transverse distance data
  std::vector<G4int>& 		GetvectorTransParticlesCode() 		{ return fvectorTransParticlesCode; }
  std::vector<G4double>& 	GetvectorTransParticlesDist()		{ return fvectorTransParticlesDist; }
    
private:
  // methods
  SKHitsCollection* GetHitsCollection(G4int hcID,
                                            const G4Event* event) const;
  void PrintEventStatistics(G4double Edep, G4double Emu, G4double Lmu) const;
  
  // data members                   
  G4int  fTankHCID;
  // vectors for first generation daugthers of the muon data
  std::vector< G4int > 		fvectorFirstGenCode;
  std::vector< G4double >	fvectorEfisrtGen;
  // vectors for the isotopes data
  std::vector< G4int > 		fvectorA;
  std::vector< G4int > 		fvectorZ;
  std::vector< G4double > 	fvectorx;
  std::vector< G4double > 	fvectory;
  std::vector< G4double > 	fvectorz;
  std::vector< G4double > 	fvectorIsoDist;
  std::vector< G4int > 		fvectorIsoParentCode;
  std::vector< G4double > 	fvectorIniTime;
  std::vector< G4double > 	fvectorFinTime;
  std::vector< G4int > 		fvectorStable;
  std::vector< G4double > 	fvectorLifeTime;
  // vectors for the isotopes' parents data
  std::vector< G4int > 		fvectorParentsCode;
  std::vector< G4int > 		fvectorAdaug;
  std::vector< G4int > 		fvectorZdaug;
  std::vector< G4double > 	fvectorEparent;
  // vectors for the path length data
  std::vector< G4int > 		fvectorPathLengthCode;
  std::vector< G4double > 	fvectorPathLengthE;
  std::vector< G4double > 	fvectorStepLength;
  // vectors for the transverse distance data
  std::vector< G4int > 		fvectorTransParticlesCode;
  std::vector< G4double >	fvectorTransParticlesDist;
  
};
                     
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
