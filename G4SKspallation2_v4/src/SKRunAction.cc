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

  // Create directories 
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  //analysisManager->SetNtupleMerging(true);
    // Note: merging ntuples is available only with Root output

  // Book histograms, ntuple
  //
  
  // Creating histograms
    analysisManager->CreateH1("Edep","Energy of deposited per event", 101, 0.5 , 100.5 * GeV //,"none", "none", "log"
); // ID 0
    analysisManager->CreateH1("KinEmu","Initial muon kinetic energy", 100, 0.1 , 1000.1 * GeV,"none", "none", "log"
); // ID 1
    analysisManager->CreateH1("PathLmu","Initial muon path lenght", 40, 100. , 500. * cm//,"none", "none", "log"
); // ID 2
  // 2D histogram for A and Z quantities
  analysisManager->CreateH2("Isotopes", "Ions produced; Z values; A values", 150, 0.5, 150.5, 300, 0.5, 300.5); // ID 0

  // Creating the ntuple
  //
  analysisManager->CreateNtuple("isotopes_data", "A, Z, position, parent particle, Kinetic Energy, Momentum direction and EventID");
  // For the isotopes ntupleID=0
  analysisManager->CreateNtupleSColumn(0 , "Name"); //column ID 0
  analysisManager->CreateNtupleIColumn(0 , "A"); //column ID 1
  analysisManager->CreateNtupleIColumn(0 , "Z"); //column ID 2
  analysisManager->CreateNtupleDColumn(0 , "x"); //column ID 3
  analysisManager->CreateNtupleDColumn(0 , "y"); //column ID 4
  analysisManager->CreateNtupleDColumn(0 , "z"); //column ID 5
  analysisManager->CreateNtupleDColumn(0 , "Transverse"); //column ID 6
  analysisManager->CreateNtupleSColumn(0 , "Process"); //column ID 7
  analysisManager->CreateNtupleSColumn(0 , "Parent"); //column ID 8
  analysisManager->CreateNtupleDColumn(0 , "LifeTime"); //column ID 9
  analysisManager->CreateNtupleDColumn(0 , "InitialTime"); //column ID 10
  analysisManager->CreateNtupleDColumn(0 , "FinalTime"); //column ID 11
  analysisManager->CreateNtupleSColumn(0 , "Daughters"); //column ID 12
  //analysisManager->CreateNtupleDColumn(0 , "ParentsEnergy"); //column ID 13
  //analysisManager->CreateNtupleDColumn(0 , "Energy"); //column ID 14 kinetic
  //analysisManager->CreateNtupleDColumn(0 , "Px"); //column ID 15
  //analysisManager->CreateNtupleDColumn(0 , "Py"); //column ID 16
  //analysisManager->CreateNtupleDColumn(0 , "Pz"); //column ID 17
  analysisManager->CreateNtupleIColumn(0 , "EventID"); //column ID 18 - 13
  analysisManager->FinishNtuple(0);

  analysisManager->CreateNtuple("muon_data", "Particle name, Kinetic Energy, position and EventID");
  // For the muon ntupleID=1
  analysisManager->CreateNtupleSColumn(1 , "Particle"); //column ID 0
  analysisManager->CreateNtupleDColumn(1 , "Energy"); //column ID 1 kinetic
  analysisManager->CreateNtupleDColumn(1 , "x"); //column ID 2
  analysisManager->CreateNtupleDColumn(1 , "y"); //column ID 3
  analysisManager->CreateNtupleDColumn(1 , "z"); //column ID 4
  analysisManager->CreateNtupleIColumn(1 , "EventID"); //column ID 5
  analysisManager->FinishNtuple(1);

  analysisManager->CreateNtuple("particles_data", "Particle name, Kinetic Energy, Step length and event ID");
  // For the particles ntupleID=2
  analysisManager->CreateNtupleSColumn(2 , "Particle"); //column ID 0
  analysisManager->CreateNtupleDColumn(2 , "Energy"); //column ID 1 kinetic
  analysisManager->CreateNtupleDColumn(2 , "PathLenght"); //column ID 2
  analysisManager->CreateNtupleIColumn(2 , "EventID"); //column ID 3
  analysisManager->FinishNtuple(2);

  analysisManager->CreateNtuple("particles2_data", "Particle name, Transverse distance and event ID");
  // For the particles ntupleID=3
  analysisManager->CreateNtupleSColumn(3 , "Particle"); //column ID 0
  analysisManager->CreateNtupleDColumn(3 , "TransverseDistance"); //column ID 1
  analysisManager->CreateNtupleIColumn(3 , "EventID"); //column ID 2
  analysisManager->FinishNtuple(3);

  analysisManager->CreateNtuple("parents_data", "name, daughter A, daughter  Z, process, Kinetic Energy and EventID");
  // For the isotopes' parents ntupleID=4
  analysisManager->CreateNtupleSColumn(4 , "Particle"); //column ID 0
  analysisManager->CreateNtupleIColumn(4 , "A"); //column ID 1
  analysisManager->CreateNtupleIColumn(4 , "Z"); //column ID 2
  analysisManager->CreateNtupleSColumn(4 , "Process"); //column ID 3
  //analysisManager->CreateNtupleSColumn(4 , "Parent"); //column ID 4
  analysisManager->CreateNtupleDColumn(4 , "Energy"); //column ID 4 kinetic
  analysisManager->CreateNtupleIColumn(4 , "EventID"); //column ID 5
  analysisManager->FinishNtuple(4);

  analysisManager->CreateNtuple("muondaug_data", "Particle name, Kinetic Energy and event ID");
  // For the particles ntupleID=5
  analysisManager->CreateNtupleSColumn(5 , "Particle"); //column ID 0
  analysisManager->CreateNtupleDColumn(5 , "Energy"); //column ID 1 kinetic
  analysisManager->CreateNtupleSColumn(5 , "Process"); //column ID 2
  analysisManager->CreateNtupleIColumn(5 , "EventID"); //column ID 3
  analysisManager->FinishNtuple(5);

  analysisManager->CreateNtuple("event_data", "Muon initial and final position, muon initial energy, muon energy deposit, muon path length, isotopes vectors data and isotopes' parents vectors data");
  // For the particles ntupleID=6
  analysisManager->CreateNtupleDColumn(6 , "Energy"); 		//column ID 0 kinetic
  analysisManager->CreateNtupleDColumn(6 , "xIni"); 		//column ID 1
  analysisManager->CreateNtupleDColumn(6 , "yIni"); 		//column ID 2
  analysisManager->CreateNtupleDColumn(6 , "zIni"); 		//column ID 3
  analysisManager->CreateNtupleDColumn(6 , "Edep"); 		//column ID 4
  analysisManager->CreateNtupleDColumn(6 , "xFin"); 		//column ID 5
  analysisManager->CreateNtupleDColumn(6 , "yFin"); 		//column ID 6
  analysisManager->CreateNtupleDColumn(6 , "zFin"); 		//column ID 7
  analysisManager->CreateNtupleDColumn(6 , "PathLength"); 	//column ID 8
  analysisManager->CreateNtupleIColumn(6 , "IsotopesSize"); 	//column ID 9
  analysisManager->CreateNtupleIColumn(6 , "ParentsSize"); 	//column ID 10
  // vectors declaration
  analysisManager->CreateNtupleIColumn(6 ,
        "A", fEventAction->GetvectorA()); //column ID 11
  analysisManager->CreateNtupleIColumn(6 ,
        "Z", fEventAction->GetvectorZ()); //column ID 12
  analysisManager->CreateNtupleDColumn(6 ,
        "x", fEventAction->Getvectorx()); //column ID 13
  analysisManager->CreateNtupleDColumn(6 ,
        "y", fEventAction->Getvectory()); //column ID 14
  analysisManager->CreateNtupleDColumn(6 ,
        "z", fEventAction->Getvectorz()); //column ID 15
  analysisManager->CreateNtupleDColumn(6 ,
        "TransverseDistance", fEventAction->GetvectorDist()); //column ID 16
  analysisManager->CreateNtupleIColumn(6 ,
        "ParentCode",fEventAction->GetvectorParentCode()); //column ID 17
  analysisManager->CreateNtupleIColumn(6 ,
        "ParticleCode",fEventAction->GetvectorParticleCode()); //column ID 18
  analysisManager->CreateNtupleIColumn(6 ,
        "Adaughter", fEventAction->GetvectorAdaug()); //column ID 19
  analysisManager->CreateNtupleIColumn(6 ,
        "Zdaughter", fEventAction->GetvectorZdaug()); //column ID 20
  analysisManager->CreateNtupleDColumn(6 ,
        "ParticleEnergy", fEventAction->GetvectorEparent()); //column ID 21
  analysisManager->FinishNtuple(6);
  
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
