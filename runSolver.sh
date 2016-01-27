#!/bin/bash

for filename in "really-hard.txt" "allcircles.txt" "input3.txt" "input.blank.txt" "input.txt" 
do
	for i in {1..10}
	do
		outputFile = $filename$i
		outputFile += ".txt"
		echo $outputFile		
#   		Sudoku_Solver args 
	done
done
