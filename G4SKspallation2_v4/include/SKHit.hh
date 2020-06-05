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
/// \file SKHit.hh
/// \brief Definition of the SKHit class

#ifndef SKHit_h
#define SKHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

/// SK hit class
///
/// It defines data members to store the the tootal energy deposit
/// of all particles in a selected volume and the initial muon kinetic energy:
/// - fEdep, fEmu

class SKHit : public G4VHit
{
  public:
    SKHit();
    SKHit(const SKHit&);
    virtual ~SKHit();

    // operators
    const SKHit& operator=(const SKHit&);
    G4bool operator==(const SKHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw() {}
    virtual void Print();

    //
    // methods to handle data
    //
    // for the muon
    void Add(G4double de);
    void FillIni(G4double Energy, G4double x, G4double y, G4double z);
    void AddL(G4double dl);
    void FillFin( G4double x, G4double y, G4double z);
    // for the isotopes
    void FillIso(G4int A, G4int Z, G4double x, G4double y, G4double z,G4double dis,  G4int code);
    // for isotopes' parents
    void FillIsoPar( G4int code, G4int A, G4int Z, G4double Energy);

    //
    // get methods
    //
    // for the muon
    G4double GetEdep() 	const;
    G4double GetEmu() 	const;
    G4double GetLmu() 	const;
    G4double GetxIni() 	const;
    G4double GetyIni() 	const;
    G4double GetzIni() 	const;
    G4double GetxFin() 	const;
    G4double GetyFin() 	const;
    G4double GetzFin() 	const;
    // for the isotopes
    std::vector< G4int > 	GetA() 			const; 
    std::vector< G4int >	GetZ() 			const;
    std::vector< G4double > 	GetxIso() 		const; 
    std::vector< G4double > 	GetyIso() 		const;
    std::vector< G4double > 	GetzIso() 		const; 
    std::vector< G4double > 	GetTransIso() 		const;
    std::vector< G4int > 	GetParentsCodes()	const;
    // for isotopes' parents
    std::vector< G4int >	GetParticleCode()	const;
    std::vector< G4int > 	GetAdaug() 		const;
    std::vector< G4int > 	GetZdaug() 		const;
    std::vector< G4double > 	GetEParf() 		const;

  private:
    // data for the muon
    G4double fEdep;     ///< Energy deposit in the sensitive volume
    G4double fEmu;      ///< Initial kinetic energy of the muon
    G4double fLmu;      ///< Path lenght of the muon
    G4double fxIni; 	///< x initial position  of the muon
    G4double fyIni;	///< y initial position  of the muon
    G4double fzIni;	///< z initial position  of the muon
    G4double fxFin;	///< x initial position  of the muon
    G4double fyFin;	///< y initial position  of the muon
    G4double fzFin;	///< z initial position  of the muon
    // data for the isotopes
    std::vector< G4int > 	fA; 		///< vector of atomic masses
    std::vector< G4int > 	fZ; 		///< vector of atomic numbers
    std::vector< G4double > 	fxIso; 		///< vector of x positions of the isotopes
    std::vector< G4double > 	fyIso;		///< vector of y positions of the isotopes
    std::vector< G4double > 	fzIso;		///< vector of z positions of the isotopes
    std::vector< G4double > 	fTransIso; 	///< vector of transverse distances of the isotopes
    std::vector< G4int > 	fParentsCodes; 	///< vector of the isotopes' parents codes
    // data for isotopes' parents
    std::vector< G4int > 	fParticleCode;	///< vector of the parent particle codes
    std::vector< G4int > 	fAdaug; 	///< vector of atomic masses of the daughters
    std::vector< G4int > 	fZdaug; 	///< vector of atomic numbers of the daughters
    std::vector< G4double > 	fEPar;		///< vector of energy of parents
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using SKHitsCollection = G4THitsCollection<SKHit>;

extern G4ThreadLocal G4Allocator<SKHit>* SKHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* SKHit::operator new(size_t)
{
  if (!SKHitAllocator) {
    SKHitAllocator = new G4Allocator<SKHit>;
  }
  void *hit;
  hit = (void *) SKHitAllocator->MallocSingle();
  return hit;
}

inline void SKHit::operator delete(void *hit)
{
  if (!SKHitAllocator) {
    SKHitAllocator = new G4Allocator<SKHit>;
  }
  SKHitAllocator->FreeSingle((SKHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Muon functions
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void SKHit::Add(G4double de) {
  fEdep += de;
}

inline G4double SKHit::GetEdep() const { 
  return fEdep; 
}

inline void SKHit::FillIni(G4double Energy, G4double x, G4double y, G4double z){
  fEmu 	= Energy;
  fxIni = x;
  fyIni = y;
  fzIni = z;
}

inline G4double SKHit::GetxIni() const { 
  return fxIni; 
}

inline G4double SKHit::GetyIni() const { 
  return fyIni; 
}

inline G4double SKHit::GetzIni() const { 
  return fzIni; 
}

inline void SKHit::FillFin( G4double x, G4double y, G4double z){
  fxFin = x;
  fyFin = y;
  fzFin = z;
}

inline G4double SKHit::GetxFin() const { 
  return fxFin; 
}

inline G4double SKHit::GetyFin() const { 
  return fyFin; 
}

inline G4double SKHit::GetzFin() const { 
  return fzFin; 
}

inline G4double SKHit::GetEmu() const { 
  return fEmu; 
}

inline void SKHit::AddL(G4double dl) {
  fLmu += dl;
}

inline G4double SKHit::GetLmu() const { 
  return fLmu; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Isotope functions
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void SKHit::FillIso	(G4int A, G4int Z, G4double x, G4double y, G4double z,G4double dis,  G4int code){
  fA.push_back 			(A);
  fZ.push_back 			(Z);
  fxIso.push_back 		(x);
  fyIso.push_back 		(y);
  fzIso.push_back 		(z);
  fTransIso.push_back 		(dis);
  fParentsCodes.push_back 	(code);
}

inline std::vector< G4int > SKHit::GetA() const { 
  return fA; 
}

inline std::vector< G4int > SKHit::GetZ() const { 
  return fZ; 
}

inline std::vector< G4double > SKHit::GetxIso() const { 
  return fxIso; 
}

inline std::vector< G4double > SKHit::GetyIso() const { 
  return fyIso; 
}

inline std::vector< G4double > SKHit::GetzIso() const { 
  return fzIso; 
}

inline std::vector< G4double > SKHit::GetTransIso() const { 
  return fTransIso; 
}

inline std::vector< G4int > SKHit::GetParentsCodes() const { 
  return fParentsCodes; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Parents functions
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void SKHit::FillIsoPar	( G4int code, G4int A, G4int Z, G4double Energy){
  fParticleCode.push_back 	(code);
  fAdaug.push_back 		(A);
  fZdaug.push_back 		(Z);
  fEPar.push_back 		(Energy);
}

inline std::vector< G4int > SKHit::GetParticleCode() const { 
  return fParticleCode; 
}

inline std::vector< G4int > SKHit::GetAdaug() const { 
  return fAdaug; 
}

inline std::vector< G4int > SKHit::GetZdaug() const { 
  return fZdaug; 
}

inline std::vector< G4double > SKHit::GetEParf() const { 
  return fEPar; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#endif
