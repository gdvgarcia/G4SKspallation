#! /bin/bash
#command to run jobs with GEANT4

#goes to the folder where I want to work
cd /home/llr/t2k/dallavalle/simulaion_test/root_files

#command to launch geant4 jobs - we make one for each physlist with random initial conditions and one with the default physlist with fixed conditions
#$1 is related to launch.sh which gives the input file
/home/llr/t2k/dallavalle/simulaion_test/G4SKspallation /home/llr/t2k/dallavalle/simulaion_test/run3.mac ! yes $1
/home/llr/t2k/dallavalle/simulaion_test/G4SKspallation /home/llr/t2k/dallavalle/simulaion_test/run3.mac 0 yes $1
/home/llr/t2k/dallavalle/simulaion_test/G4SKspallation /home/llr/t2k/dallavalle/simulaion_test/run3.mac 1 yes $1
/home/llr/t2k/dallavalle/simulaion_test/G4SKspallation /home/llr/t2k/dallavalle/simulaion_test/run3.mac 2 yes $1
/home/llr/t2k/dallavalle/simulaion_test/G4SKspallation /home/llr/t2k/dallavalle/simulaion_test/run3.mac 3 yes $1
/home/llr/t2k/dallavalle/simulaion_test/G4SKspallation /home/llr/t2k/dallavalle/simulaion_test/run3.mac

