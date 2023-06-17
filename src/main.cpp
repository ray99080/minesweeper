/*****************************************************************//**
 * \file  Source.cpp
 * \create date  April 11 2023
 * \author ±i¦wºÍ
 * \date  April 11 2023
 * \description minesweeper main funtion
 *********************************************************************/

#include<iostream>
#include"block.h"
#include"function.h"
#include"start.h"
#include<fstream>
#include<string>
#include<vector>
#include <sstream>
#include"variables.h"
#include"enum.h"
#include"minesweeper.h"

using namespace std;


//global variables
int rowSize = 0;
int columnSize = 0;
int bombCount;
int flagCount;
int openBlankCount;
int remainBlankCount;
bool boardLoad = false; //whether board is loaded
bool gameResult = true; //true for win, false for loss
GameState gamestate = GameState::STANDBY;
Block board[BOARD_MAXIMUM_SIZE][BOARD_MAXIMUM_SIZE];

void runCommandFile(string inputFileName, string outputFileName);
void runCommandInput();
void runGUI(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	srand(time(NULL));
	if (argc == 1) {
		runGUI(argc, argv);
	}
	else if (argc == 2 && string(argv[1]) == "CommandInput") {
		runCommandInput();
	}
	else if (argc == 4 && string(argv[1]) == "CommandFile") {
		runCommandFile(argv[2], argv[3]);
	}
	else {
		cout << "Command Error" << endl;
	}
}


