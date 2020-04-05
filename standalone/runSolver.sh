#!/bin/bash
simType=SIMULATED_ANNEALING
seed=-1
temp=0.05
tempDrop=0.9995
tempIter=4000
for filename in "really-hard.txt" "allcircles.txt" "input3.txt" "input.blank.txt" "input.txt"
do
	for i in {1..10}
	do
		outputFile="partA/"$filename"-"$i
		outputFile+=".txt"
		#echo $outputFile
   		./Sudoku_Solver args $filename $simType $seed $temp $tempDrop $tempIter > $outputFile
	done
done
