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
/// \file G4SKspallationTankHit.hh
/// \brief Definition of the G4SKspallationTankHit class

#ifndef G4SKspallationTankHit_h
#define G4SKspallationTankHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

/// Tank hit class
///
/// It defines data members to store the the N number
/// of produced particles(nucleons) in a selected volume, the tank:
/// - fN

class G4SKspallationTankHit : public G4VHit
{
  public:
    G4SKspallationTankHit();
    G4SKspallationTankHit(const G4SKspallationTankHit&);
    virtual ~G4SKspallationTankHit();

    // operators
    const G4SKspallationTankHit& operator=(const G4SKspallationTankHit&);
    G4bool operator==(const G4SKspallationTankHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw() {}
    virtual void Print();

    // methods to handle data
    void Add(G4double N);

    // get methods
    G4double GetN() const;
      
  private:
    G4double fN; ///< Number of nucleons in the sensitive volume, tank
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using G4SKspallationTankHitsCollection = G4THitsCollection<G4SKspallationTankHit>;

extern G4ThreadLocal G4Allocator<G4SKspallationTankHit>* G4SKspallationTankHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* G4SKspallationTankHit::operator new(size_t)
{
  if (!G4SKspallationTankHitAllocator) {
    G4SKspallationTankHitAllocator = new G4Allocator<G4SKspallationTankHit>;
  }
  void *hit;
  hit = (void *) G4SKspallationTankHitAllocator->MallocSingle();
  return hit;
}

inline void G4SKspallationTankHit::operator delete(void *hit)
{
  if (!G4SKspallationTankHitAllocator) {
    G4SKspallationTankHitAllocator = new G4Allocator<G4SKspallationTankHit>;
  }
  G4SKspallationTankHitAllocator->FreeSingle((G4SKspallationTankHit*) hit);
}

inline void G4SKspallationTankHit::Add(G4double N) {
  fN += N; ///< Number of nucleons in the sensitive volume, tank

}

inline G4double G4SKspallationTankHit::GetN() const { 
  return fN; 
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
