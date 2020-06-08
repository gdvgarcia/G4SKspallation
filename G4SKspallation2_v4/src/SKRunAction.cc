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
/// \file SKRunAction.cc
/// \brief Implementation of the SKRunAction class

#include "SKRunAction.hh"
#include "SKAnalysis.hh"
#include "SKEventAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKRunAction::SKRunAction(SKEventAction* eventAction)
 : G4UserRunAction(),
   fEventAction(eventAction)
{ 
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);     

  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in SKAnalysis.hh
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;
  analysisManager->SetVerboseLevel(1);
  //analysisManager->SetNtupleMerging(true);
    // Note: merging ntuples is available only with Root output

  //
  // Book histograms, ntuple
  //
  
  // Creating histograms
  //
  // 1D histograms
    analysisManager->CreateH1("Edep","Energy of deposited per event", 101, 0.5 , 100.5 * GeV //,"none", "none", "log"
); // ID 0
    analysisManager->CreateH1("KinEmu","Initial muon kinetic energy", 100, 0.1 , 1000.1 * GeV,"none", "none", "log"
); // ID 1
    analysisManager->CreateH1("PathLmu","Initial muon path lenght", 42, 0. , 42. * m//,"none", "none", "log"
); // ID 2
  // 2D histogram for A and Z quantities
  analysisManager->CreateH2("Isotopes", "Ions produced; Z values; A values", 150, 0.5, 150.5, 300, 0.5, 300.5); // ID 0

  //
  // Creating the ntuple
  //
  analysisManager->CreateNtuple("event_data", "Muon initial and final position, muon initial energy, muon energy deposit, muon path length, muon first generation daugthers and their energies, isotopes vectors data and isotopes' parents vectors data, particles path length data and particles transverse distance");

  //  muon data
  analysisManager->CreateNtupleDColumn("Energy"); 	//column ID 0 - initial kinetic energy
  analysisManager->CreateNtupleDColumn("xIni"); 	//column ID 1 - muon initial position
  analysisManager->CreateNtupleDColumn("yIni"); 	//column ID 2 - muon initial position
  analysisManager->CreateNtupleDColumn("zIni"); 	//column ID 3 - muon initial position
  analysisManager->CreateNtupleDColumn("Edep"); 	//column ID 4 - total energy deposited per event
  analysisManager->CreateNtupleDColumn("xFin"); 	//column ID 5 - muon final position
  analysisManager->CreateNtupleDColumn("yFin"); 	//column ID 6 - muon final position
  analysisManager->CreateNtupleDColumn("zFin"); 	//column ID 7 - muon final position
  analysisManager->CreateNtupleDColumn("PathLength"); 	//column ID 8 - total muon path length 

  // first generation daugther particles data
  analysisManager->CreateNtupleIColumn("FirstGenSize"); 		//column ID 9  - size of the vectors 
  analysisManager->CreateNtupleIColumn(
        "FirstGenParticle",fEventAction->GetvectorFirstGenCode()); 	//column ID 10 - particle code
  analysisManager->CreateNtupleDColumn(
        "FirstGenEnergy", fEventAction->GetvectorEfisrtGen()); 		//column ID 11 - initial kinetic energy
 
  // isotopes data
  analysisManager->CreateNtupleIColumn( "IsotopesSize"); 	//column ID 12 - size of the vectors 
  analysisManager->CreateNtupleIColumn(
        "A", fEventAction->GetvectorA()); 			//column ID 13 - atomic mass
  analysisManager->CreateNtupleIColumn(
        "Z", fEventAction->GetvectorZ()); 			//column ID 14 - atomic number
  analysisManager->CreateNtupleDColumn(
        "x", fEventAction->Getvectorx()); 			//column ID 15 - final position
  analysisManager->CreateNtupleDColumn(
        "y", fEventAction->Getvectory()); 			//column ID 16 - final position
  analysisManager->CreateNtupleDColumn(
        "z", fEventAction->Getvectorz()); 			//column ID 17 - final position
  analysisManager->CreateNtupleDColumn(
        "TransverseDistance", fEventAction->GetvectorIsoDist());//column ID 18 - transverse distance
  analysisManager->CreateNtupleIColumn(
        "IsoParentCode",fEventAction->GetvectorIsoParentCode());//column ID 19 - parent particle code
  analysisManager->CreateNtupleDColumn(
        "InitialTime", fEventAction->GetvectorIniTime()); 	//column ID 20 - initial time when the isotope was created
  analysisManager->CreateNtupleDColumn(
        "FinalTime", fEventAction->GetvectorFinTime()); 		//column ID 21 - final time when it stopped being tracked
  analysisManager->CreateNtupleIColumn(
        "Stable1Instable0", fEventAction->GetvectorStable()); 	//column ID 22 - if stable it has 1 if instable is has 0

  // isotopes' parents data
  analysisManager->CreateNtupleIColumn("ParentsSize"); 		//column ID 23  - size of the vectors 
  analysisManager->CreateNtupleIColumn(
        "ParentsCode",fEventAction->GetvectorParentsCode()); 	//column ID 24 - particle code
  analysisManager->CreateNtupleIColumn(
        "Adaughter", fEventAction->GetvectorAdaug()); 		//column ID 25 - daugther atomic mass
  analysisManager->CreateNtupleIColumn(
        "Zdaughter", fEventAction->GetvectorZdaug()); 		//column ID 26 - daugther atomic number
  analysisManager->CreateNtupleDColumn(
        "ParentsEnergy", fEventAction->GetvectorEparent()); 	//column ID 27 - particle energy

  // all particles path length per energy data
  analysisManager->CreateNtupleIColumn("PathLengthSize"); 		//column ID 28 - size of the vectors 
  analysisManager->CreateNtupleIColumn(
        "PathLengthCode",fEventAction->GetvectorPathLengthCode());	//column ID 29 - particle code
  analysisManager->CreateNtupleDColumn(
        "PathLengthEnergy", fEventAction->GetvectorPathLengthE()); 	//column ID 30 - particle energy
  analysisManager->CreateNtupleDColumn(
        "StepLength", fEventAction->GetvectorStepLength()); 		//column ID 31 - particle step length for this given energy

  // all particles transverse distance
  analysisManager->CreateNtupleIColumn("TransParticlesSize"); 			//column ID 32 - size of the vectors 
  analysisManager->CreateNtupleIColumn(
        "TransParticlesCode",fEventAction->GetvectorTransParticlesCode());	//column ID 33 - particle code
  analysisManager->CreateNtupleDColumn(
        "TransParticlesDistance", fEventAction->GetvectorTransParticlesDist()); //column ID 34 - transverse distance

  // finishing ntuple creation
  analysisManager->FinishNtuple();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKRunAction::~SKRunAction()
{
  delete G4AnalysisManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKRunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file where we save the data collected
  //
  G4String fileName = "SK";
  analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKRunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // print some histogram statistics
  //
  auto analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->GetH1(1) ) {
    G4cout << G4endl << " ----> print histograms statistic ";
    if(isMaster) {
      G4cout << "for the entire run " << G4endl << G4endl; 
    }
    else {
      G4cout << "for the local thread " << G4endl << G4endl; 
    }
    
    G4cout << " Total energy deposit : mean = " 
       << G4BestUnit(analysisManager->GetH1(0)->mean(), "Energy") 
       << " rms = " 
       << G4BestUnit(analysisManager->GetH1(0)->rms(),  "Energy") << G4endl;
    
    G4cout << " Muon  kinetic energy spectrum : mean = " 
       << G4BestUnit(analysisManager->GetH1(1)->mean(), "Energy") 
       << " rms = " 
       << G4BestUnit(analysisManager->GetH1(1)->rms(),  "Energy") << G4endl;
  }

  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
