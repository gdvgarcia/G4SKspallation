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
/// \file SKTankHit.hh
/// \brief Definition of the SKTankHit class

#ifndef SKTankHit_h
#define SKTankHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

/// Tank hit class
///
/// It defines data members to store the fEventID:
/// - fEventID

class SKTankHit : public G4VHit
{
  public:
    SKTankHit();
    SKTankHit(const SKTankHit&);
    virtual ~SKTankHit();

    // operators
    const SKTankHit& operator=(const SKTankHit&);
    G4bool operator==(const SKTankHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw() {}
    virtual void Print();

    // methods to handle data
    void Add();

    // get methods
    G4double GetEventID() const;
      
  private:
    G4int fEventID;        ///< Event ID
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using SKTankHitsCollection = G4THitsCollection<SKTankHit>;

extern G4ThreadLocal G4Allocator<SKTankHit>* SKTankHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* SKTankHit::operator new(size_t)
{
  if (!SKTankHitAllocator) {
    SKTankHitAllocator = new G4Allocator<SKTankHit>;
  }
  void *hit;
  hit = (void *) SKTankHitAllocator->MallocSingle();
  return hit;
}

inline void SKTankHit::operator delete(void *hit)
{
  if (!SKTankHitAllocator) {
    SKTankHitAllocator = new G4Allocator<SKTankHit>;
  }
  SKTankHitAllocator->FreeSingle((SKTankHit*) hit);
}

inline void SKTankHit::Add() {
  fEventID += 1;
}

inline G4double SKTankHit::GetEventID() const { 
  return fEventID; 
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
