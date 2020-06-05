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
/// \brief Implementation of the SKHit class

#include "SKHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<SKHit>* SKHitAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKHit::SKHit()
 : G4VHit(),
   // muon
   fEdep(0.),
   fEmu(0.),
   fLmu(0.),
   fxIni(0.), 	
   fyIni(0.),
   fzIni(0.),
   fxFin(0.),
   fyFin(0.),
   fzFin(0.)
{ 
// isotopes
   fA.clear();
   fZ.clear(); 
   fxIso.clear(); 
   fyIso.clear();
   fzIso.clear();
   fTransIso.clear(); 
   fParentsCodes.clear(); 
   // isotopes' parents
   fParticleCode.clear();
   fAdaug.clear();
   fZdaug.clear(); 	
   fEPar.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKHit::~SKHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKHit::SKHit(const SKHit& right)
  : G4VHit()
{
   // muon
   fEdep  		= right.fEdep;
   fEmu 		= right.fEmu;
   fLmu			= right.fLmu;
   fxIni 		= right.fxIni;
   fyIni 		= right.fyIni;
   fzIni 		= right.fzIni;
   fxFin 		= right.fxFin;
   fyFin 		= right.fyFin;
   fzFin 		= right.fzFin;
   // isotopes
   fA 			= right.fA;
   fZ 			= right.fZ; 
   fxIso 		= right.fxIso; 
   fyIso 		= right.fyIso;
   fzIso 		= right.fzIso;
   fTransIso 		= right.fTransIso; 
   fParentsCodes 	= right.fParentsCodes; 
   // isotopes' parents
   fParticleCode 	= right.fParticleCode;	
   fAdaug 		= right.fAdaug;	
   fZdaug 		= right.fAdaug;	
   fEPar 		= right.fEPar;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const SKHit& SKHit::operator=(const SKHit& right)
{
   // muon
   fEdep  		= right.fEdep;
   fEmu 		= right.fEmu;
   fLmu			= right.fLmu;
   fxIni 		= right.fxIni;
   fyIni 		= right.fyIni;
   fzIni 		= right.fzIni;
   fxFin 		= right.fxFin;
   fyFin 		= right.fyFin;
   fzFin 		= right.fzFin;
   // isotopes
   fA 			= right.fA;
   fZ 			= right.fZ; 
   fxIso 		= right.fxIso; 
   fyIso 		= right.fyIso;
   fzIso 		= right.fzIso;
   fTransIso 		= right.fTransIso; 
   fParentsCodes 	= right.fParentsCodes; 
   // isotopes' parents
   fParticleCode 	= right.fParticleCode;	
   fAdaug 		= right.fAdaug;	
   fZdaug 		= right.fAdaug;	
   fEPar 		= right.fEPar;

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
