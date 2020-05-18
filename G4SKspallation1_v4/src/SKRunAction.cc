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
///  It creates the histograms and Ntuples for the event

#include "SKRunAction.hh"

// #include "SKRun.hh"

#include "SKAnalysis.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKRunAction::SKRunAction()
: G4UserRunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SKRunAction::~SKRunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKRunAction::BeginOfRunAction(const G4Run*)
{ 
  // Generating the Ntuples and histrograms for data collection
  //

  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in SKAnalysis.hh
  auto analysisManager = G4AnalysisManager::Instance();
  // PRINTING TYPE OF ANALYSIS TECHNOLOGY
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  analysisManager->SetVerboseLevel(1);
  //analysisManager->SetNtupleMerging(true);
    // Note: merging ntuples is available only with Root output

  // Creating histograms
  // 1D histograms for energy spectra
  analysisManager->CreateH1("En_spec","Energy of neutrons", 301, -0.5 * MeV, 300.5 * MeV// ,"log"
);
  analysisManager->CreateH1("Ee_spec","Energy of electrons", 301, -0.5 * MeV, 300.5 * MeV// ,"log"
);
  // 2D histogram for A and Z quantities
  analysisManager->CreateH2("Isotopes", "Ions produced; Z values; A values", 150, 0.5, 150.5, 300, 0.5, 300.5);

  // Creating ntuple
  //
  analysisManager->CreateNtuple("isotopes_data", "A, Z, position and type of parent process");
  analysisManager->CreateNtupleDColumn("A");
  analysisManager->CreateNtupleDColumn("Z");
  analysisManager->CreateNtupleDColumn("x");
  analysisManager->CreateNtupleDColumn("y");
  analysisManager->CreateNtupleDColumn("z");
  analysisManager->CreateNtupleSColumn("Process");
  analysisManager->CreateNtupleIColumn("EventID");
  analysisManager->FinishNtuple();

  // Open an output file
  //
  G4String fileName = "G4SKspallation";
  analysisManager->OpenFile(fileName);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SKRunAction::EndOfRunAction(const G4Run* run)
{

  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // save histograms & ntuple
  //
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

