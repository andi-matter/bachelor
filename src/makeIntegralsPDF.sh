#!/bin/bash

for file in "/mnt/d/Programme/RootAnalysis/RootAnalysis/finishedRootfiles/*";do
    echo $file 
done
running=true
# Ask user, which file(s) should get analysed
while $running; do
    echo "Enter run number to analyse (for all enter 'a'). 'q' to quit." # XXX
    read proceed
    case $proceed in
        [a]* ) runNumber=$a;;
        [q]* ) echo "quit!"; exit;;
        *) runNumber=$proceed; echo "Runnumber is ${runNumber}!";;
    esac

    echo "Start analysis? [y/n]"
    read answer
    case $answer in
        [Yy]* ) echo "Proceeding..."; break;;
		[Nn]* ) echo "Program aborted."; running=false; exit;;
		* ) echo "Please type Y/y or N/n.";;
    esac
    
done

# compile makeIntegralsPDF.cpp with libraries
echo "Compiling!"
g++ makeIntegralsPDF.cpp `root-config --libs --cflags` -o makeIntegralsPDF

# Check if the file was successfully compiled:
if [ -e makeIntegralsPDF ]
	then
	echo "Compilation successful."
else
	echo "Compilation error. rip"
	exit
fi

file=$(find /mnt/d/Programme/RootAnalysis/RootAnalysis/finishedRootfiles -name "*${runNumber}*" )
echo "file here!"
echo $file

./makeIntegralsPDF "$file"





















