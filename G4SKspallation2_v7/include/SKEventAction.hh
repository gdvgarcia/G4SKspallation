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
#include "globals.hh"

/// Event action class
///
/// It defines data members to hold the data per event
/// of particles inside the tank/fiducial volume
///
/// In EndOfEventAction(), it prints the accumulated quantities of the energy 
/// deposit of all particles in the SK tank and the initial muon path length
/// and energy. It also add new rolls to the ntuple and some data to the 
/// histograms.

class SKEventAction : public G4UserEventAction
{
public:
  SKEventAction();
  virtual ~SKEventAction();

  virtual void  	BeginOfEventAction(const G4Event* event);
  virtual void    	EndOfEventAction(const G4Event* event);

  // get methods
  //
  // vectors for first generation daugthers of the muon data
  std::vector<G4int>& 		GetvectorFirstGenCode()			{ return fvectorFirstGenCode;		}
  std::vector<G4double>& 	GetvectorEfisrtGen()			{ return fvectorEfisrtGen; 		}
  // vectors for the isotopes data
  std::vector<G4int>& 		GetvectorA() 				{ return fvectorA; 			}
  std::vector<G4int>& 		GetvectorZ() 				{ return fvectorZ; 			}
  std::vector<G4double>& 	Getvectorx() 				{ return fvectorx; 			}
  std::vector<G4double>& 	Getvectory() 				{ return fvectory; 			}
  std::vector<G4double>& 	Getvectorz() 				{ return fvectorz; 			}
  std::vector<G4double>& 	GetvectorIsoDist() 			{ return fvectorIsoDist; 		}
  std::vector<G4int>& 		GetvectorIsoParentCode() 		{ return fvectorIsoParentCode; 		}
  std::vector<G4double>& 	GetvectorIniTime()			{ return fvectorIniTime; 		}
  std::vector<G4double>& 	GetvectorFinTime()			{ return fvectorFinTime; 		}
  std::vector<G4int>& 		GetvectorStable()			{ return fvectorStable; 		}
  std::vector<G4double>& 	GetvectorLifeTime()			{ return fvectorLifeTime; 		}
  // vectors for the isotopes' parents data
  std::vector<G4int>& 		GetvectorParentsCode() 			{ return fvectorParentsCode; 		}
  std::vector<G4int>& 		GetvectorAdaug() 			{ return fvectorAdaug; 			}
  std::vector<G4int>& 		GetvectorZdaug() 			{ return fvectorZdaug; 			}
  std::vector<G4double>& 	GetvectorEparent() 			{ return fvectorEparent; 		}
  // vectors for the path length data
  std::vector<G4int>& 		GetvectorPathLengthCode()		{ return fvectorPathLengthCode; 	}
  std::vector<G4double>& 	GetvectorPathLengthE()			{ return fvectorPathLengthE; 		}
  std::vector<G4double>& 	GetvectorStepLength()			{ return fvectorStepLength; 		}
  // vectors for the transverse distance data
  std::vector<G4int>& 		GetvectorTransParticlesCode() 		{ return fvectorTransParticlesCode; 	}
  std::vector<G4double>& 	GetvectorTransParticlesDist()		{ return fvectorTransParticlesDist; 	}
  // only necessary to the stepAction
  G4int				GetParentCode	( G4int TrackID ) 	{ return fParentsCodes[TrackID]; 	}
  G4double  			GetInitialTime	( G4int TrackID )	{ return fInitialTimes[TrackID]; 	}
  G4double 			GetxIni()				{ return fxIni; 			}
  G4double 			GetyIni()				{ return fyIni; 			}
  G4double 			GetzIni()				{ return fzIni; 			}
  G4double 			GetxFin()				{ return fxFin; 			}
  G4double 			GetyFin()				{ return fyIni; 			}
  G4double 			GetzFin()				{ return fzIni; 			}  

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
  void FillIso(G4int A, G4int Z, G4double x, G4double y, G4double z,G4double dis,  G4int Code, 
  G4double InitialTime, G4double FinalTime, G4int Stable, G4double LifeTime);
  // for isotopes' parents
  void FillIsoPar( G4int Code, G4int A, G4int Z, G4double Energy);
  // for the path length data
  void FillPathLength( G4int Code, G4double Energy, G4double StepLength);
  // for the transverse distance data
  void FillTransverse( G4int Code, G4double TransverseDistance);
  // only necessary to the stepAction
  void FillParentsCodes( G4int TrackID, G4int 		ParentCode );
  void FillInitialTimes( G4int TrackID, G4double 	InitialTime );

private:
  // methods
  void PrintEventStatistics() const;
  
