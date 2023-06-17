/*****************************************************************//**
 * \Name:  start.h
 * \date:  April 12 2023
 * \Last update:  April 16 2023
 * \brief:   This c++ defines start.h and the functions it needed
 * \author:  Benson Hsieh

 *********************************************************************/

#pragma once

#include <string>

using namespace std;

void mapReset();
int calculateAdjacent(int row, int col);
void loadBoardFile(string fileName);
void loadRandomCount(int row, int col, int count);
void loadRandomRate(int row, int col, double rate);
void randomPutBomb();