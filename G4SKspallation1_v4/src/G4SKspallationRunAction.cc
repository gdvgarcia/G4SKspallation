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
/// \file G4SKspallationRunAction.cc
/// \brief Implementation of the G4SKspallationRunAction class

#include "G4SKspallationRunAction.hh"
#include "G4SKspallationAnalysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4SKspallationRunAction::G4SKspallationRunAction()
 : G4UserRunAction()
{ 
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);     

  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in G4SKspallationAnalysis.hh
  auto analysisManager = G4AnalysisManager::Instance();
  //PRINTING TYPE OF ANALYSIS TECHNOLOGY
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Create directories 
  analysisManager->SetHistoDirectoryName("histograms");
  analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
    // Note: merging ntuples is available only with Root output

  // Book histograms, ntuple
  //
  
  // Creating histograms
  analysisManager->CreateH1("A","Particle atomic mass", 300, 0.5, 300.5  );
  analysisManager->CreateH1("Z","Particle proton number", 150, 0.5, 150.5);
  analysisManager->CreateH1("En","Energy of the neutron", 301, -0.5 * MeV, 300.5 * MeV);
  analysisManager->CreateH1("Ee","Energy of the electron", 301, -0.5 * MeV, 300.5 * MeV);
  // 2D histogram for A and Z quantities
  analysisManager->CreateH2("Particles", "Particles produced; Z values; A values", 150, 0.5, 150.5, 300, 0.5, 300.5);

  // Creating ntuple
  //
  analysisManager->CreateNtuple("G4SKspallation", "A, Z, position and type of parent process");
  analysisManager->CreateNtupleDColumn("A");
  analysisManager->CreateNtupleDColumn("Z");
  analysisManager->CreateNtupleDColumn("x");
  analysisManager->CreateNtupleDColumn("y");
  analysisManager->CreateNtupleDColumn("z");
  //analysisManager->CreateNtupleDColumn("Process");
  analysisManager->FinishNtuple();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4SKspallationRunAction::~G4SKspallationRunAction()
{
  delete G4AnalysisManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4SKspallationRunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
  G4String fileName = "G4SKspallation";
  analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4SKspallationRunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // print histogram statistics
  //
  auto analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->GetH1(0) ) {
    G4cout << G4endl << " ----> print histograms statistic ";
    if(isMaster) {
      G4cout << "for the entire run " << G4endl << G4endl; 
    }
    else {
      G4cout << "for the local thread " << G4endl << G4endl; 
    }
    
    G4cout << " A : mean = " 
       << analysisManager->GetH1(0)->mean() 
       << " rms = " 
       << analysisManager->GetH1(0)->rms() << G4endl;
    
    G4cout << " Z : mean = " 
       << analysisManager->GetH1(1)->mean()
       << " rms = " 
       << analysisManager->GetH1(1)->rms() << G4endl;

       G4cout << " E neutron : mean = " 
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
