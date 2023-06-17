/*****************************************************************//**
 * \Name:  start.cpp
 * \date:  April 12 2023
 * \Last update:  April 16 2023
 * \brief:   This c++ program makes the function of start.h
 * \author:  Benson Hsieh

 *********************************************************************/

#include<cmath>
#include<algorithm>
#include<iostream>
#include<string>
#include<fstream>
#include"block.h"
#include"function.h"
#include"variables.h"
#include"enum.h"
#include"start.h"
#include<random>


using namespace std;

int calculateAdjacent(int row, int col);

/**
 * intent: load the file into board and check adjacency
 * pre: input the file name
 * post: load the file into board and check adjacency
 */
void loadBoardFile(string fileName)
{
	bombCount = 0;
	ifstream f;
	string temp;

	f.open(fileName);

	//reads rowsize and columnsize
	f >> rowSize >> columnSize;

	int rowcount = 0;
	while (f >> temp)
	{
		//puts bomb of file into the array
		for (int i = 0; i < temp.size(); i++)
		{
			if (temp[i] == 'X')
			{
				board[rowcount][i].setBomb();
				bombCount++;
			}
		}
		rowcount++;
	}

	//set adjacency of points into the array of Blocks
	for (int i = 0; i < rowSize; i++)
	{
		for (int j = 0; j < columnSize; j++)
		{
			//set adjacency of point to -1 if the point itself is a bomb
			if (board[i][j].get_isBomb() == 1)
			{
				board[i][j].set_adjcent(-1);
			}
			else
			{
				//calls function to calculate its adjacency
				board[i][j].set_adjcent(calculateAdjacent(i, j));
			}
		}
	}
}

/**
 * intent: make a random map based on input
  *   pre: input row, col and bombcount,
  *   post: make a random map based on input
 */
void loadRandomCount(int row, int col, int count)
{
	//set bombCOunt to count
	bombCount = count;

	//set rowSize to row and columnSize to col
	rowSize = row;
	columnSize = col;

	//create bombCount bombs
	for (int i = 0; i < bombCount; i++)
	{
		randomPutBomb();
	}

	//set adjacency of points into the array of Blocks
	for (int i = 0; i < rowSize; i++)
	{
		for (int j = 0; j < columnSize; j++)
		{
			//set adjacency of point to -1 if the point itself is a bomb
			if (board[i][j].get_isBomb() == 1)
			{
				board[i][j].set_adjcent(-1);
			}
			else
			{
				//calls function to calculate its adjacency
				board[i][j].set_adjcent(calculateAdjacent(i, j));
			}
		}
	}
}


/**
 * intent: make a random map based on input
  *   pre: input row, col and bombRate,
  *   post: make a random map based on input
 */
void loadRandomRate(int row, int col, double rate)
{
	//set bombCount
	bombCount = ceil(row * col * rate);

	//set rowSize to row and columnSize to col
	rowSize = row;
	columnSize = col;

	//create bombCount bombs
	for (int i = 0; i < bombCount; i++)
	{
		randomPutBomb();
	}

	//set adjacency of points into the array of Blocks
	for (int i = 0; i < rowSize; i++)
	{
		for (int j = 0; j < columnSize; j++)
		{
			//set adjacency of point to -1 if the point itself is a bomb
			if (board[i][j].get_isBomb() == 1)
			{
				board[i][j].set_adjcent(-1);
			}
			else
			{
				//calls function to calculate its adjacency
				board[i][j].set_adjcent(calculateAdjacent(i, j));
			}
		}
	}
}

/**
 * intent: calculate how many bombs are surrounding the point
 * pre: send the row and column of point required to check
 * post: return how many bombs are surrounding the point
 */
int calculateAdjacent(int row, int col)
{
	int count = 0;

	//travel from the top left corner to the right down corner to check
	for (int i = row - 1; i <= row + 1; i++)
	{
		for (int j = col - 1; j <= col + 1; j++)
		{
			//determine if the point is valid
			if (checkPosition(i, j) && (i != row || j != col))
			{
				//if the point is a bomb, adds count by 1
				if (board[i][j].get_isBomb() == 1)
				{
					count++;
				}
			}
		}
	}
	return count;
}

/**
 * intent: reset the whole map
 * pre: nothing
 * post: reset the whole map
 */
void mapReset()
{
	for (int i = 0; i < BOARD_MAXIMUM_SIZE; i++)
	{
		for (int j = 0; j < BOARD_MAXIMUM_SIZE; j++)
		{
			board[i][j].reset();
		}
	}
}

/**
 * intent: randomly put one bomb into the map
 * pre: nothing
 * post: randomly put one bomb into the map
 */
void randomPutBomb()
{
	bool created = 0;
	while (!created)
	{
		int createR = rand() % rowSize;
		int createC = rand() % columnSize;

		//check if the point is already a bomb
		if (board[createR][createC].get_isBomb() == 0)
		{
			board[createR][createC].setBomb();
			created = 1;
		}
	}
}