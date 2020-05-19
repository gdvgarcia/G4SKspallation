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

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKRunAction::SKRunAction()
 : G4UserRunAction()
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
  // 1D histograms for energy spectra
  analysisManager->CreateH1("En_spec","Energy of neutrons", 1000, 0.1 , 500.1 , "MeV", "none", "log"
);
  analysisManager->CreateH1("Ee_spec","Energy of electrons + positrons", 1000, 0.1 , 500.1 ,"MeV", "none", "log"
);
  analysisManager->CreateH1("Epi_spec","Energy of charged pions", 1000, 0.1 , 500.1 ,"MeV", "none", "log"
);
  analysisManager->CreateH1("Epi0_spec","Energy of neutral pion", 1000, 0.1 , 500.1 ,"MeV", "none", "log"
);
    analysisManager->CreateH1("Edep","Energy of deposited per event", 101, 0.5 , 100.5 * GeV// ,"log"
);
  // 2D histogram for A and Z quantities
  analysisManager->CreateH2("Isotopes", "Ions produced; Z values; A values", 150, 0.5, 150.5, 300, 0.5, 300.5);

  // Creating the ntuple
  //
  analysisManager->CreateNtuple("isotopes_data", "A, Z, position, type of parent process and EventID");
  // For the isotopes ID=0
  analysisManager->CreateNtupleDColumn(0 , "A");
  analysisManager->CreateNtupleDColumn(0 , "Z");
  analysisManager->CreateNtupleDColumn(0 , "x");
  analysisManager->CreateNtupleDColumn(0 , "y");
  analysisManager->CreateNtupleDColumn(0 , "z");
  analysisManager->CreateNtupleSColumn(0 , "Process");
  analysisManager->CreateNtupleIColumn(0 , "ParentID");
  analysisManager->CreateNtupleIColumn(0 , "EventID");
  analysisManager->FinishNtuple(0);

  analysisManager->CreateNtuple("muon_data", "Particle name, Energy, position and EventID");
  // For the muon ID=1
  analysisManager->CreateNtupleSColumn(1 , "Particle");
  analysisManager->CreateNtupleDColumn(1 , "Energy");
  analysisManager->CreateNtupleDColumn(1 , "x");
  analysisManager->CreateNtupleDColumn(1 , "y");
  analysisManager->CreateNtupleDColumn(1 , "z");
  analysisManager->CreateNtupleIColumn(1 , "EventID");
  analysisManager->FinishNtuple(1);

  analysisManager->CreateNtuple("parents_data", "Particle name, Energy, position,daugher A and Z, trackID andEventID");
  // For the parent particles ID=2
  analysisManager->CreateNtupleSColumn(2 , "Particle");
  analysisManager->CreateNtupleDColumn(2 , "Energy");
  analysisManager->CreateNtupleDColumn(2 , "x");
  analysisManager->CreateNtupleDColumn(2 , "y");
  analysisManager->CreateNtupleDColumn(2 , "z");
  analysisManager->CreateNtupleDColumn(2 , "A");
  analysisManager->CreateNtupleDColumn(2 , "Z");
  analysisManager->CreateNtupleIColumn(2 , "TrackID");
  analysisManager->CreateNtupleIColumn(2 , "EventID");
  analysisManager->FinishNtuple(2);
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
    
    G4cout << " Neutron energy spectrum : mean = " 
       << G4BestUnit(analysisManager->GetH1(0)->mean(), "Energy") 
       << " rms = " 
       << G4BestUnit(analysisManager->GetH1(0)->rms(),  "Energy") << G4endl;
    
    G4cout << " Electron + positrons energy spectrum : mean = " 
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
