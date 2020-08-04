#include "G4ios.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "WCSimDetectorConstruction.hh"
#include "WCSimPhysicsListFactory.hh"
#include "WCSimPhysicsListFactoryMessenger.hh"
#include "WCSimTuningParameters.hh"
#include "WCSimTuningMessenger.hh"
#include "WCSimPrimaryGeneratorAction.hh"
#include "WCSimEventAction.hh"
#include "WCSimRunAction.hh"
#include "WCSimStackingAction.hh"
#include "WCSimTrackingAction.hh"
#include "WCSimSteppingAction.hh"
#include "WCSimVisManager.hh"
#include "WCSimRandomParameters.hh"
#include "G4UIExecutive.hh"

#include "G4VisExecutive.hh" // ADDED SPALLATION

#include <chrono>  // for high_resolution_clock - SPALLATION ADDITION

void file_exists(const char * filename) {
  bool exists = access(filename, F_OK) != -1;
  if(!exists) {
    G4cerr << filename << " not found or inaccessible. Exiting" << G4endl;
    exit(-1);
  }
}

int main(int argc,char** argv)
{
  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;

  // get the pointer to the UI manager
  G4UImanager* UI = G4UImanager::GetUIpointer();

  // Set up the tuning parameters that need to be read before the detector
  //  construction is done
  WCSimTuningParameters* tuningpars = new WCSimTuningParameters();

  // Get the tuning parameters
  file_exists("macros/tuning_parameters.mac");
  UI->ApplyCommand("/control/execute macros/tuning_parameters.mac");

  // define random number generator parameters
  WCSimRandomParameters *randomparameters = new WCSimRandomParameters();

  //
  // SPALLATION ADDITION : random seed with the time
  //
  // -------------------------------------------------------------------------------------
  // Record time
  auto time = std::chrono::high_resolution_clock::now(); // getting the time now

  // Converting the time into an int taking the milisecs into account 
  double t_1 = std::chrono::duration_cast<std::chrono::duration<double>>(time.time_since_epoch()).count();
  t_1 = t_1 * 1000. - 1592000000000.; // multiplied by 1 000  to get milisecs precision and removing the large unnecessary number of 1592000000000 msecs 
  
  // Using the time for the seed
  long seed = (long)t_1;
  int iseed = (int)(seed%2147483647); // making it smaller than the largest int
  randomparameters->SetSeed(iseed);

  // -------------------------------------------------------------------------------------


  // UserInitialization classes (mandatory)
  enum DetConfiguration {wfm=1,fwm=2};
  G4int WCSimConfiguration = fwm;

  WCSimDetectorConstruction* WCSimdetector = new 
    WCSimDetectorConstruction(WCSimConfiguration,tuningpars);

  runManager->SetUserInitialization(WCSimdetector);

  // Added selectable physics lists 2010-07 by DMW
  // Set up the messenger hooks here, initialize the actual list after loading jobOptions.mac
  WCSimPhysicsListFactory *physFactory = new WCSimPhysicsListFactory();

  // Currently, default physics list is set to FTFP_BERT
  // The custom WCSim physics list option is removed in versions later than WCSim1.6.0
  file_exists("macros/jobOptions.mac");
  UI->ApplyCommand("/control/execute macros/jobOptions.mac");

  // Initialize the physics factory to register the selected physics.
  physFactory->InitializeList();
  runManager->SetUserInitialization(physFactory);

  // Visualization
  //G4VisManager* visManager = new WCSimVisManager;
  //visManager->Initialize();

  // SPALLATION CHOICE
  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive("Quiet");
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  visManager->Initialize();

  // Set user action classes
  WCSimPrimaryGeneratorAction* myGeneratorAction = new 
    WCSimPrimaryGeneratorAction(WCSimdetector);
  runManager->SetUserAction(myGeneratorAction);

  //
  // SPALLATION ADDITION : RunAction name of the physics list, so 
  // 	we can check for coupling or not the ABLA model
  //
  WCSimRunAction* myRunAction = new WCSimRunAction(WCSimdetector, randomparameters, physFactory->GetPhysicsListName());

  //save all the options from WCSimTuningParameters & WCSimPhysicsListFactory
  //(set in tuning_parameters.mac & jobOptions*.mac)
  tuningpars->SaveOptionsToOutput(myRunAction->GetRootOptions());
  physFactory->SaveOptionsToOutput(myRunAction->GetRootOptions());

  runManager->SetUserAction(myRunAction);

  runManager->SetUserAction(new WCSimEventAction(myRunAction, WCSimdetector,
						 myGeneratorAction));
  runManager->SetUserAction(new WCSimTrackingAction);

  runManager->SetUserAction(new WCSimStackingAction(WCSimdetector));

  runManager->SetUserAction(new WCSimSteppingAction);


  // Initialize G4 kernel
  runManager->Initialize();


  if (argc==1)   // Define UI terminal for interactive mode  
  { 
    // Start UI Session
    // G4UIsession* session =  new G4UIterminal(new G4UItcsh);

    //using working example N04 for Qt UI Compatible code
    G4UIExecutive * ui = new G4UIExecutive(argc,argv);

    // Visualization Macro
    UI->ApplyCommand("/control/execute WCSim.mac");
    ui->SessionStart();
    delete ui;

    // Start Interactive Mode
    //session->SessionStart();

    //delete session;
  }
  else           // Batch mode
  { 
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    file_exists(fileName);
    if(fileName == "vis.mac"){
      G4cout << "ERROR: Execute without arg for interactive mode" << G4endl;
      //return -1;
    }

    UI->ApplyCommand(command+fileName);
  }

  // SPALLATION ADDITION : check seeding
  // std::cout << " \n Random seed will be set as " << iseed << " .\n";
  std::cout << " \n Random seed is  : " << randomparameters->GetSeed() << " \n";

  delete visManager;

  delete runManager;
  return 0;
}


