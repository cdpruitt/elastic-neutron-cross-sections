#!/bin/bash

################################################################################
#                               analyze.sh                                     #
################################################################################
#
# This script initiates data analysis. It decides which experimental
# runs to analyze, then calls a series of C++ codes to process those runs.
# Several flags can be used to manage this workflow:
#
#  Flag |                           Description
#-------+-----------------------------------------------------------------------
#    -r | for each run given in ../<experiment>/runsToSort.txt, analyze the most
#       | recent subrun
#       | (e.g., ./analyze -r)
#-------+-----------------------------------------------------------------------
#
################################################################################

# SECTION 1: recompile analysis code

make
if [ "$?" != 0 ] # "$?" is return value of previous command
then
    echo "Compilation failed - correct errors in source code."
    exit $?
fi

################################################################################

# SECTION 2: process script flags

# Parse runtime flags
while getopts "rh" opt; do
    case ${opt} in
        r)
            runlist=true
            ;;
        h)
            removeHistos=true
            ;;
        \?)
            # Flags unrecognized - exit script and give user help text
            printf "\nInvalid flag given.\n\nValid flags are:\n"
            printf "    -r (analyze runs listed in ..runsToSort.txt)\n"
            printf "    -h (overwrite histos from previous analysis)\n"

            exit
            ;;
    esac
done

################################################################################

# SECTION 3: define analysis workflow

# This function is called once for each data set we want to analyze.

analyze ()
{
    inputFileName=$1
    outputDirectoryName=$2

    # Send error messages to a text file
    2>&1 | tee > "$outputDirectoryName"/error.txt

    if [[ $removeHistos = true ]]
    then
        rm "$outputDirectoryName"/histos.root
    fi

    ./histos "$inputFileName" "$outputDirectoryName"/histos.root
}

################################################################################

# SECTION 4: determine which runs should be analyzed

# Analyze runs listed in runsToSort.txt
if [[ $runlist = true && -a runsToSort.txt ]]
then
    printf "\nRunlist mode enabled. Reading runs from ./runsToSort.txt...\n"

    # loop through all runs listed in runsToSort.txt
    while read runNumber; do

        # Create directory to hold the output of our analysis
        if [ ! -d "../analyzedData/runs/$runNumber" ]
        then
            mkdir "../analyzedData/runs/$runNumber"
        fi

        printf "\n"
        printf "\n************************************"
        printf "\n     Reading data from $runNumber"
        printf "\n************************************\n"

        # Check to see if all subruns should be analyzed, or just one
        # Sort all sub-runs in the specified run directory
        for f in "../rawData/runs/$runNumber/tree.root";
        do
            outputDirectoryName="../analyzedData/runs/$runNumber"
            analyze "$f" "$outputDirectoryName"
        done

    done < runsToSort.txt

    # using each detector, calculate cross sections for all angles
    ./calculateCS 4M
    ./calculateCS 6M

    printf "\n\n"

    exit
fi
