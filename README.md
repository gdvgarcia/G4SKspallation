# G4SKspallation

Here you will find files related to simulations of the muon induced spallation at the Super-Kamiokande water Cherenkov detector with the software Geant4(version 10.06.p01).


The first simulation consists of downward muons with the SK average muon energy 271 GeV targeting the SK water tank (with no photomultiplier introduced) with a layer of 200 cm of water outside the upper part of detector(equivalent to 74 cm of rock as stated in the SK article), where we have taken as reference the CMS barrel hadron calorimeter(which is about 79 cm deep and which is a sampling calorimeter made of brass and plastic scintillator tiles), because we want some neutrons from outside interactions to be able to enter the SK tank(as for distances of interaction larger than this length, we would expect no neutron arriving in the detector). In 13 May 2020, we decided to no longer keep particle creation outside the detector. So particles now are generated right on top of the water inside the tank.

G4SKspallation1_v4.1 is the simplest simulation with only downward  fixed energy muons. It has been completed with a data reatriving method which fills histograms and ntuples per run and it can be also activated the mesh scoring method. While G4SKspallation1_v4_event has the same properties expect to the creation of histograms per event and not per run.


The second simulation consists of downward muons(with the extra possibility of activating the SK angular distribution) with the SK average muon energy 271 GeV (with the extra possibility of activating the SK muon energy spectrum) targeting the SK water tank (with no photomultiplier introduced) with a layer of 200 cm of water outside the upper part of detector(equivalent to 74 cm of rock as stated in the SK article), where we have taken as reference the CMS barrel hadron calorimeter(which is about 79 cm deep and which is a sampling calorimeter made of brass and plastic scintillator tiles), because we want some neutrons from outside interactions to be able to enter the SK tank(as for distances of interaction larger than this length, we would expect no neutron arriving in the detector). In 13 May 2020, we decided to no longer keep particle creation outside the detector. So particles now are generated right on top of the water inside the tank.

G4SKspallation2_v3 enhances the simulation by adding the possibility of generating muons with special angular and energy distributions with positions uniformly distributed on the top of the tank. This angular and energy distribution can be adjusted directly by an ASCII file. Some other improvements in code quality for data retrieving methods have also been added.  G4SKspallation2_v4 has the main change in the way ntuples are created. This version creates one ntuple per event with vectors containing information about the respective event. So we just have one tree per run. And G4SKspallation2_v2Shower contains many of the same features from the previous simulation, but it was created for the collection of special data. The study of the shower was its main objective, so the data ntuples and the histograms are very different from the main version simulations.


The third simulation adds the photomultipliers to the tank...

Geant4 default units : http://geant4.web.cern.ch/sites/geant4.web.cern.ch/files/geant4/collaboration/working_groups/electromagnetic/gallery/units/SystemOfUnits.html

References:

http://www-sk.icrr.u-tokyo.ac.jp/doc/sk/pub/sknimpaper.pdf
https://iopscience.iop.org/article/10.1088/1742-6596/293/1/012001/pdf
https://cds.cern.ch/record/357153/files/CMS_HCAL_TDR.pdf

Installing Geant4 : https://github.com/gdvgarcia/Geant4.6-4.5installation/wiki/GEANT4installation-wiki-everything-here
