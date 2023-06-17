#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_minesweeper.h"
#include<qpushbutton.h>
#include<array>
#include<qgridlayout.h>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include"variables.h"
#include<iostream>

class minesweeper : public QMainWindow
{
	Q_OBJECT

public:
	minesweeper(QWidget *parent = Q_NULLPTR);
	~minesweeper();
	void start();
private slots:
	void modeChange();
	void load();
	void printGameBoard();
	void printGameState();
	void printGameAnswer();
	void startGame();
	void changeBoard();
	void changeInformation();
	void checkGameover();
	void minesweeper::onLeftButtonClicked(int row, int column);
	void minesweeper::onRightButtonClicked(int row, int column);
	

private:
	Ui::minesweeperClass *ui;
	QGridLayout *layout;
	std::array<QPushButton*, BOARD_MAXIMUM_SIZE * BOARD_MAXIMUM_SIZE> buttons;
};
