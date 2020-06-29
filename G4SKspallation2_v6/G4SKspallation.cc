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
/// \file G4SKspallation.cc
/// \brief Main program of the G4SKspallation
/// You can choose the physics list by using a third argument when opening the application
///  and the initial conditon file with a fourth argument

#include "SKDetectorConstruction.hh"
#include "SKActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#include "G4Threading.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4ScoringManager.hh"
#include "QGSP_BERT_HP.hh"  	//physics list that will be used by default 
#include "QGSP_BERT.hh"  	// ID 0
#include "QGSP_BIC_HP.hh"   	// ID 1
#include "FTFP_BERT_HP.hh"  	// ID 2
#include "QGSP_INCLXX_HP.hh"  	// ID 3
#include "G4PhysListFactory.hh"
#include "G4VUserPhysicsList.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

#include <iostream>
#include <string>

#include <chrono>  // for high_resolution_clock

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  
  // Optionally: choose a different Random engine...
  //G4Random::setTheEngine(new CLHEP::RanecuEngine);
  //G4Random::setTheEngine(new CLHEP::MTwistEngine);
  //G4Random::setTheEngine(new CLHEP::DRand48Engine);
  //G4Random::setTheEngine(new CLHEP::RandEngine);
  G4Random::setTheEngine( new CLHEP::HepJamesRandom ); 
		
  // Record starting time
  auto start = std::chrono::high_resolution_clock::now(); // getting the time now
  // converting the time into an int taking the milisecs into account 
  double t_1 = std::chrono::duration_cast<std::chrono::duration<double>>(start.time_since_epoch()).count();
  t_1 = t_1 * 1000. - 1592000000000.; // multiplied by 1 000  to get milisecs precision and removing the large unnecessary number of 1592000000000 secs - so this simulation has a maximum of around 30 years validation - Now 2020 
  
  long seed = (long)t_1; // so we use the time for the seed
  // Setting your seed
  G4Random::setTheSeed(seed);
  std::cout << " \n Initial random seed is : " << seed << " \n";

	
  // Construct the default run manager
  //
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
  // you can set your own number of threads changing the numberOfcores - G4Threading::GetNumberOfCores() returns the number of threads 
  // available on the machine(This number includes the number of hyper threads)
  G4int numberOfcores =  G4Threading::G4GetNumberOfCores();
  runManager->SetNumberOfThreads(numberOfcores);  
  std::cout << "Multithreading mode : ON \n \n";
#else
  G4RunManager* runManager = new G4RunManager;
  std::cout << "Multithreading mode : OFF \n \n";
#endif

	
  // Activate UI-command base scorer
  //G4ScoringManager * scManager = G4ScoringManager::GetScoringManager();
  //scManager->SetVerboseLevel(0);

	
  // Set mandatory initialization classes
  //
  // Detector construction
  runManager->SetUserInitialization(new SKDetectorConstruction());

  // Physics list
  G4PhysListFactory  *physListFactory = new G4PhysListFactory();
  G4VUserPhysicsList* physicsList = 0; // initialize
  // Detect if we are choosing a physics list (if there is a third argument)
  //
  if (  argc > 2 ) { 
	G4String physL = argv[2];
  	if (  physL.compare("!") != 0 ){// check if we have a ! , because it is just a place holder
  		if( physL.size() == 1 ){ // check if we really have a size one argument 
  			G4int ID = atof(physL.c_str()); // getting the ID of the chosen phys List
			// taking the phys list by its ID
  			switch(ID) {
      			case 0:
         			physicsList =  physListFactory->GetReferencePhysList("QGSP_BERT");
      	        		break;
      			case 1:
          			physicsList =  physListFactory->GetReferencePhysList("QGSP_BIC_HP");
          			break;
      			case 2:
          			physicsList =  physListFactory->GetReferencePhysList("FTFP_BERT_HP");
          			break;
      			case 3:
          			physicsList =  physListFactory->GetReferencePhysList("QGSP_INCLXX_HP"); // it says that this phys List is still under validation and improvements
          			break;
      			default:
          			physicsList =  physListFactory->GetReferencePhysList("QGSP_BERT_HP"); //default
          			break;
			}
    		}else{physicsList =  physListFactory->GetReferencePhysList("QGSP_BERT_HP");} //default one
	}else{physicsList =  physListFactory->GetReferencePhysList("QGSP_BERT_HP");} //default one
  }else{physicsList =  physListFactory->GetReferencePhysList("QGSP_BERT_HP");} //default one
  physicsList->SetVerboseLevel(0);
  runManager->SetUserInitialization(physicsList);
    
  // User action initialization
  //
  runManager->SetUserInitialization(new SKActionInitialization());

	
  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive("Quiet");
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  visManager->Initialize();

	
  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();


  // Process macro or start UI session
  //
  if ( ! ui ) { // batch mode
    	// // Detect if we want to read a initial condition file (if there is a forth argument)
    	if (  argc > 3 ) { 
#ifdef G4MULTITHREADED
  		// first if in multithread we have to initialize the run so the primary generators for each thread are created and so we can asign the file name and threa information for those classes
  		G4String cmdinitial = "/run/initialize "; 	// command to initialize the run
  		UImanager->ApplyCommand(cmdinitial);
  		std::cout << "Number of threads being used : " << numberOfcores << "\n";
#endif
						
		G4String command1 	= "/SK/generator/SetFileName "; // command to set the initial condition file name with the fourth argument
    		G4String fileName1 	= argv[3];			// initial conditions file name - if "yes" it uses default file
		if( fileName1 == "yes" ){ 				// using default file
    			UImanager->ApplyCommand(command1);
		}
		else{							// using given file
    			UImanager->ApplyCommand(command1+fileName1);
		}

#ifdef G4MULTITHREADED
  	// If we are in multithreading mode we give the number of threads to the particle generator so it reads correctly the ini cond file
  						
  	G4String command2 	= "/SK/generator/SetThreads "; 		// command to set the number of threads
  	G4String nCores 	= std::to_string(numberOfcores); 	// number of threads
  	UImanager->ApplyCommand(command2 + nCores);
#endif	
    	}

    	// running the macro
    	G4String command3 	= "/control/execute ";		// command to run a macro
    	G4String fileName2 	= argv[1];			// macro file name 
	UImanager->ApplyCommand(command3+fileName2);

    	// Record end time
    	auto finish = std::chrono::high_resolution_clock::now();

    	// Printing Elapsed time
    	std::chrono::duration<double> elapsed = finish - start;
    	std::cout << "\n Elapsed time: " << elapsed.count() << " s\n";
  }
  else { // interactive mode 
    	// activating muon energy and angular distribution for the interactive mode
    	G4String command1 	= "/SK/generator/SetFileName "; 	// command to set the initial condition to be read from ini_cond.txt
    	UImanager->ApplyCommand(command1);

#ifdef G4MULTITHREADED
  	// If we are in multithreading we give the number of threads to the particle generator so it reads correctly the ini cond file
  	std::cout << "Number of threads being used : " << numberOfcores << "\n";
  						
  	G4String command2 	= "/SK/generator/SetThreads "; 		// command to set the number of threads
  	G4String nCores 	= std::to_string(numberOfcores); 	// number of threads
  	UImanager->ApplyCommand(command2 + nCores);
#endif	

    	// running the visualization macro
    	UImanager->ApplyCommand("/control/execute init_vis.mac");	// command to run a macro
    	ui->SessionStart();
    	delete ui;
  }

  // remembering the seed used
  std::cout  << " \n" << "The initial random seed used was : " << seed << " \n \n";


  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !
  delete visManager;
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
