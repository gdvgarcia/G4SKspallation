#!/bin/bash

#Command ./launch.sh full_path_to_folder_with_input_files 

#loop over input files
for input in $1/*
do

    echo $input
    root -l -q "/home/ggarcia/Analysis/CovertMUSICtotxt.C(\"$input\")"


done
