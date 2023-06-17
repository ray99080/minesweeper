/*****************************************************************//**
 * \file  function.cpp
 * \create date  April 12 2023
 * \author
 * \date  April 16 2023
 * \description other functions
 *********************************************************************/

#include<iostream>
#include<string>
#include<fstream>
#include"Block.h"
#include"function.h"
#include"variables.h"
#include"enum.h"
#include"start.h"
#include<vector>
#include<sstream>

using namespace std;

//print in the output file
//print game board
void printGameBoard(fstream& output)
{
	//print char/int depending on its state
	for (int i = 0; i < rowSize; i++) {
		for (int j = 0; j < columnSize; j++) {
			if (board[i][j].get_isOpen()) {
				output << board[i][j].get_adjcent() << " ";
			}
			else if (board[i][j].get_isClosed()) {
				output << "#" << " ";
			}
			else if (board[i][j].get_isFlaged()) {
				output << "f" << " ";
			}
			else if (board[i][j].get_isQuestionMarked()) {
				output << "?" << " ";
			}
		}
		output << endl;
	}
}

//print game answer
void printGameAnswer(fstream& output)
{
	//print char/int depending on its state
	for (int i = 0; i < rowSize; i++) {
		for (int j = 0; j < columnSize; j++) {
			if (!(board[i][j].get_isBomb())) {
				output << board[i][j].get_adjcent() << " ";
			}
			else if (board[i][j].get_isBomb()) {
				output << "X" << " ";
			}
		}
		output << endl;
	}
}

//print game state
void printGameState(fstream& output)
{
	if (gamestate == GameState::STANDBY) {
		output << "Standby" << endl;
	}
	else if (gamestate == GameState::PLAYING) {
		output << "Playing" << endl;
	}
	else if (gamestate == GameState::GAMEOVER) {
		output << "GameOver" << endl;
	}
}

//print bomb count
void printBombCount(fstream& output) { output << bombCount << endl; }

//print flag count
void printFlagCount(fstream& output) { output << flagCount << endl; }

//print opened blank count
void printOpenBlankCount(fstream& output) { output << openBlankCount << endl; }

//print remained blank count
void printRemainBlankCount(fstream& output) { output << remainBlankCount << endl; }

//cout
//cout game board
void coutGameBoard()
{
	//print char/int depending on its state
	for (int i = 0; i < rowSize; i++) {
		for (int j = 0; j < columnSize; j++) {
			if (board[i][j].get_isOpen()) {
				cout << board[i][j].get_adjcent() << " ";
			}
			else if (board[i][j].get_isClosed()) {
				cout << "#" << " ";
			}
			else if (board[i][j].get_isFlaged()) {
				cout << "f" << " ";
			}
			else if (board[i][j].get_isQuestionMarked()) {
				cout << "?" << " ";
			}
		}
		cout << endl;
	}
}

//cout game answer
void coutGameAnswer()
{
	//print char/int depending on its state
	for (int i = 0; i < rowSize; i++) {
		for (int j = 0; j < columnSize; j++) {
			if (!(board[i][j].get_isBomb())) {
				cout << board[i][j].get_adjcent() << " ";
			}
			else if (board[i][j].get_isBomb()) {
				cout << "X" << " ";
			}
		}
		cout << endl;
	}
}

//cout game state
void coutGameState()
{
	if (gamestate == GameState::STANDBY) {
		cout << "Standby" << endl;
	}
	else if (gamestate == GameState::PLAYING) {
		cout << "Playing" << endl;
	}
	else if (gamestate == GameState::GAMEOVER) {
		cout << "GameOver" << endl;
	}
}

//cout bomb count
void coutBombCount() { cout << bombCount << endl; }

//cout flag count
void coutFlagCount() { cout << flagCount << endl; }

//cout opened blank count
void coutOpenBlankCount() { cout << openBlankCount << endl; }

//cout remained blank count
void coutRemainBlankCount() { cout << remainBlankCount << endl; }

