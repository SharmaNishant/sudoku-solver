//   _   _  _       _                    _
//  | \ | |(_)     | |                  | |
//  |  \| | _  ___ | |__    __ _  _ __  | |_
//  | . ` || |/ __|| '_ \  / _` || '_ \ | __|
//  | |\  || |\__ \| | | || (_| || | | || |_
//  \_| \_/|_||___/|_| |_| \__,_||_| |_| \__|
//
// Created by nishant on 21/1/16.
//
#include "Sudoku.h"
#include "GlobalFunctions.h"
#include <cstdlib>
#include <climits>
#include <set>
#include <fstream>
#include <assert.h>
#include <iostream>

/** Generates a blank sudoku */
Sudoku::Sudoku() {
	this->initializeMembers();
	this->seed = time(NULL);
}

/** Generates a sudoku from the file and marks the non-zero as fixed */
Sudoku::Sudoku(string filename, unsigned int seed) {
	this->initializeMembers();
	this->readSudoku_File(filename);
	this->seed = seed;
}

Sudoku::Sudoku(Cell sudoku[9][9], unsigned int seed) {
	this->initializeMembers();
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			this->sudoku[i][j] = sudoku[i][j];
		}
	}
	this->seed = seed;
}

Sudoku::~Sudoku() {
}

/**
 * Function to print the current Sudoku on screen
 */
void Sudoku::PrintSudoku() {
	cout << "╔═════════╦═════════╦═════════╗"<<endl;
	for (int i = 0; i < 9; ++i) {
		cout << "║";
		for (int j = 0; j < 9; ++j)
		{
			//cout << " " << this->sudoku[i][j].value << " ";
			cout << " "; colorPrint(this->sudoku[i][j].value); cout << " ";
			if (((j + 1) % 3) == 0) {
				cout << "║";
			}
		}
		cout << endl;
		if (((i + 1) % 3) == 0 && (i !=8 )) {
			cout << "╠═════════╬═════════╬═════════╣"<<endl;
		}
	}
	cout << "╚═════════╩═════════╩═════════╝"<<endl;
}

/** Print Sudoku with the swap location as colored */
void Sudoku::PrintSudoku(Location first, Location second) {
	cout << "╔═════════╦═════════╦═════════╗"<<endl;
	for (int i = 0; i < 9; ++i) {
		cout << "║";
		for (int j = 0; j < 9; ++j) {
			if((first.row == i && first.col == j))
				cout << "\033[1;31m " << this->sudoku[i][j].value << " \033[0m";
			else if((second.row == i && second.col == j))
				cout << "\033[1;32m " << this->sudoku[i][j].value << " \033[0m";
			else
				cout << " " << this->sudoku[i][j].value << " ";
			if (((j + 1) % 3) == 0) {
				cout << "║";
			}
		}
		cout << endl;
		if (((i + 1) % 3) == 0 && (i !=8 )) {
			cout << "╠═════════╬═════════╬═════════╣"<<endl;
		}
	}
	cout << "╚═════════╩═════════╩═════════╝"<<endl;
}

int Sudoku::GetFitness() {
	return this->fitness;
}

/** Make a swap after making sure the new fitnes is '<' or '<=' based on the settings */
void Sudoku::Swap(Location first, Location second) {
	int firstValue = this->sudoku[first.row][first.col].value;
	if(this->sudoku[first.row][first.col].isFixed)
	{
		assert(!"Trying to swap a fixed location");
	}
	int secondValue = this->sudoku[second.row][second.col].value;
	if(this->sudoku[second.row][second.col].isFixed)
	{
		assert(!"Trying to swap a fixed location");
	}

	int newFitness = 0;
	//test swap columns and get updated fitness delta values
	if(first.col != second.col)
	{
		newFitness += getColumnFitness(first.col, firstValue, secondValue);
		//cout<< "newFitness col 1 " << newFitness << endl;

		newFitness += getColumnFitness(second.col, secondValue, firstValue);
		//cout<< "newFitness col 2 " << newFitness << endl;

	}

	//test swap quadrants and get updated fitness delta values
	int firstQuad = getQuadrantNumber(first.row,first.col);
	int secondQuad = getQuadrantNumber(second.row,second.col);

	if(firstQuad != secondQuad)
	{
		newFitness += getQuadrantFitness(firstQuad, firstValue, secondValue);
		//cout<< "newFitness quad "<< firstQuad << " " << newFitness << endl;
		newFitness += getQuadrantFitness(secondQuad, secondValue, firstValue);
		//cout<< "newFitness quad "<< secondQuad << " " << newFitness << endl;
	}

	//final new fitness calculation and check
	newFitness += fitness;
	//cout<< newFitness << " <= " << fitness << endl;



	if(newFitness costOperator fitness)
	{
		//cout<< newFitness << " < " << fitness << endl;
		//cout<<"swap\n";
		//make the swap
		//calculateFitness();
		//PrintSudoku();
		this->sudoku[first.row][first.col].value = secondValue;
		this->sudoku[second.row][second.col].value = firstValue;
		if(first.col != second.col) {
			updateColumnFitness(first.col, firstValue, secondValue);
			updateColumnFitness(second.col, secondValue, firstValue);
		}
		if(firstQuad != secondQuad) {
			updateQuadrantFitness(firstQuad, firstValue, secondValue);
			updateQuadrantFitness(secondQuad, secondValue, firstValue);
		}
		fitness = newFitness;
		//PrintSudoku();
		//exit(1);
	}
}