void runCommandFile(string inputFileName, string outputFileName) {
	fstream input(inputFileName);
	fstream output(outputFileName);
	string command;

	while (getline(input, command)) {
		vector<string> userInput = split(command);

		if (userInput.size() == 0) {
			continue;
		}
		if (userInput[0] == "Load") {

			//command load is only available when standby
			if (gamestate != GameState::STANDBY) {
				output << "<" << command << "> : Failed" << endl;
				continue;
			}

			string generateMode = userInput[1]; //how to generate the board

			if (generateMode == "BoardFile") {

				//if user input wrong command
				if (userInput.size() != 3) {
					output << "Command Error" << endl;
					continue;
				}

				string boardFileName = userInput[2];

				fstream board(boardFileName);

				//test if boardFileName is valid
				if (!board.is_open()) {
					output << "<" << command << "> : Failed" << endl;
					continue;
				}

				board.close();

				//load file
				mapReset();
				loadBoardFile(boardFileName);

				boardLoad = true;

				output << "<" << command << "> : Success" << endl;
			}
			else if (generateMode == "RandomCount") {
				//if user input wrong command
				if (userInput.size() != 5) {
					output << "Command Error" << endl;
					continue;
				}

				int row = stoi(userInput[2]);
				int column = stoi(userInput[3]);
				int bomb = stoi(userInput[4]);

				if (bomb >= row * column) {
					output << "<" << command << "> : Failed" << endl;
					continue;
				}

				//load file
				mapReset();
				loadRandomCount(row, column, bomb);

				boardLoad = true;

				output << "<" << command << "> : Success" << endl;
			}
			else if (generateMode == "RandomRate") {
				//if user input wrong command
				if (userInput.size() != 5) {
					output << "Command Error" << endl;
					continue;
				}

				int row = stoi(userInput[2]);
				int column = stoi(userInput[3]);
				double rate = stod(userInput[4]);

				if (!(rate >= 0 && rate < 1)) {
					output << "<" << command << "> : Failed" << endl;
					continue;
				}

				//load file
				mapReset();
				loadRandomRate(row, column, rate);

				boardLoad = true;

				output << "<" << command << "> : Success" << endl;
			}
			else {
				output << "Command Error" << endl;
			}
		}
		else if (userInput[0] == "StartGame") {

			//if user input wrong command
			if (userInput.size() != 1) {
				output << "Command Error" << endl;
				continue;
			}

			//command StartGame is only available when standby and board is loaded
			if (gamestate != GameState::STANDBY || !boardLoad) {
				output << "<" << command << "> : Failed" << endl;
				continue;
			}

			gamestate = GameState::PLAYING;

			//set all global variable
			flagCount = 0;
			openBlankCount = 0;
			remainBlankCount = rowSize * columnSize;

			output << "<" << command << "> : Success" << endl;
		}
		else if (userInput[0] == "Print") {

			//if user input wrong command
			if (userInput.size() != 2) {
				output << "Command Error" << endl;
				continue;
			}

			string target = userInput[1]; //what user wants to print

			if (target == "GameBoard") {
				output << "<" << command << "> : " << endl;
				printGameBoard(output);
			}
			else if (target == "GameAnswer") {
				output << "<" << command << "> : " << endl;
				printGameAnswer(output);
			}
			else if (target == "GameState") {
				output << "<" << command << "> : ";
				printGameState(output);
			}
			else if (target == "BombCount") {
				output << "<" << command << "> : ";
				printBombCount(output);
			}
			else if (target == "FlagCount") {
				output << "<" << command << "> : ";
				printFlagCount(output);
			}
			else if (target == "OpenBlankCount") {
				output << "<" << command << "> : ";
				printOpenBlankCount(output);
			}
			else if (target == "RemainBlankCount") {
				output << "<" << command << "> : ";
				printRemainBlankCount(output);
			}
			else {
				output << "Command Error" << endl;
				continue;
			}
			continue;
		}
		else if (userInput[0] == "LeftClick") {

			//if user input wrong command
			if (userInput.size() != 3) {
				output << "Command Error" << endl;
				continue;
			}

			//command LeftClick is only available when playing
			if (gamestate != GameState::PLAYING) {
				output << "<" << command << "> : Failed" << endl;
				continue;
			}

			int row = stoi(userInput[1]);
			int column = stoi(userInput[2]);

			if (!checkPosition(row, column) || //if position is invalid
				board[row][column].get_isOpen() || //if block is opened
				board[row][column].get_isFlaged()) { //if block is Flaged

				output << "<" << command << "> : Failed" << endl;
				continue;
			}

			output << "<" << command << "> : Success" << endl;

			leftClick(row, column);

		}
		else if (userInput[0] == "RightClick") {

			//if user input wrong command
			if (userInput.size() != 3) {
				output << "Command Error" << endl;
				continue;
			}

			//command RightClick is only available when playing
			if (gamestate != GameState::PLAYING) {
				output << "<" << command << "> : Failed" << endl;
				continue;
			}

			int row = stoi(userInput[1]);
			int column = stoi(userInput[2]);

			if (!checkPosition(row, column) || //if position is invalid
				board[row][column].get_isOpen()) { //if block is opened

				output << "<" << command << "> : Failed" << endl;
				continue;
			}

			output << "<" << command << "> : Success" << endl;

			rightClick(row, column);

		}
		else if (userInput[0] == "Replay") {

			//if user input wrong command
			if (userInput.size() != 1) {
				output << "Command Error" << endl;
				continue;
			}

			//command Replay is only available when GameOver
			if (gamestate != GameState::GAMEOVER) {
				output << "<" << command << "> : Failed" << endl;
				continue;
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

			output << "<" << command << "> : Success" << endl;
		}
		else if (userInput[0] == "Quit") {

			//if user input wrong command
			if (userInput.size() != 1) {
				output << "Command Error" << endl;
				continue;
			}

			//command Quit is only available when GameOver
			if (gamestate != GameState::GAMEOVER) {
				output << "<" << command << "> : Failed" << endl;
				continue;
			}

			output << "<" << command << "> : Success" << endl;

			break;

		}
		else {
			output << "Command Error" << endl;
			continue;
		}

		//update remainBlankCount
		remainBlankCount = (rowSize * columnSize) - openBlankCount;

		//check if game is over
		if (gamestate == GameState::PLAYING && remainBlankCount == bombCount) {
			gamestate = GameState::GAMEOVER;
			gameResult = true;
		}
		if (gamestate == GameState::GAMEOVER && gameResult) {
			output << "You win the game" << endl;
		}
		if (gamestate == GameState::GAMEOVER && !gameResult) {
			output << "You loss the game" << endl;
		}

	}
}

void runCommandInput() {
	string command;
	while (getline(cin, command)) {
		vector<string> userInput = split(command);

		if (userInput.size() == 0) {
			continue;
		}

		if (userInput[0] == "Load") {

			//command load is only available when standby
			if (gamestate != GameState::STANDBY) {
				cout << "<" << command << "> : Failed" << endl;
				continue;
			}

			string generateMode = userInput[1]; //how to generate the board

			if (generateMode == "BoardFile") {

				//if user input wrong command
				if (userInput.size() != 3) {
					cout << "Command Error" << endl;
					continue;
				}

				string boardFileName = userInput[2];

				fstream board(boardFileName);

				//test if boardFileName is valid
				if (!board.is_open()) {
					cout << "<" << command << "> : Failed" << endl;
					continue;
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
					continue;
				}

				int row = stoi(userInput[2]);
				int column = stoi(userInput[3]);
				int bomb = stoi(userInput[4]);

				if (bomb >= row * column) {
					cout << "<" << command << "> : Failed" << endl;
					continue;
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
					continue;
				}

				int row = stoi(userInput[2]);
				int column = stoi(userInput[3]);
				double rate = stod(userInput[4]);

				if (!(rate >= 0 && rate < 1)) {
					cout << "<" << command << "> : Failed" << endl;
					continue;
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
				continue;
			}

			//command StartGame is only available when standby and board is loaded
			if (gamestate != GameState::STANDBY || !boardLoad) {
				cout << "<" << command << "> : Failed" << endl;
				continue;
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
				continue;
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
				continue;
			}
			continue;
		}
		else if (userInput[0] == "LeftClick") {

			//if user input wrong command
			if (userInput.size() != 3) {
				cout << "Command Error" << endl;
				continue;
			}

			//command LeftClick is only available when playing
			if (gamestate != GameState::PLAYING) {
				cout << "<" << command << "> : Failed" << endl;
				continue;
			}

			int row = stoi(userInput[1]);
			int column = stoi(userInput[2]);

			if (!checkPosition(row, column) || //if position is invalid
				board[row][column].get_isOpen() || //if block is opened
				board[row][column].get_isFlaged()) { //if block is Flaged

				cout << "<" << command << "> : Failed" << endl;
				continue;
			}

			cout << "<" << command << "> : Success" << endl;

			leftClick(row, column);

		}
		else if (userInput[0] == "RightClick") {

			//if user input wrong command
			if (userInput.size() != 3) {
				cout << "Command Error" << endl;
				continue;
			}

			//command RightClick is only available when playing
			if (gamestate != GameState::PLAYING) {
				cout << "<" << command << "> : Failed" << endl;
				continue;
			}

			int row = stoi(userInput[1]);
			int column = stoi(userInput[2]);

			if (!checkPosition(row, column) || //if position is invalid
				board[row][column].get_isOpen()) { //if block is opened

				cout << "<" << command << "> : Failed" << endl;
				continue;
			}

			cout << "<" << command << "> : Success" << endl;

			rightClick(row, column);

		}
		else if (userInput[0] == "Replay") {

			//if user input wrong command
			if (userInput.size() != 1) {
				cout << "Command Error" << endl;
				continue;
			}

			//command Replay is only available when GameOver
			if (gamestate != GameState::GAMEOVER) {
				cout << "<" << command << "> : Failed" << endl;
				continue;
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
				continue;
			}

			//command Quit is only available when GameOver
			if (gamestate != GameState::GAMEOVER) {
				cout << "<" << command << "> : Failed" << endl;
				continue;
			}

			cout << "<" << command << "> : Success" << endl;

			break;

		}
		else {
			cout << "Command Error" << endl;
			continue;
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
}

void runGUI(int argc, char* argv[]) {
	QApplication a(argc, argv);
	minesweeper w;
	w.show();
	w.start();
	a.exec();
}
