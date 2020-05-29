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

    // methods to handle data
    void Add(G4double de);
    void Fill(G4double emu);
    void AddL(G4double dl);

    // get methods
    G4double GetEdep() const;
    G4double GetEmu() const;
    G4double GetLmu() const;
      
  private:
    G4double fEdep;        ///< Energy deposit in the sensitive volume
    G4double fEmu;         ///< Initial kinetic energy of the muon
    G4double fLmu;         ///< Path lenght of the muon
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

inline void SKHit::Add(G4double de) {
  fEdep += de;
}

inline G4double SKHit::GetEdep() const { 
  return fEdep; 
}

inline void SKHit::Fill(G4double emu) {
  fEmu = emu;
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

#endif
