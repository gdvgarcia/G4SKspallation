#!/bin/bash
#set LOG_DIR=/home/llr/t2k/dallavalle/simulaion_test/log
#set ERR_DIR=/home/llr/t2k/dallavalle/simulaion_test/err

#Command ./launch.sh full_path_to_folder_with_input_files 

#loop over input files
for input in $1/*
do

    echo $input
    #/opt/exp_soft/cms/t3/t3submit -long runG4.sh $input
    #/home/ggarcia/Programs/geant4/G4SKspallation2-build/G4SKspallation /home/ggarcia/Programs/geant4/G4SKspallation2-build/run2.mac

done