//open the blank, if its adjcent is 0, then open 8 blank around it 
void leftClick(int row, int column) {
	if (board[row][column].get_isBomb()) {
		gamestate = GameState::GAMEOVER;
		gameResult = false;
	}
	//condition: the blank is opened
	else if (board[row][column].get_isOpen()) {
		return;
	}
	//condition: others
	else if (board[row][column].get_adjcent() > 0) {
		board[row][column].set_Opened();
		openBlankCount++;
		return;
	}
	else if (board[row][column].get_adjcent() == 0) {
		board[row][column].set_Opened();
		openBlankCount++;
		//open(leftclick) 8 blanks around it
		for (int i = row - 1; i <= row + 1; i++) {
			for (int j = column - 1; j <= column + 1; j++) {
				//skip itself
				if (i == row && j == column) {
					continue;
				}
				//check position
				if (!checkPosition(i, j)) {
					continue;
				}

				leftClick(i, j);
			}
		}
	}
}

//change the blank in closed, flag and questionmark
void rightClick(int row, int column)
{
	if (board[row][column].get_isClosed()) {
		board[row][column].set_Flaged();
		flagCount++;
	}
	else if (board[row][column].get_isFlaged()) {
		board[row][column].set_QuestionMark();
		flagCount--;
	}
	else if (board[row][column].get_isQuestionMarked()) {
		board[row][column].set_Closed();
	}
}

//detect if the position is out of range
bool checkPosition(int row, int column) { return (row >= 0 && row < rowSize&& column >= 0 && column < columnSize); }

