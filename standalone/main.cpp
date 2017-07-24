#include <iostream>
#include "Sudoku.h"
#include "GlobalFunctions.h"
#include "fstream"

using namespace std;

int iterationCount;
double temperature;
double temperatureDropConstant;
int temperatureDropIterations;

string launchFile = "sudoku.launch";

/**
 * If no arguments are passed, try to search for launch file to run accordingly, otherwise blank sudoku with just hill climbing
 * otherwise one can pass '1' or 'file' followed by filename of the launch file to run this solver (See ReadMe)
 * or one can pass '2' or 'args' followed by arguments in the predefined order to run solver solver (See ReadMe)
 */
int main(int argCount, char** argValues)
{
	/** Initializing Variables with default values **/
	Sudoku sudoku;
	iterationCount = 0;
	temperature = 0;
	temperatureDropConstant = 0;
	temperatureDropIterations = 1;
	unsigned int seed = time(NULL);
	string filename;
	SimulationType  simType;

	//cout << argCount << endl;
	//checking input arguments
	if(argCount > 2)
	{
		string input = argValues[1];
		if(input[0] == '1' || input.compare("file") == 0 )
		{
			//cout<< "lq\n"<< input[1];
			ReadFromFile( argValues[2], filename, simType, seed );
			sudoku = Sudoku(filename, seed);
		}
		else if(input[0] == '2' || input.compare("args") == 0)
		{
			ReadFromArgs(argCount, argValues, filename, simType, seed);
			sudoku = Sudoku(filename, seed);
		}
		else
		{
			simType = HILL_CLIMBING;
		}
	}
	else
	{
		ifstream launchFileReader(launchFile);
		if(launchFileReader.is_open())
		{
			launchFileReader.close();
			ReadFromFile(launchFile, filename, simType, seed );
			//cout << filename << " - " << simType << endl;
			sudoku = Sudoku(filename, seed);
		}
		else
		{
			simType = HILL_CLIMBING;
		}
	}

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
	//SimulatedAnnealing(sudoku, seed);

	switch (simType)
	{
		case (SIMULATED_ANNEALING): SimulatedAnnealing(sudoku,seed);
			break;
		case (HILL_CLIMBING_PROBABILITY): HillClimbingProbability(sudoku,seed);
			break;
		case (HILL_CLIMBING):
		default:
			HillClimbing(sudoku,seed);
	}



	return 0;
}