  // data members    
  //
  // data for the muon
  G4double fEdep;     						///< Energy deposit in the sensitive volume
  G4double fEmu;      						///< Initial kinetic energy of the muon
  G4double fLmu;      						///< Path lenght of the muon
  G4double fxIni; 						///< x initial position  of the muon
  G4double fyIni;						///< y initial position  of the muon
  G4double fzIni;						///< z initial position  of the muon
  G4double fxFin;						///< x initial position  of the muon
  G4double fyFin;						///< y initial position  of the muon
  G4double fzFin;						///< z initial position  of the muon         
  // vectors for first generation daugthers of the muon data
  std::vector< G4int > 		fvectorFirstGenCode;		///< vector of the first generation particles codes
  std::vector< G4double >	fvectorEfisrtGen;		///< vector of energies of first generation particles
  // vectors for the isotopes data
  std::vector< G4int > 		fvectorA;			///< vector of atomic masses
  std::vector< G4int > 		fvectorZ;			///< vector of atomic numbers 
  std::vector< G4double > 	fvectorx;			///< vector of x positions of the isotopes
  std::vector< G4double > 	fvectory;			///< vector of y positions of the isotopes
  std::vector< G4double > 	fvectorz;			///< vector of z positions of the isotopes
  std::vector< G4double > 	fvectorIsoDist;			///< vector of transverse distances of the isotopes
  std::vector< G4int > 		fvectorIsoParentCode;		///< vector of the isotopes' parents codes
  std::vector< G4double > 	fvectorIniTime;			///< vector of initial times of the isotopes
  std::vector< G4double > 	fvectorFinTime;			///< vector of final times of the isotopes
  std::vector< G4int > 		fvectorStable;			///< vector of the stability(1) or instability(0) of isotopes
  std::vector< G4double > 	fvectorLifeTime;		///< vector of life times of the isotopes
  // vectors for the isotopes' parents data
  std::vector< G4int > 		fvectorParentsCode;		///< vector of the parent particle codes
  std::vector< G4int > 		fvectorAdaug;			///< vector of atomic masses of the daughters
  std::vector< G4int > 		fvectorZdaug;			///< vector of atomic numbers of the daughters
  std::vector< G4double > 	fvectorEparent;			///< vector of energy of parents
  // vectors for the path length data
  std::vector< G4int > 		fvectorPathLengthCode;		///< vector of the particle codes
  std::vector< G4double > 	fvectorPathLengthE;		///< vector of energies of particles
  std::vector< G4double > 	fvectorStepLength;		///< vector of steps lengths of particles for a given energy
  // vectors for the transverse distance data
  std::vector< G4int > 		fvectorTransParticlesCode;	///< vector of the particle codes
  std::vector< G4double >	fvectorTransParticlesDist;	///< vector of the transverse distance

  // data members necessary to collect the information in the StepAction
  std::map< int, int > 		fParentsCodes; 			// keep parents' trackIDs so we can recover parents' codes
  std::map< int, double> 	fInitialTimes; 			// keep isotopes' initial time
};
                     
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Muon functions
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void SKEventAction::Add(G4double de) {
  fEdep += de;
}

inline void SKEventAction::FillIni(G4double Energy, G4double x, G4double y, G4double z){
  fEmu 	= Energy;
  fxIni = x;
  fyIni = y;
  fzIni = z;
}

inline void SKEventAction::FillFin(G4double x, G4double y, G4double z){
  fxFin = x;
  fyFin = y;
  fzFin = z;
}

inline void SKEventAction::AddL(G4double dl) {
  fLmu += dl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// First generation function
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void SKEventAction::FillFirstGen(G4double Energy, G4int Code){
  fvectorFirstGenCode.push_back 	(Code);
  fvectorEfisrtGen.push_back 		(Energy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Isotope function
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
inline void SKEventAction::FillIso(G4int A, G4int Z, G4double x, G4double y, G4double z,G4double dis,  G4int Code, G4double InitialTime, G4double FinalTime, G4int Stable, G4double LifeTime){
  fvectorA.push_back 			(A);
  fvectorZ.push_back 			(Z);
  fvectorx.push_back 			(x);
  fvectory.push_back 			(y);
  fvectorz.push_back 			(z);
  fvectorIsoDist.push_back 		(dis);
  fvectorIsoParentCode.push_back 	(Code);
  fvectorIniTime.push_back 		(InitialTime);
  fvectorFinTime.push_back 		(FinalTime);
  fvectorStable.push_back 		(Stable);
  fvectorLifeTime.push_back 		(LifeTime);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Parents function
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void SKEventAction::FillIsoPar( G4int Code, G4int A, G4int Z, G4double Energy){
  fvectorParentsCode.push_back	 	(Code);
  fvectorAdaug.push_back 		(A);
  fvectorZdaug.push_back 		(Z);
  fvectorEparent.push_back 		(Energy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Path Length function
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void SKEventAction::FillPathLength( G4int Code, G4double Energy, G4double StepLength){
  fvectorPathLengthCode.push_back 	(Code);
  fvectorPathLengthE.push_back 		(Energy);
  fvectorStepLength.push_back 		(StepLength);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Transverse distance function
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void SKEventAction::FillTransverse( G4int Code, G4double TransverseDistance){
  fvectorTransParticlesCode.push_back 	(Code);
  fvectorTransParticlesDist.push_back 	(TransverseDistance);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#endif
