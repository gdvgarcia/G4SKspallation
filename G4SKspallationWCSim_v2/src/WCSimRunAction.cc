#include "WCSimRunAction.hh"
#include "WCSimRunActionMessenger.hh"

#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"

#include "jhfNtuple.h"

#ifdef REFLEX_DICTIONARY
#include "Cintex/Cintex.h"
#endif
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TStreamerInfo.h"
#include "WCSimRootEvent.hh"
#include "WCSimRootGeom.hh"
#include "WCSimPmtInfo.hh"

// SPALLATION ADDITION : for data retrieving
#include "g4root.hh"
#include "WCSimEventAction.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

// SPALLATION ADDITION : libraries for the INCL coupling
#include "G4HadronicInteraction.hh"
#include "G4HadronicInteractionRegistry.hh"
#include "G4INCLXXInterfaceStore.hh"
#include "G4INCLXXInterface.hh"
#include "G4AblaInterface.hh"

#include <vector>

int pawc_[500000];                // Declare the PAWC common
struct ntupleStruct jhfNtuple;

WCSimRunAction::WCSimRunAction(WCSimDetectorConstruction* test, WCSimRandomParameters* rand, G4String physName) // SPALLATION ADDITION : the string
  : wcsimrandomparameters(rand), useTimer(false)
{
  ntuples = 1;

  // Messenger to allow IO options
  wcsimdetector = test;
  messenger = new WCSimRunActionMessenger(this);

  wcsimrootoptions = new WCSimRootOptions();

  PhysicsName = physName; // SPALLATION ADDITION 

  // SPALLATION ADDITION FOR ANALYSIS
  // Create analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;
  analysisManager->SetVerboseLevel(1);
  // Creating the ntuple
  //
  analysisManager->CreateNtuple("parents_data", "parents code and parents energy");

  //  n-tuple data
  analysisManager->CreateNtupleIColumn("ParentsCode"); 		//column ID 0 - parent particle code
  analysisManager->CreateNtupleDColumn("ParentsEnergy"); 	//column ID 1 - parent particle kinectic energy when creating 

  // finishing ntuple creation
  analysisManager->FinishNtuple();

  // Creating histograms
  //
  // 1D histograms
  analysisManager->CreateH1("Edep","Energy of deposited per event", 101, 0.1 , 100.5 * GeV );
  analysisManager->CreateH1("Emu","Initial energy of the muon", 1001, 0.1 , 10000.5 * GeV );
}

WCSimRunAction::~WCSimRunAction()
{
  delete G4AnalysisManager::Instance();   // SPALLATION ADDITION 
}

