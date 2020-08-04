// Visualization Manager


#include "WCSimVisManager.hh"
#include "G4TrajectoryDrawByParticleID.hh"

// Supported drivers...

// Not needing external packages or libraries...
#include "G4ASCIITree.hh"
#include "G4DAWNFILE.hh"
//#include "G4GAGTree.hh"
#include "G4HepRepFile.hh"
#include "G4HepRep.hh"
#include "G4RayTracer.hh"
#include "G4VRML1File.hh"
#include "G4VRML2File.hh"

// Needing external packages or libraries...

// #include "G4FukuiRenderer.hh" SPALLATION REMOVAL
// #include "G4Wo.hh" SPALLATION REMOVAL
// #include "G4Xo.hh" SPALLATION REMOVAL
#include "G4OpenGLImmediateX.hh"
#include "G4OpenGLStoredX.hh"
// #include "G4OpenGLImmediateWin32.hh" SPALLATION REMOVAL
// #include "G4OpenGLStoredWin32.hh" SPALLATION REMOVAL
#include "G4OpenGLImmediateXm.hh" 
#include "G4OpenGLStoredXm.hh"
#include "G4OpenGLImmediateQt.hh"
#include "G4OpenGLStoredQt.hh"
// #include "G4OpenInventorX.hh" SPALLATION REMOVAL
// #include "G4OpenInventorWin32.hh" SPALLATION REMOVAL
// #include "G4VRML1.hh" SPALLATION REMOVAL
// #include "G4VRML2.hh" SPALLATION REMOVAL

WCSimVisManager::WCSimVisManager () {}

void WCSimVisManager::RegisterGraphicsSystems () {

  // Graphics Systems not needing external packages or libraries...
  RegisterGraphicsSystem (new G4ASCIITree);
  RegisterGraphicsSystem (new G4DAWNFILE);
  //RegisterGraphicsSystem (new G4GAGTree);
  RegisterGraphicsSystem (new G4HepRepFile);
  RegisterGraphicsSystem (new G4HepRep);
  RegisterGraphicsSystem (new G4RayTracer);
  RegisterGraphicsSystem (new G4VRML1File);
  RegisterGraphicsSystem (new G4VRML2File);

  // Graphics systems needing external packages or libraries...

//  RegisterGraphicsSystem (new G4FukuiRenderer); SPALLATION REMOVAL
//  RegisterGraphicsSystem (new G4Wo); SPALLATION REMOVAL
//  RegisterGraphicsSystem (new G4Xo); SPALLATION REMOVAL
  RegisterGraphicsSystem (new G4OpenGLImmediateX);
  RegisterGraphicsSystem (new G4OpenGLStoredX);
//  RegisterGraphicsSystem (new G4OpenGLImmediateQt); SPALLATION REMOVAL
  RegisterGraphicsSystem (new G4OpenGLStoredQt);
//  RegisterGraphicsSystem (new G4OpenGLImmediateWin32); SPALLATION REMOVAL
//  RegisterGraphicsSystem (new G4OpenGLStoredWin32); SPALLATION REMOVAL
  RegisterGraphicsSystem (new G4OpenGLImmediateXm);
  RegisterGraphicsSystem (new G4OpenGLStoredXm);
//  RegisterGraphicsSystem (new G4OpenInventorX); SPALLATION REMOVAL
//  RegisterGraphicsSystem (new G4OpenInventorWin32); SPALLATION REMOVAL
//  RegisterGraphicsSystem (new G4VRML1); SPALLATION REMOVAL
//  RegisterGraphicsSystem (new G4VRML2); SPALLATION REMOVAL

  //create new drawByParticleID model
  G4TrajectoryDrawByParticleID* mymodel = new G4TrajectoryDrawByParticleID;
  
  //Configure model
  mymodel->SetDefault("cyan");
  mymodel->Set("gamma","green");
  mymodel->Set("nu_e","yellow");
  mymodel->Set("nu_mu","yellow");
  mymodel->Set("anti_nu_e","yellow");
  mymodel->Set("anti_nu_mu","yellow");
  mymodel->Set("e-","blue");
  mymodel->Set("mu-","white");
  mymodel->Set("e+","red");
  mymodel->Set("mu+",G4Colour(0.78, 0.78, 0.78));  //to distinguish mu+ from mu- on black background.
  mymodel->Set("proton","magenta");
  mymodel->Set("neutron","cyan");

  if (fVerbose > 0) {
    G4cout <<
      "\nYou have successfully chosen to use the following graphics systems."
	 << G4endl;
    // PrintAvailableGraphicsSystems (); SPALLATION REMOVAL
  }
  RegisterModel(mymodel);


}

