#!/bin/bash
# echo ./*
var=./*
runNumber="data"
# echo $var
for dir in $var; do
	printf $dir 
	printf "\n ylgsodfij \n"
	if [[ $dir == *"${runNumber}"* ]]; then
		memDir=$dir
		printf "memDir "
		printf $memDir
		for file in ${memDir}/*; do
			echo " test here   "
			printf $file
			if [[ $file == *"bin"* ]]; then
				memFile=$file
				printf "rootfile found  "
				printf $memFile 
				break
			fi 
		done 
		break
	fi
done 