/** Just make a swap */
void Sudoku::SureSwap(Location first, Location second)
{
	int firstValue = this->sudoku[first.row][first.col].value;
	if(this->sudoku[first.row][first.col].isFixed)
	{
		assert(!"Trying to swap a fixed location");
	}
	int secondValue = this->sudoku[second.row][second.col].value;
	if(this->sudoku[second.row][second.col].isFixed)
	{
		assert(!"Trying to swap a fixed location");
	}

	int newFitness = 0;
	//test swap columns and get updated fitness delta values
	if(first.col != second.col)
	{
		newFitness += getColumnFitness(first.col, firstValue, secondValue);
		//cout<< "newFitness col 1 " << newFitness << endl;

		newFitness += getColumnFitness(second.col, secondValue, firstValue);
		//cout<< "newFitness col 2 " << newFitness << endl;

	}

	//test swap quadrants and get updated fitness delta values
	int firstQuad = getQuadrantNumber(first.row,first.col);
	int secondQuad = getQuadrantNumber(second.row,second.col);

	if(firstQuad != secondQuad)
	{
		newFitness += getQuadrantFitness(firstQuad, firstValue, secondValue);
		//cout<< "newFitness quad "<< firstQuad << " " << newFitness << endl;
		newFitness += getQuadrantFitness(secondQuad, secondValue, firstValue);
		//cout<< "newFitness quad "<< secondQuad << " " << newFitness << endl;
	}

	//final new fitness calculation and check
	newFitness += fitness;
	//cout<< newFitness << " <= " << fitness << endl;

	//make the swap
	//calculateFitness();
	//PrintSudoku();
	this->sudoku[first.row][first.col].value = secondValue;
	this->sudoku[second.row][second.col].value = firstValue;
	if(first.col != second.col) {
		updateColumnFitness(first.col, firstValue, secondValue);
		updateColumnFitness(second.col, secondValue, firstValue);
	}
	if(firstQuad != secondQuad) {
		updateQuadrantFitness(firstQuad, firstValue, secondValue);
		updateQuadrantFitness(secondQuad, secondValue, firstValue);
	}
	fitness = newFitness;
}


/**
 * Returns new fitness if hypothetically a swap is made
 */
int Sudoku::TestSwap(Location first, Location second) {
	int firstValue = this->sudoku[first.row][first.col].value;
	if(this->sudoku[first.row][first.col].isFixed)
	{
		assert(!"Trying to swap a fixed location");
	}
	int secondValue = this->sudoku[second.row][second.col].value;
	if(this->sudoku[second.row][second.col].isFixed)
	{
		assert(!"Trying to swap a fixed location");
	}

	int newFitness = 0;
	//test swap columns and get updated fitness delta values
	if(first.col != second.col)
	{
		newFitness += getColumnFitness(first.col, firstValue, secondValue);
		//cout<< "newFitness col 1 " << newFitness << endl;

		newFitness += getColumnFitness(second.col, secondValue, firstValue);
		//cout<< "newFitness col 2 " << newFitness << endl;

	}

	//test swap quadrants and get updated fitness delta values
	int firstQuad = getQuadrantNumber(first.row,first.col);
	int secondQuad = getQuadrantNumber(second.row,second.col);

	if(firstQuad != secondQuad)
	{
		newFitness += getQuadrantFitness(firstQuad, firstValue, secondValue);
		//cout<< "newFitness quad "<< firstQuad << " " << newFitness << endl;
		newFitness += getQuadrantFitness(secondQuad, secondValue, firstValue);
		//cout<< "newFitness quad "<< secondQuad << " " << newFitness << endl;
	}

	//final new fitness calculation and check
	newFitness += fitness;
	//cout<< newFitness << " <= " << fitness << endl;

	return newFitness;
}

