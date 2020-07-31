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
/// \file SKHit.cc
/// \Implementation of the SKHit class

#include "SKHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>
#include <vector>
#include <string> 

G4ThreadLocal G4Allocator<SKHit>* SKHitAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKHit::SKHit()
 : G4VHit(),
   // muon
   fEdep(0.), 			///< Energy deposit in the sensitive volume
   fEmu(0.),			///< Initial kinetic energy of the muon
   fLmu(0.),			///< Path lenght of the muon
   fxIni(0.), 			///< x initial position  of the muon
   fyIni(0.),			///< y initial position  of the muon
   fzIni(0.),			///< z initial position  of the muon
   fxFin(0.),			///< x initial position  of the muon
   fyFin(0.),			///< y initial position  of the muon
   fzFin(0.)			///< z initial position  of the muon
{ 
    //first generation daugther of the muon
    fFirstGenCode.clear();	///< vector of the first generation particles codes
    fEfisrtGen.clear();		///< vector of energies of first generation particles
    // isotopes
    fA.clear(); 		///< vector of atomic masses
    fZ.clear(); 		///< vector of atomic numbers
    fxIso.clear(); 		///< vector of x positions of the isotopes
    fyIso.clear();		///< vector of y positions of the isotopes
    fzIso.clear();		///< vector of z positions of the isotopes
    fIsoDist.clear(); 		///< vector of transverse distances of the isotopes
    fIsoParentCode.clear(); 	///< vector of the isotopes' parents codes
    fIniTime.clear();		///< vector of initial times of the isotopes
    fFinTime.clear(); 		///< vector of final times of the isotopes
    fStable.clear(); 		///< vector of the stability(1) or instability(0) of isotopes
    fLifeTime.clear();		///< vector of life times of the isotopes
    // isotopes' parents
    fParentsCode.clear();	///< vector of the parent particle codes
    fAdaug.clear(); 		//< vector of atomic masses of the daughters
    fZdaug.clear(); 		///< vector of atomic numbers of the daughters
    fEparent.clear();		///< vector of energy of parents
    // path length
    fPathLengthCode.clear();	///< vector of the particle codes 
    fPathLengthE.clear();	///< vector of energies of particles
    fStepLength.clear();	//< vector of steps lengths of particles for a given energy
    // transverse distance
    fTransCode.clear();		///< vector of the particle codes 
    fTransDist.clear(); 	///< vector of the transverse distance
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKHit::~SKHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKHit::SKHit(const SKHit& right)
  : G4VHit()
{
    // muon
    fEdep  		= right.fEdep;		///< Energy deposit in the sensitive volume
    fEmu 		= right.fEmu;		///< Initial kinetic energy of the muon
    fLmu		= right.fLmu;		///< Path lenght of the muon
    fxIni 		= right.fxIni;		///< x initial position  of the muon
    fyIni 		= right.fyIni;		///< y initial position  of the muon
    fzIni 		= right.fzIni;		///< z initial position  of the muon
    fxFin 		= right.fxFin;		///< x initial position  of the muon
    fyFin 		= right.fyFin;		///< y initial position  of the muon
    fzFin 		= right.fzFin;		///< z initial position  of the muon
    //first generation daugther of the muon
    fFirstGenCode 	= right.fFirstGenCode;	///< vector of the first generation particles codes
    fEfisrtGen 		= right.fEfisrtGen;		///< vector of energies of first generation particles
    // isotopes
    fA 			= right.fA; 		///< vector of atomic masses
    fZ 			= right.fZ; 		///< vector of atomic numbers
    fxIso 		= right.fxIso;		///< vector of x positions of the isotopes
    fyIso 		= right.fyIso;		///< vector of y positions of the isotopes
    fzIso 		= right.fzIso;		///< vector of z positions of the isotopes
    fIsoDist 		= right.fIsoDist; 	///< vector of transverse distances of the isotopes
    fIsoParentCode 	= right.fIsoParentCode; ///< vector of the isotopes' parents codes
    fIniTime 		= right.fIniTime;	///< vector of initial times of the isotopes
    fFinTime 		= right.fFinTime;	///< vector of final times of the isotopes
    fStable 		= right.fStable; 	///< vector of the stability(1) or instability(0) of isotopes
    fLifeTime		= right.fLifeTime;	///< vector of life times of the isotopes
    // isotopes' parents
    fParentsCode 	= right.fParentsCode;	///< vector of the parent particle codes
    fAdaug		= right.fAdaug; 	//< vector of atomic masses of the daughters
    fZdaug 		= right.fZdaug; 	///< vector of atomic numbers of the daughters
    fEparent 		= right.fEparent;	///< vector of energy of parents
    // path length
    fPathLengthCode 	= right.fPathLengthCode;///< vector of the particle codes 
    fPathLengthE 	= right.fPathLengthE;	///< vector of energies of particles
    fStepLength 	= right.fStepLength;	//< vector of steps lengths of particles for a given energy
    // transverse distance
    fTransCode 		= right.fTransCode;	///< vector of the particle codes 
    fTransDist 		= right.fTransDist;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const SKHit& SKHit::operator=(const SKHit& right)
{
    // muon
    fEdep  		= right.fEdep;		///< Energy deposit in the sensitive volume
    fEmu 		= right.fEmu;		///< Initial kinetic energy of the muon
    fLmu		= right.fLmu;		///< Path lenght of the muon
    fxIni 		= right.fxIni;		///< x initial position  of the muon
    fyIni 		= right.fyIni;		///< y initial position  of the muon
    fzIni 		= right.fzIni;		///< z initial position  of the muon
    fxFin 		= right.fxFin;		///< x initial position  of the muon
    fyFin 		= right.fyFin;		///< y initial position  of the muon
    fzFin 		= right.fzFin;		///< z initial position  of the muon
    //first generation daugther of the muon
    fFirstGenCode 	= right.fFirstGenCode;	///< vector of the first generation particles codes
    fEfisrtGen 		= right.fEfisrtGen;		///< vector of energies of first generation particles
    // isotopes
    fA 			= right.fA; 		///< vector of atomic masses
    fZ 			= right.fZ; 		///< vector of atomic numbers
    fxIso 		= right.fxIso;		///< vector of x positions of the isotopes
    fyIso 		= right.fyIso;		///< vector of y positions of the isotopes
    fzIso 		= right.fzIso;		///< vector of z positions of the isotopes
    fIsoDist 		= right.fIsoDist; 	///< vector of transverse distances of the isotopes
    fIsoParentCode 	= right.fIsoParentCode; ///< vector of the isotopes' parents codes
    fIniTime 		= right.fIniTime;	///< vector of initial times of the isotopes
    fFinTime 		= right.fFinTime;	///< vector of final times of the isotopes
    fStable 		= right.fStable; 	///< vector of the stability(1) or instability(0) of isotopes
    fLifeTime		= right.fLifeTime;	///< vector of life times of the isotopes
    // isotopes' parents
    fParentsCode 	= right.fParentsCode;	///< vector of the parent particle codes
    fAdaug		= right.fAdaug; 	//< vector of atomic masses of the daughters
    fZdaug 		= right.fZdaug; 	///< vector of atomic numbers of the daughters
    fEparent 		= right.fEparent;	///< vector of energy of parents
    // path length
    fPathLengthCode 	= right.fPathLengthCode;///< vector of the particle codes 
    fPathLengthE 	= right.fPathLengthE;	///< vector of energies of particles
    fStepLength 	= right.fStepLength;	//< vector of steps lengths of particles for a given energy
    // transverse distance
    fTransCode 		= right.fTransCode;	///< vector of the particle codes 
    fTransDist 		= right.fTransDist;

  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SKHit::operator==(const SKHit& right) const
{
  return ( this == &right ) ? true : false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKHit::Print()
{
  G4cout
     << "Edep: " 
     << std::setw(7) << G4BestUnit(fEdep,"Energy")
     << G4endl;
   G4cout
     << "KinEmu: " 
     << std::setw(7) << G4BestUnit(fEmu,"Energy")
     << G4endl;
   G4cout
     << "Lmu: " 
     << std::setw(7) << G4BestUnit(fLmu,"Lenght")
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
