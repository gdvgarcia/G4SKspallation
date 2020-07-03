#!/bin/bash
#set LOG_DIR=/home/llr/t2k/dallavalle/simulaion_test1/log
set ERR_DIR=/home/llr/t2k/dallavalle/simulaion_test1/err

#Command ./launch.sh full_path_to_folder_with_input_files 

#loop over input files
for input in $1/*
do

    # command to print the initial conditions file
    echo $input
    # command to submit the job in the cluster
    /opt/exp_soft/cms/t3/t3submit -long launchgeant.sh $input 

done