bool Sudoku::isFixed(Location loc) {
	return this->sudoku[loc.row][loc.col].isFixed;
}

int Sudoku::getValue(Location loc) {
	return this->sudoku[loc.row][loc.col].value;
}

/* Fills sudoku with random values while making sure all the rows get unique values*/
bool Sudoku::fillSudoku() {
	//as safety calculate fitness first
	srand(this->seed);
	//calculateFitness();
	int randLoc;
	for (int row = 0; row < 9; ++row) {
		set<int> currentRow = {1,2,3,4,5,6,7,8,9};
		//populate currentRow vector
		for (int col = 0; col < 9; ++col) {
			currentRow.erase(this->sudoku[row][col].value);
//			cout<<"erased "<< this->sudoku[row][col].value<<endl;
		}
//		cout<<"row "<< row +1 << " is now \n";
//		for (int i : currentRow){
//			cout<< i << " ";
//		}
//
//		cout<<"seed "<< seed <<"\n";

		for (int colm = 0; colm < 9; ++colm) {
			if (this->sudoku[row][colm].value == 0)
			{
				randLoc = rand() % currentRow.size();
				//for extracting the value, save it and then delete it
				set<int>::const_iterator it(currentRow.begin());
				advance(it,randLoc);
				this->sudoku[row][colm].value = *it;
				currentRow.erase(it);
			}
		}


	}
	return false;
}

/** Return the count of the number of missing elements */
int Sudoku::getColumnFitness(int col) {
	int fitness=0;
	for (int val = 1; val < 10; ++val)
	{
		if(this->column[col][val]>0)
		{
			++fitness;
		}
	}
	//cout<< "Column "<< col + 1 <<"'s fitness: "<< fitness <<endl;
	return 9-fitness;
}

/** Return the count of the number of missing elements */
int Sudoku::getQuadrantFitness(int quad) {
	int fitness=0;
	for (int val = 1; val < 10; ++val)
	{
		if(this->quadrant[quad][val]>0)
		{
			++fitness;
		}
	}
	//cout<< "Quadrant "<< quad + 1 <<"'s fitness: "<< fitness <<endl;
	return 9-fitness;
}

/** Return the sum of fitness of the all quadrants */
int Sudoku::getQuadrantFitness() {
	int fitness = 0;
	for (int i = 0; i < 9; ++i) {
		fitness += getQuadrantFitness(i);
	}
	//cout<<"Quad Fitness: " << fitness <<endl;
	return fitness;
}

/** Return the sum of fitness of the all columns */
int Sudoku::getColumnFitness() {
	int fitness = 0;
	for (int j = 0; j < 9; ++j) {
		fitness += getColumnFitness(j);
	}
	//cout<<"Colm Fitness: " << fitness <<endl;
	return fitness;
}

int Sudoku::getQuadrantNumber(int row, int col) {
	if(row<=2 && col <= 2) return 0;
	else if(row<=2 && col <= 5) return 1;
	else if(row<=2 && col <= 8) return 2;
	else if(row<=5 && col <= 2) return 3;
	else if(row<=5 && col <= 5) return 4;
	else if(row<=5 && col <= 8) return 5;
	else if(row<=8 && col <= 2) return 6;
	else if(row<=8 && col <= 5) return 7;
	else if(row<=8 && col <= 8) return 8;
	else return -1;
}

void Sudoku::initializeMembers() {
	//A null cell to initialize sudoku with
	Cell null;
	null.isFixed = false;
	null.value = 0;
	//initializing sudoku to 0 and not fixed
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			this->sudoku[i][j] = null;
		}
	}
	//initializing columns and quadrants to not present in the beginning
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 10; ++j) {
			this->column[i][j] = false;
			this->quadrant[i][j] = false;
		}
	}
	//setting fitness to max
	this->fitness = INT_MAX;
}

bool Sudoku::readSudoku_File(string filename) {
	//we will keep updating this cell
	Cell currentCell;
	string currentRow;
	int currentReadValue;
	ifstream myFile (filename);
	if (myFile.is_open())
	{
		for (int i = 0; i < 9; ++i)
		{
			if( getline (myFile, currentRow) )
			{
				vector<string> currentRowValues = split(currentRow, ' ');
				for (int j = 0; j < 9; ++j)
				{
					currentReadValue = stoi(currentRowValues[j]);
					if(currentReadValue == 0)
					{
						currentCell.value = 0;
						currentCell.isFixed = false;
					}
					else if(0 < currentReadValue && currentReadValue < 10)
					{
						currentCell.value = currentReadValue;
						currentCell.isFixed = true;
					}
					else
					{
						assert(!"Out of range input value!!!");
					}
					this->sudoku[i][j] = currentCell;
				}
			}
			else
			{
				assert(!"Unable to read the input file correctly!!!");
			}
		}
		myFile.close();
		return true;
	}
	else
	{
		assert(!"Unable to open input file!!!");
	}
	return false;
}

