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
/// \file SKRunAction.hh
/// \brief Definition of the SKRunAction class

#ifndef SKRunAction_h
#define SKRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "SKEventAction.hh"

class G4Run;
class G4GenericMessenger;

/// Run action class
///
/// It accumulates statistic for the full run with use of analysis tools.
/// H1D and H2D histograms and a ntuple are created in BeginOfRunAction()
/// The histograms and ntuple are then saved in the output file in a format
/// according to a selected technology in SKAnalysis.hh.
///
/// In EndOfRunAction(), the accumulated statistic is printed and saved in the SK file created.
///

class SKRunAction : public G4UserRunAction
{
  public:
    SKRunAction( SKEventAction* eventAction );
    virtual ~SKRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);


    private:
    void DefineCommands(); 			// method where we define our command that sets the seed for the name of the root file

    G4int  			fSeed; 		// pointer a to G4 gun class
    G4GenericMessenger* 	fMessenger;	// messenger which sets the file name
    SKEventAction* 		fEventAction;   // so we get the pointer to the vectors of each event
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

