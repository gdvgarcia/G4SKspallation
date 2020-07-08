#!/bin/bash
#command to run jobs with GEANT4

#goes to the folder where I want to work
cd /data_T2K/dallavalle/simulation_test1/root_files_from_simu


# configuring conditions to run the geant4 application
module use /opt/exp_soft/vo.llr.in2p3.fr/modulefiles_el7
module load cmake/3.10.1
module load compilers/gcc/7.1.1
source /opt/exp_soft/llr/GEANT4/10.5.1/bin/geant4.sh


#command to launch geant4 jobs - we make one for each physlist with random initial conditions and one with the default physlist with fixed conditions
#$1 is related to launch.sh which gives the input file
/home/llr/t2k/dallavalle/G4SKspallation_geant4-build_SQ/G4SKspallation /home/llr/t2k/dallavalle/G4SKspallation_geant4-build_SQ/run3.mac ! $1 no
