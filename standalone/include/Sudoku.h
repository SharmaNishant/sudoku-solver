//   _   _  _       _                    _
//  | \ | |(_)     | |                  | |
//  |  \| | _  ___ | |__    __ _  _ __  | |_
//  | . ` || |/ __|| '_ \  / _` || '_ \ | __|
//  | |\  || |\__ \| | | || (_| || | | || |_
//  \_| \_/|_||___/|_| |_| \__,_||_| |_| \__|
//
// Created by nishant on 21/1/16.
//

/**  Sample Sudoku Quadrant numbers
 ************************************
 *          *           *           *
 * Quad 1   *   Quad 2  *    Quad 3 *
 *          *           *           *
 ************************************
 *          *           *           *
 * Quad 4   *   Quad 5  *    Quad 6 *
 *          *           *           *
 ************************************
 *          *           *           *
 * Quad 7   *   Quad 8  *    Quad 9 *
 *          *           *           *
 ************************************
 */

#ifndef AISOFT_HW1_SUDOKU_H
#define AISOFT_HW1_SUDOKU_H

#include <vector>
#include <time.h>
#include "string"
using namespace std;

//zero is ignored in row, column
//rows are fixed

struct Cell{
	int value;
	bool isFixed;
};

struct Location{
	int row;
	int col;
	Location(){};
	Location(int r, int c)
	{
		row = r;
		col = c;
	};
};

class Sudoku {

public:
	Sudoku();
	Sudoku(string filename, unsigned int seed = time(NULL));
	Sudoku(Cell sudoku[9][9], unsigned int seed = time(NULL));
	virtual ~Sudoku();

	//initializes a sudoku based on the input
	bool fillSudoku();
	void PrintSudoku();
	void PrintSudoku(Location first, Location second);
	int GetFitness();
	void Swap(Location first, Location second);
	void SureSwap(Location first, Location second);
	int TestSwap(Location first, Location second);

	bool isFixed(Location loc);
	int getValue(Location loc);
	void calculateFitness();

private:
	//objects
	Cell sudoku[9][9];
	int column[9][10];
	int quadrant[9][10];
	int fitness;
	unsigned int seed;


	//initializes cost/fitness values
	void initializeMembers();
	//read sudoku from a file
	bool readSudoku_File(string filename);
	//returns fitness for a single column i
	int getColumnFitness(int col);
	//returns fitness for a single quadrant i
	int getQuadrantFitness(int quad);
	//returns fitness for all quadrants
	int getQuadrantFitness();
	//returns fitness for all columns
	int getColumnFitness();
	//returns quadrant value given a location
	int getQuadrantNumber(int row,int col);
	//generate fitness for quadrant
	void generateQuadrantFitness();
	//generate fitness for columns
	void generateColumnFitness();
	//generate a temp fitness value for a column
	int getColumnFitness(int col, int toRemove, int toAdd);
	//generate a temp fitness value for a quadrant
	int getQuadrantFitness(int quad, int toRemove, int toAdd);
	//update column fitness on a swap
	void updateColumnFitness(int col, int toRemove, int toAdd);
	//update quadrant fitness on a swap
	void updateQuadrantFitness(int quad, int toRemove, int toAdd);


};


#endif //AISOFT_HW1_SUDOKU_H
