//
// Created by nishant on 21/1/16.
//

#include "GlobalFunctions.h"
#include <iostream>
#include <unistd.h>
#include <cmath>
#include <assert.h>
#include "fstream"
#include "sstream"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

/**
 * This Function takes a Standard string and a delimiter to break the string into substrings
 */
std::vector<std::string> split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

/**
 * This function takes an int values [1-9] and prints it in color on terminal. Colors are pre-mapped to int values
 * */
void colorPrint(int n)
{
	switch (n)
	{
		case(1): cout<< "\033[1;31m" << n << "\033[0m";
			break;
		case(2): cout<< "\033[1;32m" << n << "\033[0m";
			break;
		case(3): cout<< "\033[1;33m" << n << "\033[0m";
			break;
		case(4): cout<< "\033[1;34m" << n << "\033[0m";
			break;
		case(5): cout<< "\033[1;35m" << n << "\033[0m";
			break;
		case(6): cout<< "\033[1;36m" << n << "\033[0m";
			break;
		case(7): cout<< "\033[0;31m" << n << "\033[0m";
			break;
		case(8): cout<< "\033[0;36m" << n << "\033[0m";
			break;
		case(9): cout<< "\033[0;32m" << n << "\033[0m";
			break;
		default: cout<< n;
	}

}

/**
 * This function will drop the temperature/probability value after some iterations. Input for this function are all global
 */
void DropTemperature()
{
	assert(temperatureDropIterations > 0 && "Value cannot be zero or less than zero");
	if(iterationCount % temperatureDropIterations == 0)
		temperature *= temperatureDropConstant;
}

/**
 * This function prints the final result on screen
 */
void DisplayResult(Sudoku &sudoku)
{
	if(iterationCount >= maxIterations)
	{
		cout << "\033[A\033[2K";
		cout << "\033[1;31mMaxed Out Iterations!!!\033[0m" << endl << endl;
	}
	else
	{
		cout << "\033[A\033[2K";
		cout << "\033[1;32mSolved!!!\033[0m" << endl << endl;
	}
	cout<<"Sudoku (Final)"<<endl;
	sudoku.PrintSudoku();
	cout<<"Fitness = " << sudoku.GetFitness() << endl << endl;
	cout<<"Number of iterations: " << iterationCount << endl;
}

/**
 * This function generates two random locations for swapping. (One Row and Two column, as we are maintaining rows)
 */
void GenerateSwapLocations(int &row, int &colOne, int &colTwo, Location &first, Location &second)
{
	row = rand() % 9;
	colOne = rand() % 9;
	colTwo = rand() % 9;
	first.row = second.row = row;
	first.col = colOne;
	second.col = colTwo;
}

/**
 * Hill Climbing Implementation (no bad move probability, just swap if better fitness is found)
 */
void HillClimbing(Sudoku &sudoku, unsigned int seed)
{
	//seeding with a different value
	srand(seed);
	//to choose a random row and two columns
	int row;
	int colOne, colTwo;
	Location first, second;
	while (sudoku.GetFitness() > 0 && iterationCount < maxIterations) {
		//get swap locations
		GenerateSwapLocations(row, colOne, colTwo, first, second);
		//check if the selected cells are not fixed or if they are the same values
		if (sudoku.isFixed(first) || sudoku.isFixed(second) || colOne == colTwo) {
			continue; // if any of the location is fixed then move on
		}
		int newFiteness = sudoku.TestSwap(first,second);
		//this will test and swap
		if(newFiteness costOperator sudoku.GetFitness())
		{
			sudoku.Swap(first, second);
		}

		if ((iterationCount % 1000000) == 0) {
			cout << "\033[A\033[2K";
			cout << iterationCount / 1000000 << " Million iterations passed. Current fitness: " <<
			sudoku.GetFitness() << endl;
		}
		++iterationCount;
	}
	DisplayResult(sudoku);
}

/**
 * Simulated Annealing Implementation (we use a probability to make a bad move and the probability goes down with time)
 */
void SimulatedAnnealing(Sudoku &sudoku, unsigned int seed)
{
	//seeding with a different value
	srand(seed);
	//to choose a random row and two columns
	int row;
	int colOne, colTwo;
	Location first, second;
	while (sudoku.GetFitness() > 0 && iterationCount < maxIterations)
	{
		//get locations
		GenerateSwapLocations(row, colOne, colTwo, first, second);
		//check if the selected cells are not fixed or if they are the same values
		if(sudoku.isFixed(first) || sudoku.isFixed(second) || colOne == colTwo)
		{
			continue; // if any of the location is fixed then move on
		}
		//get new Fitness value
		int newFitness = sudoku.TestSwap(first,second);
		double randVal = rand() % 100;
		double randValFloat = randVal / 10000000;

		if(newFitness costOperator sudoku.GetFitness())
		{
			sudoku.Swap(first, second);
		}
		else if(exp(((float) sudoku.GetFitness() - newFitness ) / temperature) - randValFloat > 0)
		{
			sudoku.SureSwap(first, second);
		}

		//Display Status after every million iterations
		if((iterationCount % 1000000) ==0)
		{
			cout << "\033[A\033[2K";
			cout << iterationCount / 1000000 << " Million iterations passed. Current fitness: " << sudoku.GetFitness() << endl;
		}
		//cout << "\n\033[2J\033[1;1H"; //clear screen
		//Drop Temperature
		DropTemperature();
		//updating iteration count
		++iterationCount;
	}
	DisplayResult(sudoku);
}

