#include "WCSimPrimaryGeneratorMessenger.hh"
#include "WCSimPrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4ios.hh"

WCSimPrimaryGeneratorMessenger::WCSimPrimaryGeneratorMessenger(WCSimPrimaryGeneratorAction* pointerToAction)
:myAction(pointerToAction)
{
  mydetDirectory = new G4UIdirectory("/mygen/");
  mydetDirectory->SetGuidance("WCSim detector control commands.");

  genCmd = new G4UIcmdWithAString("/mygen/generator",this);
  genCmd->SetGuidance("Select primary generator.");
  //T. Akiri: Addition of laser
  genCmd->SetGuidance(" Available generators : muline, gun, laser, gps");
  genCmd->SetParameterName("generator",true);
  genCmd->SetDefaultValue("muline");
  //T. Akiri: Addition of laser
  genCmd->SetCandidates("muline gun laser gps");

  fileNameCmd = new G4UIcmdWithAString("/mygen/vecfile",this);
  fileNameCmd->SetGuidance("Select the file of vectors.");
  fileNameCmd->SetGuidance(" Enter the file name of the vector file");
  fileNameCmd->SetParameterName("fileName",true);
  fileNameCmd->SetDefaultValue("inputvectorfile");


  // SPALLATION ADDITION 
  fileNameMuonCmd = new G4UIcmdWithAString("/mygen/SetFileName",this);
  fileNameMuonCmd->SetGuidance("File name for the initial conditions of the generated muon (default value = ini_cond.txt).");
  fileNameMuonCmd->SetGuidance(" Enter the file name of the muon file");
  fileNameMuonCmd->SetParameterName("fFileName",true);
  fileNameMuonCmd->SetDefaultValue("ini_cond.txt");

  nOfEventsCmd = new G4UIcmdWithAnInteger("/mygen/SetNEvents",this);
  nOfEventsCmd->SetGuidance("Set the number of events to be read.");
  nOfEventsCmd->SetParameterName("fNEvents",true);
  nOfEventsCmd->SetDefaultValue(1);


  ClearFlagCmd = new G4UIcmdWithAnInteger("/mygen/SetClearFlag",this);
  ClearFlagCmd->SetGuidance("Set the flag to clear the file reader after a run.");
  ClearFlagCmd->SetParameterName("fClear",true);
  ClearFlagCmd->SetDefaultValue(1);


  AngleFlagCmd = new G4UIcmdWithAnInteger("/mygen/SetAngles",this);
  AngleFlagCmd->SetGuidance("If you do not want to read the angular distribution set it to 0.");
  AngleFlagCmd->SetParameterName("fAngles",true);
  AngleFlagCmd->SetDefaultValue(0);
}

WCSimPrimaryGeneratorMessenger::~WCSimPrimaryGeneratorMessenger()
{
  delete genCmd;
  delete mydetDirectory;
}

void WCSimPrimaryGeneratorMessenger::SetNewValue(G4UIcommand * command,G4String newValue)
{
  if( command==genCmd )
  {
    if (newValue == "muline")
    {
      myAction->SetMulineEvtGenerator(true);
      myAction->SetGunEvtGenerator(false);
      myAction->SetLaserEvtGenerator(false);
      myAction->SetGPSEvtGenerator(false);
    }
    else if ( newValue == "gun")
    {
      myAction->SetMulineEvtGenerator(false);
      myAction->SetGunEvtGenerator(true);
      myAction->SetLaserEvtGenerator(false);
      myAction->SetGPSEvtGenerator(false);
    }
    else if ( newValue == "laser")   //T. Akiri: Addition of laser
    {
      myAction->SetMulineEvtGenerator(false);
      myAction->SetGunEvtGenerator(false);
      myAction->SetLaserEvtGenerator(true);
      myAction->SetGPSEvtGenerator(false);
    }
    else if ( newValue == "gps")
    {
      myAction->SetMulineEvtGenerator(false);
      myAction->SetGunEvtGenerator(false);
      myAction->SetLaserEvtGenerator(false);
      myAction->SetGPSEvtGenerator(true);
    }
  }

  if( command == fileNameCmd )
  {
    myAction->OpenVectorFile(newValue);
    G4cout << "Input vector file set to " << newValue << G4endl;
  }

  // SPALLATION ADDITION
  if( command == fileNameMuonCmd )
  {
    myAction->SetMuonFileName(newValue);
    G4cout << "Input muon file set to " << newValue << G4endl;
  }

  if( command == nOfEventsCmd )
  {
    myAction->SetNevents(nOfEventsCmd->GetNewIntValue(newValue));
    G4cout << "Number of events is set to " << newValue << G4endl;
  }

  if( command == ClearFlagCmd )
  {
    myAction->SetClearFlag(ClearFlagCmd->GetNewIntValue(newValue));
    G4cout << "Flag FileReader clear is set to " << newValue << G4endl;
  }

  if( command == AngleFlagCmd )
  {
    myAction->SetAnglesFlag(AngleFlagCmd->GetNewIntValue(newValue));
    G4cout << "Flag angles reader is set to " << newValue << G4endl;
  }

}

G4String WCSimPrimaryGeneratorMessenger::GetCurrentValue(G4UIcommand* command)
{
  G4String cv;
  
  if( command==genCmd )
  {
    if(myAction->IsUsingMulineEvtGenerator())
      { cv = "muline"; }
    else if(myAction->IsUsingGunEvtGenerator())
      { cv = "gun"; }
    else if(myAction->IsUsingLaserEvtGenerator())
      { cv = "laser"; }   //T. Akiri: Addition of laser
    else if(myAction->IsUsingGPSEvtGenerator())
      { cv = "gps"; }
  }
  
  return cv;
}

