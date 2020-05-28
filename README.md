# G4SKspallation

Here you will find files related to simulations of the muon induced spallation at the Super-Kamiokande water Cherenkov detector with the software Geant4(version 10.06.p01).

The first simulation consists of downward muons(SK angular distribution can be activated) with the SK average muon energy 271 GeV ( the SK muon energy spectrum can be activated) targeting the SK water tank (with no photomultiplier introduced) with a layer of 200 cm of water outside the upper part of detector(equivalent to 74 cm of rock as stated in the SK article), where we have taken as reference the CMS barrel hadron calorimeter(which is about 79 cm deep and which is a sampling calorimeter made of brass and plastic scintillator tiles), because we want some neutrons from outside interactions to be able to enter the SK tank(as for distances of interaction larger than this length, we would expect no neutron arriving in the detector). In 13 May 2020, we decided to no longer keep particle creation outside the detector. So particles now are generated right on top of the water inside the tank. 

The second simulation adds the photomultipliers to the tank...


References:

http://www-sk.icrr.u-tokyo.ac.jp/doc/sk/pub/sknimpaper.pdf
https://iopscience.iop.org/article/10.1088/1742-6596/293/1/012001/pdf
https://cds.cern.ch/record/357153/files/CMS_HCAL_TDR.pdf
