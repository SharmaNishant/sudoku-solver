#include <iostream>
#include "Sudoku.h"
#include "GlobalFunctions.h"

using namespace std;

int iterationCount;
double temperature;
double temperatureDropConstant;
int temperatureDropIterations;

int main(int argCount, char** argValues)
{

	Sudoku sudoku;
	iterationCount = 0;
	unsigned int seed = time(NULL);
	//checking input arguments
	if(argCount == 2)
	{
		sudoku = Sudoku(argValues[1]);
	}
	else if(argCount > 2)
	{
		seed = atoi(argValues[2]);
		sudoku = Sudoku(argValues[1], seed);
	}

	//Sudoku sudoku;
	cout<<"Sudoku (from input file)"<<endl;
	sudoku.PrintSudoku();
	sudoku.calculateFitness();
	cout<<"Fitness = " << sudoku.GetFitness() << endl << endl << endl;

	cout<<"Sudoku (Initialized)"<<endl;
	sudoku.fillSudoku();
	sudoku.PrintSudoku();
	sudoku.calculateFitness();
	cout<<"Fitness = " << sudoku.GetFitness() << endl << endl << endl;

	//HillClimbing(sudoku, seed);
	SimulatedAnealing(sudoku,seed);

	cout<<"Sudoku (Final)"<<endl;
	sudoku.PrintSudoku();
	cout<<"Fitness = " << sudoku.GetFitness() << endl << endl;
	cout<<"Number of iterations: " << iterationCount << endl;

	return 0;
}