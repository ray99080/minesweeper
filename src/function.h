/*****************************************************************//**
 * \file  function.h
 * \create date  April 12 2023
 * \author 張安睿、方柏文
 * \date  April 16 2023
 * \description other functions
 *********************************************************************/
#pragma once
#include<fstream>
#include<string>
#include<vector>



 //print in the output file
void printGameBoard(std::fstream& output);
void printGameAnswer(std::fstream& output);
void printGameState(std::fstream& output);
void printBombCount(std::fstream& output);
void printFlagCount(std::fstream& output);
void printOpenBlankCount(std::fstream& output);
void printRemainBlankCount(std::fstream& output);

//cout 
void coutGameBoard();
void coutGameAnswer();
void coutGameState();
void coutBombCount();
void coutFlagCount();
void coutOpenBlankCount();
void coutRemainBlankCount();

//other
void leftClick(int row, int column);
void rightClick(int row, int column);
bool checkPosition(int row, int column);
void GUIcommand(std::string command);
const std::vector<std::string> split(const std::string& str);