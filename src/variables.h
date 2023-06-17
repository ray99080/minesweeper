/*****************************************************************//**
 * \file  variables.h
 * \create date  April 13 2023
 * \author ray
 * \date  April 13 2023
 * \description
 *********************************************************************/
#pragma once
#include"function.h"
#include"block.h"
#include"enum.h"
#include<fstream>

const int BOARD_MAXIMUM_SIZE = 100;

//global variables
extern int rowSize;
extern int columnSize;
extern int bombCount;
extern int flagCount;
extern int openBlankCount;
extern int remainBlankCount;
extern bool boardLoad; //whether board is loaded
extern bool gameResult; //true for win, false for loss
extern GameState gamestate;
extern Block board[100][100];