/**
 * Hill Climbing Implementation with a fixed probability of making a bad move over time
 */
void HillClimbingProbability(Sudoku &sudoku, unsigned int seed)
{
	//seeding with a different value
	srand(seed);
	//to choose a random row and two columns
	int row;
	int colOne, colTwo;
	Location first, second;
	while (sudoku.GetFitness() > 0 && iterationCount < maxIterations)
	{
		//get locations
		GenerateSwapLocations(row, colOne, colTwo, first, second);
		//check if the selected cells are not fixed or if they are the same values
		if(sudoku.isFixed(first) || sudoku.isFixed(second) || colOne == colTwo)
		{
			continue; // if any of the location is fixed then move on
		}
		//get new Fitness value
		int newFitness = sudoku.TestSwap(first,second);
		double randVal = rand() % 100;
		double randValFloat = randVal / 10000000;

		if(newFitness costOperator sudoku.GetFitness())
		{
			sudoku.Swap(first, second);
		}
		else if(exp(((float) sudoku.GetFitness() - newFitness ) / temperature) - randValFloat > 0)
		{
			sudoku.SureSwap(first, second);
		}

		//Display Status after every million iterations
		if((iterationCount % 1000000) ==0)
		{
			cout << "\033[A\033[2K";
			cout << iterationCount / 1000000 << " Million iterations passed. Current fitness: " << sudoku.GetFitness() << endl;
		}
		//cout << "\n\033[2J\033[1;1H"; //clear screen
		//updating iteration count
		++iterationCount;
	}
	DisplayResult(sudoku);
}

/**
 * This Function Reads Arguments from a pre defined launch file. This will help in saving recompilation time
 */
void ReadFromFile(string filename, string &fileName, SimulationType &simType, unsigned int &seed)
{
	string currentLine;
	ifstream launchFile(filename);
	if(launchFile.is_open())
	{
		//read comment 1st line
		getline (launchFile, currentLine);
		//read filename
		getline (launchFile, fileName);
		/**********/
		//read comment on 3rd line
		getline (launchFile, currentLine);
		//read type of simulation
		getline (launchFile, currentLine);
		if(currentLine.compare("HILL_CLIMBING") == 0 || currentLine[0] == '0')
			simType = HILL_CLIMBING;
		else if(currentLine.compare("HILL_CLIMBING_PROBABILITY") == 0 || currentLine[0] == '1')
			simType = HILL_CLIMBING_PROBABILITY;
		else
			simType = SIMULATED_ANNEALING; //this is default
		/**********/
		//read comment on 5th line
		getline (launchFile, currentLine);
		//read seed value
		getline (launchFile, currentLine);
		int seeder = stoi(currentLine);
		if(seeder == -1)
			seed = time(NULL);
		else
			seed = seeder;
		/**********/
		//read comment on 7th line
		getline (launchFile, currentLine);
		//read probability / temperature
		getline (launchFile, currentLine);
		temperature = stod(currentLine);
		assert(temperature >= 0 && temperature <=1 && "Temperature/Probability cannot be less than 0 or more than 1");
		/**********/
		//read comment on 9th line
		getline (launchFile, currentLine);
		//read temperature drop constant
		getline (launchFile, currentLine);
		temperatureDropConstant = stod(currentLine);
		assert(temperatureDropConstant < 1 && temperatureDropConstant >=0 && "Temperature drop constant range should be [0,1)");
		/**********/
		//read comment 11th line
		getline (launchFile, currentLine);
		//read type of simulation
		getline (launchFile, currentLine);
		temperatureDropIterations = stoi(currentLine);
		assert(temperatureDropIterations > 0 && "Temperature cannot be dropped in less than 1 iteration");
	}
	else
	{
		assert(!"Unable to open input file");
	}
}

/**
 * This Function reads arguments from the command line
 */
void ReadFromArgs(int argCount, char** argValues, string &fileName, SimulationType &simType, unsigned int &seed)
{
	//first argument is always checked
	//retrieving filename
	if(argCount > 2)
	{
		stringstream ss(argValues[2]);
		fileName = ss.str();
	}
	//retrieving simulation type
	if(argCount > 3)
	{
		stringstream ss(argValues[3]);
		if(ss.str().compare("HILL_CLIMBING") == 0 || ss.str()[0] == '0')
			simType = HILL_CLIMBING;
		else if(ss.str().compare("HILL_CLIMBING_PROBABILITY") == 0 || ss.str()[0] == '1')
			simType = HILL_CLIMBING_PROBABILITY;
		else
			simType = SIMULATED_ANNEALING; //this is default
	}
	//retrieving simulation type
	if(argCount > 4)
	{
		int seeder = atoi(argValues[4]);
		if(seeder < 0)
			seed = time(NULL);
		else
			seed = seeder;
	}
	//retrieving temperature
	if(argCount > 5)
	{
		temperature = atof(argValues[5]);
		assert(temperature >= 0 && temperature <=1 && "Temperature/Probability cannot be less than 0 or more than 1");
	}
	//retrieving
	if(argCount > 6)
	{
		temperatureDropConstant = atof(argValues[6]);
		assert(temperatureDropConstant < 1 && temperatureDropConstant >=0 && "Temperature drop constant range should be [0,1)");
	}
	//retrieving
	if(argCount > 7)
	{
		temperatureDropIterations = atoi(argValues[7]);
		assert(temperatureDropIterations > 0 && "Temperature cannot be dropped in less than 1 iteration");
	}

}