void WCSimRunAction::BeginOfRunAction(const G4Run* aRun) // SPALLATION UNCOMMENTED /*aRun*/ to aRun
{
  if(useTimer) {
    timer.Reset();
    timer.Start();
  }

  // SPALLATION ADDITION : coupling the INCL++ to the QGSP_INCLXX_HP list or to the QGSP_INCLXX list
  //
  if( PhysicsName == "QGSP_INCLXX_HP" || PhysicsName == "QGSP_INCLXX" ){ // check if we are using the QGSP_INCLXX_HP or the QGSP_INCLXX list
      	G4cout << "Coupling INCLXX to ABLA" << G4endl;
  	// Get hold of pointers to the INCL++ model interfaces
  	std::vector<G4HadronicInteraction *> interactions = G4HadronicInteractionRegistry::Instance()
  	  ->FindAllModels(G4INCLXXInterfaceStore::GetInstance()->getINCLXXVersionName());
  	for(std::vector<G4HadronicInteraction *>::const_iterator iInter=interactions.begin(), e=interactions.end();
   	  iInter!=e; ++iInter) {
    		G4INCLXXInterface *theINCLInterface = static_cast<G4INCLXXInterface*>(*iInter);
    		if(theINCLInterface) {
      			// Instantiate the ABLA model
      			G4HadronicInteraction *interaction = G4HadronicInteractionRegistry::Instance()->FindModel("ABLA");
      			G4AblaInterface *theAblaInterface = static_cast<G4AblaInterface*>(interaction);
      			if(!theAblaInterface){
        			theAblaInterface = new G4AblaInterface;
			}
      			// Couple INCL++ to ABLA
      			theINCLInterface->SetDeExcitation(theAblaInterface);
     		}
  	}
	G4cout << "\n\nABLA++ is coupled to the QGSP_INCLXX(_HP) physics list.\n\n" << G4endl;;
  }

  // SPALLATION ADDITION : number of events to be read from the muon file
   // getting UI manager to set information about the run
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Reading the right number of events from the initial conditions file
  // setting the number of events
  G4String command 	= "/mygen/SetNEvents "; 					// command to set the number of events
  G4String nEvents 	= std::to_string(aRun->GetNumberOfEventToBeProcessed()); 	// number of events beeing processed in this core
  UImanager->ApplyCommand(command + nEvents);


  
//   G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  numberOfEventsGenerated = 0;
  numberOfTimesWaterTubeHit = 0;
  numberOfTimesCatcherHit = 0;

#ifdef REFLEX_DICTIONARY
  ROOT::Cintex::Cintex::Enable();
#endif

  // Needed for Root 4.00/04
  WCSimRootEvent::Class()->GetStreamerInfo()->Optimize(kFALSE);
  // MF, aug 2006 ... you never know...
  WCSimRootTrigger::Class()->GetStreamerInfo()->Optimize(kFALSE);

  // Create the Root file

  // Now controlled by the messenger
  
  G4String rootname = GetRootFileName();
  TFile* hfile = new TFile(rootname.c_str(),"RECREATE","WCSim ROOT file");
  hfile->SetCompressionLevel(2);

  // Event tree
  WCSimTree = new TTree("wcsimT","WCSim Tree");

  wcsimrootsuperevent = new WCSimRootEvent(); //empty list
  wcsimrootsuperevent_OD = new WCSimRootEvent();
  //  wcsimrootsuperevent->AddSubEvent(); // make at least one event
  wcsimrootsuperevent->Initialize(); // make at least one event
  wcsimrootsuperevent_OD->Initialize(); // make at least one event
  Int_t branchStyle = 1; //new style by default
  TTree::SetBranchStyle(branchStyle);
  Int_t bufsize = 64000;

  //  TBranch *branch = tree->Branch("wcsimrootsuperevent", "Jhf2kmrootsuperevent", &wcsimrootsuperevent, bufsize,0);
  // TBranch *branch = WCSimTree->Branch("wcsimrootevent", "WCSimRootEvent", &wcsimrootsuperevent, bufsize,2);
  wcsimrooteventbranch = WCSimTree->Branch("wcsimrootevent", "WCSimRootEvent", &wcsimrootsuperevent, bufsize,2);
  wcsimrooteventbranch_OD = WCSimTree->Branch("wcsimrootevent_OD", "WCSimRootEvent", &wcsimrootsuperevent_OD, bufsize,2);

  // Geometry tree

  geoTree = new TTree("wcsimGeoT","WCSim Geometry Tree");
  wcsimrootgeom = new WCSimRootGeom();
  TBranch *geoBranch = geoTree->Branch("wcsimrootgeom", "WCSimRootGeom", &wcsimrootgeom, bufsize,0);

  FillGeoTree();

  // Options tree
  optionsTree = new TTree("wcsimRootOptionsT","WCSim Options Tree");
  optionsTree->Branch("wcsimrootoptions", "WCSimRootOptions", &wcsimrootoptions, bufsize, 0);

  //set detector & random options
  wcsimdetector->SaveOptionsToOutput(wcsimrootoptions);
  wcsimrandomparameters->SaveOptionsToOutput(wcsimrootoptions);

  // SPALLATION ADDITION : open file for the data
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file where to save the data collected
  //
  analysisManager->OpenFile("SK_run");

}

void WCSimRunAction::EndOfRunAction(const G4Run*)
{
  // SPALLATION ADDITION : saving the file 
  // print some histogram statistics - make sure you keep one 1D histograms in your simulation
  //
  auto analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->GetH1(0) ) {
    G4cout << G4endl << " ----> print histograms statistic ";
    
    G4cout << " Total energy deposit : mean = " 
       << G4BestUnit(analysisManager->GetH1(0)->mean(), "Energy") 
       << " rms = " 
       << G4BestUnit(analysisManager->GetH1(0)->rms(),  "Energy") << G4endl;
  }

  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();


//G4cout << "Number of Events Generated: "<< numberOfEventsGenerated << G4endl;
//G4cout << "Number of times OD hit: " << numberOfTimesMRDHit << G4endl;
//G4cout << "Number of times FGD hit: "    << numberOfTimesFGDHit << G4endl;
//G4cout << "Number of times lArD hit: "  << numberOfTimeslArDHit << G4endl;
//G4cout<<"Number of times waterTube hit: " << numberOfTimesWaterTubeHit<<G4endl;
//   G4cout << ((float(numberOfTimesMRDHit)+float(numberOfTimesFGDHit))/float(numberOfEventsGenerated))*100.
// 	 << "% hit FGD or MRD" << G4endl;
//   G4cout << "Number of times Catcher hit: " << numberOfTimesCatcherHit<<G4endl;
//   G4cout << "Number of times Rock hit: " << numberOfTimesRockHit<<G4endl;
//  G4cout << (float(numberOfTimesCatcherHit)/float(numberOfEventsGenerated))*100.
//        << "% through-going (hit Catcher)" << G4endl;

  //Write the options tree
  G4cout << "EndOfRunAction" << G4endl;
  optionsTree->Fill();
  optionsTree->Write();
  
  // Close the Root file at the end of the run

  TFile* hfile = WCSimTree->GetCurrentFile();
  hfile->Close();

  // Clean up stuff on the heap; I think deletion of hfile and trees
  // is taken care of by the file close

  delete wcsimrootsuperevent; wcsimrootsuperevent=0;
  delete wcsimrootsuperevent_OD; wcsimrootsuperevent_OD=0;
  delete wcsimrootgeom; wcsimrootgeom=0;

  if(useTimer) {
    timer.Stop();
    G4cout << "WCSimRunAction ran from BeginOfRunAction() to EndOfRunAction() in:"
	   << "\t" << timer.CpuTime()  << " seconds (CPU)"
	   << "\t" << timer.RealTime() << " seconds (real)" << G4endl;
  }

  // SPALLATION ADDITION
  // cleaning the file reader
  //
  // getting UI manager to set information about the run
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  G4String command 	= "/mygen/SetClearFlag "; 		// command to clear the file reader after a run
  UImanager->ApplyCommand(command);
}

