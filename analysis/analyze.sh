#!/bin/bash

################################################################################
#                               analyze.sh                                     #
################################################################################
#
# This script initiates data analysis on raw ROOT trees containing from an
# elastic neutron scattering experiment at TUNL.
# In order to make data analysis modular (and to facilitate review) results from
# each stage of analysis are stored in ROOT files that are used for subsequent
# stages of analysis.
#
# The stages are:
#
# - ./readConfig, which reads the configuration data for the experimental setup
# in question (from ../configuration directory)
#
# - ./histos, which takes raw data in the form of one ROOT tree per experimental
# run (located in ../rawData/<experiment name> directory) and produces raw and
# PID-gated histograms for each detector (located in ../processedData/runs)
#
# - ./subtractBackground, which takes PID-gated histograms and produces
# difference histograms between target and blank for each detector angle
# (located in ../processedData/angles). This is the "signal" used to calculate
# cross sections.
#
# - ./calculateCS, which takes the difference histograms generated above and,
# using a reference cross section, calculates the cross section for each
# detector angle and target (results are written to ../results directory).
#
################################################################################
#                              HOW TO USE
################################################################################
#
# To start data analysis, run:
#
#   ./analyze.sh -<flag>
#
# where zero or more of the following flags are used.
#
#  Flag |                           Description
#-------+-----------------------------------------------------------------------
#    -h | recreate histograms from the raw root tree data (the default mode is
#       | not to recreate histograms if they already exist)
#-------+-----------------------------------------------------------------------
#
################################################################################

# Before starting analysis, recompile analysis code if necessary
make
if [ "$?" != 0 ] # "$?" is return value of previous command
then
    echo "Compilation failed - correct errors in source code."
    exit $?
fi

# read flags
while getopts "h" opt; do
    case ${opt} in
        h)
            recreateHistos=true
            if [[ $recreateHistos = true ]]
            then
                printf "\n Flag -h used: recreating histos for all runs.\n\n"
            fi
            ;;
        \?)
            # Flags unrecognized - exit script and give user help text
            printf "\nInvalid flag given.\n\nValid flags are:\n"
            printf "    -h (recreate histos from raw data trees)\n"

            exit
            ;;
    esac
done

################################################################################

experimentFileName="../configuration/experiment.txt"
if ! [ -f $experimentFileName ]
then
    printf "Error: failed to find $experimentFileName."
    exit
fi
read -r experiment<$experimentFileName # find out which experimental dataset to analyze

if [ -z $experiment ]
then
    printf "Error: failed to read experimental directory name from\
    $experimentFileName. Is the file empty?\n"
    exit
fi

printf "Analyzing data from $experiment\n\n"

if ! [[ -a "../configuration/$experiment/runConfig.txt" ]]
then
    printf "Error: failed to find runConfig.txt in\
    ../configuration/$experiment."
    exit
fi

./bin/driver "$experiment" "$recreateHistos"

#while read line
#do
#    IFS=' ' read -r -a tokens <<< "$line" # parse line into space-delimited tokens
#
#    re='[0-9]+'
#    if ! [[ "${tokens[0]}" =~ $re ]]
#    then
#        continue # skip lines that do not start with an integer
#    fi
#
#    runNumber=${tokens[0]}
#

#        printf "\n************************************"
#        printf "\n     Reading data from $runNumber"
#        printf "\n************************************\n"
#
#        # Send error messages to a text file
#        2>&1 | tee > "$outputDirectoryName"/error.txt
#
#        ./bin/histos "$inputFileName" "$outputDirectoryName"/histos.root
#    fi
#done < ../configuration/$experiment/runConfig.txt
#
##./subtractBackground
##./calculateCS
#
#printf "\nFinished analysis.\n\n"
