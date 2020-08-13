# G4SKspallation

##### Here you will find files related to simulations of the muon induced spallation at the Super-Kamiokande water Cherenkov detector with the software Geant4(version 10.06.p01).


## The first simulation consists of :
downward muons with the SK average muon energy 271 GeV targeting the SK water tank (with no photomultiplier introduced) with a layer of 200 cm of water outside the upper part of detector(equivalent to 74 cm of rock as stated in the SK article), where we have taken as reference the CMS barrel hadron calorimeter(which is about 79 cm deep and which is a sampling calorimeter made of brass and plastic scintillator tiles), because we want some neutrons from outside interactions to be able to enter the SK tank(as for distances of interaction larger than this length, we would expect no neutron arriving in the detector). In 13 May 2020, we decided to no longer keep particle creation outside the detector. So particles now are generated right on top of the water inside the tank.

G4SKspallation1_v4.1 is the simplest simulation with only downward  fixed energy muons. It has been completed with a data reatriving method which fills histograms and ntuples per run and it can be also activated the mesh scoring method. 

While G4SKspallation1_v4_event has the same properties expect to the creation of histograms per event and not per run.


## The second simulation consists of :
downward muons(with the extra possibility of activating the SK angular distribution or choose a user's angular distribution file) with the SK average muon energy 271 GeV (with the extra possibility of activating the SK muon energy spectrum or choose a user's energy distribution file) targeting the SK water tank with particles generated right on the upper and side borders of the tank.

G4SKspallation2_v3 enhances the simulation by adding the possibility of generating muons with special angular and energy distributions with positions uniformly distributed on the top of the tank. This angular and energy distribution can be adjusted directly by an ASCII file. Some other improvements in code quality for data retrieving methods have also been added.  

G4SKspallation2_v5 has the main change in the way ntuples are created and the main improvement that one can run this simulation in multithreading mode. This version creates one ntuple row per event with vectors containing information about the respective event.  So we just have one tree per run. 

G4SKspallation2_v6 improves the initial conditions reader and has the possibility of reading the file of user's choice by a third argument in batch mode. It alsos improve the random generation of positions of the primary particles.

G4SKspallation2_v7 maintains main aspects of v6 but it also has a substantial change in the retrieving method, no longer using Sensitive Detectors and Hits, but user's SteppingAction. This makes retrieving data much more intuitive. It has also improvements in the output file generation and the possibility of gerenating muons plus with a fourth argument in batch mode. 

G4SKspallation2_v8 just add the possibility of deactivating the angular distribution reader from the input file by the addition of a fourth argument(making the muon plus argument the fifth).

G4SKspallation2_vF adds the possibility of not saving data from events which do not satisfy conditions that we choose (here, events that have no isotopes with atomic masses greater or equal 8 produced) by the addition of a sixth argument.

And G4SKspallation2_v2Shower contains many of the same features from the previous simulation v3, but it was created for the collection of special data. The study of the shower was its main objective, so the data ntuples and the histograms are very different from the main version simulations. 

G4SKspallation2_v4OutPart was developped to verify that the outside conditions for the studies inside the water tank were negligible, showing that less than 1% of the particles present inside the tank are produced outside the tank. This simulation is the version 4 with some variations for saving the number of particles produced in and out the tank.


## The third simulation consists of :
the integrtion of a correct physics list choice and muon generation (the same as for G4SKspallation2) whitin the simulation developed by the WCSim group. It contains then the right full geometry of SK and also the reconstruction of events in the PMTs. This new simulation is named G4SKspallationWCSim.

G4SKspallationWCSim_v0 was the minimal changes in the master branch of the WCSim simulation for it to run in the current version of Geant4 (v10.06.p01). G4SKspallationWCSim_v1 has the changes necessary for the muon generation from our initial conditions file, for the right physics list choice (even the ABLA coupling) and for a random seeding depending on the starting time of opening of the simulation application. G4SKspallationWCSim_v2 has adds our root file data retriving method (a bit different from our second simulation - more simple than that one, because we did not have access to the EventAction in the RunAction). The data taken in v2 is the isotopes parent particles spectra and the histogram of the mon initial energy and the muon energy loss. G4SKspallationWCSim_v3 has a new class that makes the retrieving method from G4SKspallation2 possible. So it has a tree with the data about isotopes, their parents and initial muons. G4SKspallationWCSim_v4 keeps all the previous advances and integrates a unique method of naming the root files created from the original WCSim simulation and adds a flag to only keep data for events which contain the production of isotopes with atomic masses equal or over 8.

### For v1, changes in : 

* WCSim.cc
* WCSimPhysicsListFactory.hh
* WCSimRunAction.hh
* WCSimRunAction.cc
* WCSimPrimaryGeneratorAction.hh
* WCSimPrimaryGeneratorAction.cc
* WCSimPrimaryGeneratorMessenger.hh
* WCSimPrimaryGeneratorMessenger.cc
* WCSimDetectorConstruction.hh

* CMakeLists.txt

* Also our SKFileReader was simplified


### For v2, v3, v4, changes in : 

* WCSim.cc
* WCSimPhysicsListFactory.hh
* WCSimRunAction.hh
* WCSimRunAction.cc
* WCSimPrimaryGeneratorAction.hh
* WCSimPrimaryGeneratorAction.cc
* WCSimPrimaryGeneratorMessenger.hh
* WCSimPrimaryGeneratorMessenger.cc
* WCSimDetectorConstruction.hh
* WCSimDetectorConstruction.cc
* WCSimSteppingAction.hh
* WCSimSteppingAction.cc
* WCSimEventAction.hh
* WCSimEventAction.cc

* CMakeLists.txt

* Also our SKFileReader was simplified

#### Special for v3, v4 are changes in the macros : 

* jobOptions.mac
* WCSim.mac


**Note :** The directory WCSim-master is the master branch of WCSim taken on July 31 2020.

### Issues with the G4SKspallationWCSim :
There are three warnings happening that are problably coming from the choice of Geant4 version for the simulation. One of this warnings reduces the quality of the physics list for the low energy neutron transport. The other two have no real implications. These two are geometrical issues and the physics list is related to the location of neutron data. To solve them, use the official version of Geant4 for WCSim and just copy the changes I've made, not deleting the compiler flags (that were removed for the simulation to run in the newest version).


## References:

http://www-sk.icrr.u-tokyo.ac.jp/doc/sk/pub/sknimpaper.pdf
https://iopscience.iop.org/article/10.1088/1742-6596/293/1/012001/pdf
https://cds.cern.ch/record/357153/files/CMS_HCAL_TDR.pdf

## Useful info :

#### Geant4 default units : http://geant4.web.cern.ch/sites/geant4.web.cern.ch/files/geant4/collaboration/working_groups/electromagnetic/gallery/units/SystemOfUnits.html

#### Geant4 Particle Data Group Integer Identifier : https://home.fnal.gov/~mrenna/lutp0613man2/node44.html or http://pdg.lbl.gov/2019/reviews/rpp2019-rev-monte-carlo-numbering.pdf - at the end of the document

#### Installing Geant4 : https://github.com/gdvgarcia/Geant4.6-4.5installation/wiki/GEANT4installation-wiki-everything-here