void WCSimRunAction::FillGeoTree(){
  // Fill the geometry tree
  G4int geo_type;
  G4double cylinfo[3];
  G4double pmtradius;
  G4double pmtradiusOD;
  G4int numpmt;
  G4int numpmtOD;
  G4int orientation;
  Float_t offset[3];
  
  Int_t tubeNo;
  Float_t pos[3];
  Float_t rot[3];
  Int_t cylLoc;

  if (wcsimdetector->GetIsEggShapedHyperK()) {
      geo_type = 2;
  }
  else {
      geo_type = 0;
  }
  // geo_type 1 is for defunct mailbox design

  wcsimrootgeom-> SetGeo_Type(geo_type);

  if (geo_type == 0) {
      //cylinder
      cylinfo[1] = wcsimdetector->GetGeo_Dm(3);
      cylinfo[2] = wcsimdetector->GetGeo_Dm(2);
      wcsimrootgeom-> SetWCCylRadius(cylinfo[1]);
      wcsimrootgeom-> SetWCCylLength(cylinfo[2]);
  }


  pmtradius = wcsimdetector->GetPMTSize1();
  numpmt = wcsimdetector->GetTotalNumPmts();
  pmtradiusOD = wcsimdetector->GetODPMTSize();
  numpmtOD = wcsimdetector->GetTotalNumODPmts();
  orientation = 0;
  
  wcsimrootgeom-> SetWCPMTRadius(pmtradius);
  wcsimrootgeom-> SetODWCPMTRadius(pmtradiusOD);
  wcsimrootgeom-> SetOrientation(orientation);
  
  G4ThreeVector offset1= wcsimdetector->GetWCOffset();
  offset[0] = offset1[0];
  offset[1] = offset1[1];
  offset[2] = offset1[2];
  wcsimrootgeom-> SetWCOffset(offset[0],offset[1],offset[2]);
  
  std::vector<WCSimPmtInfo*> *fpmts = wcsimdetector->Get_Pmts();
  WCSimPmtInfo *pmt;
  for (unsigned int i=0;i!=fpmts->size();i++){
    pmt = ((WCSimPmtInfo*)fpmts->at(i));
    pos[0] = pmt->Get_transx();
    pos[1] = pmt->Get_transy();
    pos[2] = pmt->Get_transz();
    rot[0] = pmt->Get_orienx();
    rot[1] = pmt->Get_orieny();
    rot[2] = pmt->Get_orienz();
    tubeNo = pmt->Get_tubeid();
    cylLoc = pmt->Get_cylocation();
    wcsimrootgeom-> SetPMT(i,tubeNo,cylLoc,rot,pos);
  }
  if (fpmts->size() != (unsigned int)numpmt) {
    G4cout << "Mismatch between number of ID pmts and pmt list in geofile.txt!!"<<G4endl;
    G4cout << fpmts->size() <<" vs. "<< numpmt <<G4endl;
  }

  std::vector<WCSimPmtInfo*> *fODpmts = wcsimdetector->Get_ODPmts();
  for (unsigned int i=0;i!=fODpmts->size();i++){
    pmt = ((WCSimPmtInfo*)fODpmts->at(i));
    pos[0] = pmt->Get_transx();
    pos[1] = pmt->Get_transy();
    pos[2] = pmt->Get_transz();
    rot[0] = pmt->Get_orienx();
    rot[1] = pmt->Get_orieny();
    rot[2] = pmt->Get_orienz();
    tubeNo = pmt->Get_tubeid();
    cylLoc = pmt->Get_cylocation();
    wcsimrootgeom-> SetPMT(i,tubeNo,cylLoc,rot,pos);
  }
  if (fpmts->size() != (unsigned int)numpmt) {
    G4cout << "Mismatch between number of OD pmts and pmt list in geofile.txt!!"<<G4endl;
    G4cout << fODpmts->size() <<" vs. "<< numpmt <<G4endl;
  }

  wcsimrootgeom-> SetWCNumPMT(numpmt);
  wcsimrootgeom-> SetODWCNumPMT(numpmtOD);

  geoTree->Fill();
  geoTree->Write();
}
