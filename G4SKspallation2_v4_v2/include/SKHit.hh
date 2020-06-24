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

#include <vector>
#include <string> 

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
    // for the first generation daugther of the muon
    void FillFirstGen(G4double Energy, G4int Code);
    // for the isotopes
    void FillIso(G4int A, G4int Z, G4double x, G4double y, G4double z,G4double dis,  G4int Code, G4double InitialTime, G4double FinalTime, G4int Stable);
    // for isotopes' parents
    void FillIsoPar( G4int Code, G4int A, G4int Z, G4double Energy);
    // for the path length data
    void FillPathLength( G4int Code, G4double Energy, G4double StepLength);
    // for the transverse distance data
    void FillTransverse( G4int Code, G4double TransverseDistance);

    //
    // get methods
    //
    // for the muon
    G4double GetEdep() 	const 	{ return fEdep; }
    G4double GetEmu() 	const 	{ return fEmu; 	}
    G4double GetLmu() 	const 	{ return fLmu; 	}
    G4double GetxIni() 	const	{ return fxIni; }
    G4double GetyIni() 	const	{ return fyIni; }
    G4double GetzIni() 	const	{ return fzIni; }
    G4double GetxFin() 	const	{ return fxFin; }
    G4double GetyFin() 	const	{ return fyFin; }
    G4double GetzFin() 	const	{ return fzFin; }
    // for the first generation daugther of the muon
    std::vector< G4int > 	GetFirstGenCode()	const 	{ return fFirstGenCode; 	}
    std::vector< G4double > 	GetEfisrtGen()		const 	{ return fEfisrtGen; 		}
    // for the isotopes	
    std::vector< G4int > 	GetA() 			const 	{ return fA; 			} 
    std::vector< G4int >	GetZ() 			const 	{ return fZ; 			}
    std::vector< G4double > 	GetxIso() 		const 	{ return fxIso; 		} 
    std::vector< G4double > 	GetyIso() 		const 	{ return fyIso; 		}
    std::vector< G4double > 	GetzIso() 		const 	{ return fzIso; 		} 
    std::vector< G4double > 	GetIsoDist() 		const 	{ return fIsoDist; 		}
    std::vector< G4int > 	GetIsoParentCode()	const 	{ return fIsoParentCode; 	}
    std::vector< G4double > 	GetIniTime() 		const 	{ return fIniTime; 		} 
    std::vector< G4double > 	GetFinTime() 		const 	{ return fFinTime; 		}
    std::vector< G4int > 	GetStable()		const 	{ return fStable; 		}
    // for isotopes' parents
    std::vector< G4int >	GetParentsCode()	const 	{ return fParentsCode; 		}
    std::vector< G4int > 	GetAdaug() 		const 	{ return fAdaug; 		}
    std::vector< G4int > 	GetZdaug() 		const 	{ return fZdaug; 		}
    std::vector< G4double > 	GetEparent() 		const 	{ return fEparent; 		}
    // for the path length data
    std::vector< G4int >	GetPathLengthCode()	const 	{ return fPathLengthCode; 	} 
    std::vector< G4double > 	GetPathLengthE() 	const 	{ return fPathLengthE; 		} 
    std::vector< G4double > 	GetStepLength() 	const 	{ return fStepLength; 		}
    // for the transverse distance data
    std::vector< G4int >	GetTransParticlesCode()	const 	{ return fTransCode; 	}
    std::vector< G4double >  	GetTransParticlesDist()	const 	{ return fTransDist; 	} 	

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
    // data for the first generation daugther of the muon
    std::vector< G4int > 	fFirstGenCode;	///< vector of the first generation particles codes
    std::vector< G4double > 	fEfisrtGen;	///< vector of energies of first generation particles
    // data for the isotopes
    std::vector< G4int > 	fA; 		///< vector of atomic masses
    std::vector< G4int > 	fZ; 		///< vector of atomic numbers
    std::vector< G4double > 	fxIso; 		///< vector of x positions of the isotopes
    std::vector< G4double > 	fyIso;		///< vector of y positions of the isotopes
    std::vector< G4double > 	fzIso;		///< vector of z positions of the isotopes
    std::vector< G4double > 	fIsoDist; 	///< vector of transverse distances of the isotopes
    std::vector< G4int > 	fIsoParentCode; ///< vector of the isotopes' parents codes
    std::vector< G4double > 	fIniTime;	///< vector of initial times of the isotopes
    std::vector< G4double > 	fFinTime; 	///< vector of final times of the isotopes
    std::vector< G4int > 	fStable; 	///< vector of the stability(1) or instability(0) of isotopes
    // data for isotopes' parents
    std::vector< G4int > 	fParentsCode;	///< vector of the parent particle codes
    std::vector< G4int > 	fAdaug; 	///< vector of atomic masses of the daughters
    std::vector< G4int > 	fZdaug; 	///< vector of atomic numbers of the daughters
    std::vector< G4double > 	fEparent;	///< vector of energy of parents
    // data for the path length
    std::vector< G4int > 	fPathLengthCode;///< vector of the particle codes 
    std::vector< G4double > 	fPathLengthE;	///< vector of energies of particles
    std::vector< G4double > 	fStepLength;	//< vector of steps lengths of particles for a given energy
    // data for the transverse distance
    std::vector< G4int > 	fTransCode;	///< vector of the particle codes 
    std::vector< G4double > 	fTransDist; 	///< vector of the transverse distance
    
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

inline void SKHit::FillIni(G4double Energy, G4double x, G4double y, G4double z){
  fEmu 	= Energy;
  fxIni = x;
  fyIni = y;
  fzIni = z;
}

inline void SKHit::FillFin(G4double x, G4double y, G4double z){
  fxFin = x;
  fyFin = y;
  fzFin = z;
}

inline void SKHit::AddL(G4double dl) {
  fLmu += dl;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// First generation function
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void SKHit::FillFirstGen(G4double Energy, G4int Code){
  fFirstGenCode.push_back 	(Code);
  fEfisrtGen.push_back 		(Energy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Isotope function
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void SKHit::FillIso(G4int A, G4int Z, G4double x, G4double y, G4double z,G4double dis,  G4int Code, G4double InitialTime, G4double FinalTime, G4int Stable){
  fA.push_back 			(A);
  fZ.push_back 			(Z);
  fxIso.push_back 		(x);
  fyIso.push_back 		(y);
  fzIso.push_back 		(z);
  fIsoDist.push_back 		(dis);
  fIsoParentCode.push_back 	(Code);
  fIniTime.push_back 		(InitialTime);
  fFinTime.push_back 		(FinalTime);
  fStable.push_back 		(Stable);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Parents function
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void SKHit::FillIsoPar( G4int Code, G4int A, G4int Z, G4double Energy){
  fParentsCode.push_back 	(Code);
  fAdaug.push_back 		(A);
  fZdaug.push_back 		(Z);
  fEparent.push_back 		(Energy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Path Length function
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void SKHit::FillPathLength( G4int Code, G4double Energy, G4double StepLength){
  fPathLengthCode.push_back 	(Code);
  fPathLengthE.push_back 	(Energy);
  fStepLength.push_back 	(StepLength);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Transverse distance function
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void SKHit::FillTransverse( G4int Code, G4double TransverseDistance){
  fTransCode.push_back 	(Code);
  fTransDist.push_back 	(TransverseDistance);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
