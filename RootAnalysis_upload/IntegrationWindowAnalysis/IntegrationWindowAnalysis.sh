#!/bin/bash

#################
### COMPILE   ###
#################
cd "${0%/*}"

g++ IntegrationWindowAnalysis.C -lstdc++fs `root-config --libs --cflags` -o IntegrationWindow -lSpectrum 

##################
### INITIALIZE ###
#################
echo "HALLO COMPUTER"
echo $PWD
echo "ls"
ls ../rootfiles/
echo "ls end"

 for file in ../rootfiles/*; do
	#without extension
	echo "file in loop"
	echo "$file"

	if [ ! -z "$1" ]; then

		if [[ $file == *"$1"* ]]; then
			echo $line
		else
			continue
		fi

	fi
	rootFilePath=$file

	prefix="../rootfiles/" # this used to be "../rootfiles//" but that didnt work as of wavecatcher update & subsequent header change 04/2021
	runName=${file#$prefix} #Remove prefix
	suffix=".root"
	runName=${runName%$suffix} #Remove suffix

	runNr=$(echo "$runName" | sed -r 's/^([^.]+).*$/\1/; s/^[^0-9]*([0-9]+).*$/\1/')
	echo "$runName" 
	echo "$runNr"
	echo "$rootFilePath"
	echo $PWD

	./IntegrationWindow $runName $runNr $rootFilePath 
done
 