void Sudoku::calculateFitness() {
	this->generateColumnFitness();
	this->generateQuadrantFitness();
	this->fitness = this->getQuadrantFitness() + this->getColumnFitness();
}

/** Sets the values of the fitness array with the number of times a value appears in the sub-square for each sub-square*/
void Sudoku::generateQuadrantFitness() {
	//set everything false before regenerating
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 10; ++j) {
			this->quadrant[i][j] = 0;
		}
	}
	//now calculating quadrant values
	int quadrantNumber = 0;
	for (int row = 0; row < 9; ++row) {
		for (int col = 0; col < 9; ++col) {
			quadrantNumber = getQuadrantNumber(row,col);
			this->quadrant[quadrantNumber][ this->sudoku[row][col].value ]++;
		}
	}

//	for (int row = 0; row < 9; ++row) {
//		cout << "Quadrant: " << row + 1 <<endl;
//		for (int col = 1; col < 10; ++col) {
//			cout<< col << " - " << this->quadrant[row][ col ] <<endl;
//		}
//	}

}

void Sudoku::generateColumnFitness() {
	//set everything false before regenerating
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 10; ++j) {
			this->column[i][j] = 0;
		}
	}
	//now calculating column values
	for (int row = 0; row < 9; ++row) {
		for (int col = 0; col < 9; ++col) {
			this->column[col][ this->sudoku[row][col].value ]++;
		}
	}
}

int Sudoku::getColumnFitness(int col, int toRemove, int toAdd) {
	int fitness = 0;
	//rows are distinct
	int remVal = this->column[col][toRemove];
	int addVal = this->column[col][toAdd];
	//cout << "----Column " << col+1 << endl;
	//cout<< "remove value count "<< remVal <<endl;
	//cout<< "add value count"<< addVal << endl;
	//if it exists only once, then our fitness will take a hit otherwise nah
	if(remVal <= 1) fitness++;
	//if it doesn't exist, then our fitness will increase otherwise nah
	if(addVal == 0) fitness--;
	//cout<< "return fitness value " << fitness << endl;
	return fitness;
}

int Sudoku::getQuadrantFitness(int quad, int toRemove, int toAdd) {
	int fitness = 0;
	int remVal = this->quadrant[quad][toRemove];
	int addVal = this->quadrant[quad][toAdd];
	//cout << "----Quadrant " << quad+1 << endl;
	//cout<< "remove value count "<< remVal <<endl;
	//cout<< "add value count"<< addVal << endl;
	//if it exists only once, then our fitness will take a hit otherwise nah
	if(remVal <= 1) fitness++;
	//if it doesn't exist, then our fitness will increase otherwise nah
	if(addVal == 0) fitness--;
	//cout<< "return fitness value " << fitness << endl;
	return fitness;
}

void Sudoku::updateColumnFitness(int col, int toRemove, int toAdd) {
	int remVal = this->column[col][toRemove]--;
	int addVal = this->column[col][toAdd]++;

	//sanity check
	if(this->column[col][toRemove] < 0) this->column[col][toRemove] = 0;
	if(this->column[col][toAdd] > 9) this->column[col][toAdd] = 9;

	//if it exists only once, then our fitness will take a hit otherwise nah
	if(remVal <= 1) fitness++;
	//if it doesn't exist, then our fitness will increase otherwise nah
	if(addVal == 0) fitness--;
}

void Sudoku::updateQuadrantFitness(int quad, int toRemove, int toAdd) {
	int remVal = this->quadrant[quad][toRemove]--;
	int addVal = this->quadrant[quad][toAdd]++;

	//sanity check
	if(this->quadrant[quad][toRemove] < 0) this->quadrant[quad][toRemove] = 0;
	if(this->quadrant[quad][toAdd] > 9) this->quadrant[quad][toAdd] = 9;

	//if it exists only once, then our fitness will take a hit otherwise nah
	if(remVal <= 1) fitness++;
	//if it doesn't exist, then our fitness will increase otherwise nah
	if(addVal == 0) fitness--;
}