void GUIcommand(string command) {
	vector<string> userInput = split(command);

	if (userInput.size() == 0) {
		return;
	}

	if (userInput[0] == "Load") {

		//command load is only available when standby
		if (gamestate != GameState::STANDBY) {
			cout << "<" << command << "> : Failed" << endl;
			return;
		}

		string generateMode = userInput[1]; //how to generate the board

		if (generateMode == "BoardFile") {

			//if user input wrong command
			if (userInput.size() != 3) {
				cout << "Command Error" << endl;
				return;
			}

			string boardFileName = userInput[2];

			fstream board(boardFileName);

			//test if boardFileName is valid
			if (!board.is_open()) {
				cout << "<" << command << "> : Failed" << endl;
				return;
			}

			board.close();

			//load file
			mapReset();
			loadBoardFile(boardFileName);

			boardLoad = true;

			cout << "<" << command << "> : Success" << endl;
		}
		else if (generateMode == "RandomCount") {
			//if user input wrong command
			if (userInput.size() != 5) {
				cout << "Command Error" << endl;
				return;
			}

			int row = stoi(userInput[2]);
			int column = stoi(userInput[3]);
			int bomb = stoi(userInput[4]);

			if (bomb >= row * column) {
				cout << "<" << command << "> : Failed" << endl;
				return;
			}

			//load file
			mapReset();
			loadRandomCount(row, column, bomb);

			boardLoad = true;

			cout << "<" << command << "> : Success" << endl;
		}
		else if (generateMode == "RandomRate") {
			//if user input wrong command
			if (userInput.size() != 5) {
				cout << "Command Error" << endl;
				return;
			}

			int row = stoi(userInput[2]);
			int column = stoi(userInput[3]);
			double rate = stod(userInput[4]);

			if (!(rate >= 0 && rate < 1)) {
				cout << "<" << command << "> : Failed" << endl;
				return;
			}

			//load file
			mapReset();
			loadRandomRate(row, column, rate);

			boardLoad = true;

			cout << "<" << command << "> : Success" << endl;
		}
		else {
			cout << "Command Error" << endl;
		}
	}
	else if (userInput[0] == "StartGame") {

		//if user input wrong command
		if (userInput.size() != 1) {
			cout << "Command Error" << endl;
			return;
		}

		//command StartGame is only available when standby and board is loaded
		if (gamestate != GameState::STANDBY || !boardLoad) {
			cout << "<" << command << "> : Failed" << endl;
			return;
		}

		gamestate = GameState::PLAYING;

		//set all global variable
		flagCount = 0;
		openBlankCount = 0;
		remainBlankCount = rowSize * columnSize;

		cout << "<" << command << "> : Success" << endl;
	}
	else if (userInput[0] == "Print") {

		//if user input wrong command
		if (userInput.size() != 2) {
			cout << "Command Error" << endl;
			return;
		}

		string target = userInput[1]; //what user wants to print

		if (target == "GameBoard") {
			cout << "<" << command << "> : " << endl;
			coutGameBoard();
		}
		else if (target == "GameAnswer") {
			cout << "<" << command << "> : " << endl;
			coutGameAnswer();
		}
		else if (target == "GameState") {
			cout << "<" << command << "> : ";
			coutGameState();
		}
		else if (target == "BombCount") {
			cout << "<" << command << "> : ";
			coutBombCount();
		}
		else if (target == "FlagCount") {
			cout << "<" << command << "> : ";
			coutFlagCount();
		}
		else if (target == "OpenBlankCount") {
			cout << "<" << command << "> : ";
			coutOpenBlankCount();
		}
		else if (target == "RemainBlankCount") {
			cout << "<" << command << "> : ";
			coutRemainBlankCount();
		}
		else {
			cout << "Command Error" << endl;
			return;
		}
		return;
	}
	else if (userInput[0] == "LeftClick") {

		//if user input wrong command
		if (userInput.size() != 3) {
			cout << "Command Error" << endl;
			return;
		}

		//command LeftClick is only available when playing
		if (gamestate != GameState::PLAYING) {
			cout << "<" << command << "> : Failed" << endl;
			return;
		}

		int row = stoi(userInput[1]);
		int column = stoi(userInput[2]);

		if (!checkPosition(row, column) || //if position is invalid
			board[row][column].get_isOpen() || //if block is opened
			board[row][column].get_isFlaged()) { //if block is Flaged

			cout << "<" << command << "> : Failed" << endl;
			return;
		}

		cout << "<" << command << "> : Success" << endl;

		leftClick(row, column);

	}
	else if (userInput[0] == "RightClick") {

		//if user input wrong command
		if (userInput.size() != 3) {
			cout << "Command Error" << endl;
			return;
		}

		//command RightClick is only available when playing
		if (gamestate != GameState::PLAYING) {
			cout << "<" << command << "> : Failed" << endl;
			return;
		}

		int row = stoi(userInput[1]);
		int column = stoi(userInput[2]);

		if (!checkPosition(row, column) || //if position is invalid
			board[row][column].get_isOpen()) { //if block is opened

			cout << "<" << command << "> : Failed" << endl;
			return;
		}

		cout << "<" << command << "> : Success" << endl;

		rightClick(row, column);

	}
	else if (userInput[0] == "Replay") {

		//if user input wrong command
		if (userInput.size() != 1) {
			cout << "Command Error" << endl;
			return;
		}

		//command Replay is only available when GameOver
		if (gamestate != GameState::GAMEOVER) {
			cout << "<" << command << "> : Failed" << endl;
			return;
		}


		//reset all global variables
		rowSize = 0;
		columnSize = 0;
		bombCount = 0;
		flagCount = 0;
		openBlankCount = 0;
		remainBlankCount = 0;
		boardLoad = false;
		gamestate = GameState::STANDBY;

		cout << "<" << command << "> : Success" << endl;
	}
	else if (userInput[0] == "Quit") {

		//if user input wrong command
		if (userInput.size() != 1) {
			cout << "Command Error" << endl;
			return;
		}

		//command Quit is only available when GameOver
		if (gamestate != GameState::GAMEOVER) {
			cout << "<" << command << "> : Failed" << endl;
			return;
		}

		cout << "<" << command << "> : Success" << endl;

		return;

	}
	else {
		cout << "Command Error" << endl;
		return;
	}

	//update remainBlankCount
	remainBlankCount = (rowSize * columnSize) - openBlankCount;

	//check if game is over
	if (gamestate == GameState::PLAYING && remainBlankCount == bombCount) {
		gamestate = GameState::GAMEOVER;
		gameResult = true;
	}
	if (gamestate == GameState::GAMEOVER && gameResult) {
		cout << "You win the game" << endl;
	}
	if (gamestate == GameState::GAMEOVER && !gameResult) {
		cout << "You loss the game" << endl;
	}
}

/**
 * .
 * \intent: split the command with space
 * \pre: a string of command
 * \post: a vector with commands
 */
const vector<string> split(const string& str) {
	vector<string> result;
	stringstream ss(str);
	string tok;

	while (getline(ss, tok, ' ')) {
		result.push_back(tok);
	}
	return result;
}