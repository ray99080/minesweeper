#include "minesweeper.h"
#include"block.h"
#include"enum.h"
#include"function.h"
#include"start.h"
#include"variables.h"
#include <QComboBox>
#include<qlabel.h>
#include<string>
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
#include <QMouseEvent>
#include <QMessageBox>
#include <qabstractbutton.h>
#include"ui_minesweeper.h"
#include<QtMultimedia/Qsound>



minesweeper::minesweeper(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::minesweeperClass) 
{
	ui->setupUi(this);

	QSound* bgm = new QSound(".\\sound\\bgm.wav");
	bgm->setLoops(-1);
	bgm->play();

	QObject::connect(ui->comboBox_mode, SIGNAL(currentTextChanged(QString)), this, SLOT(modeChange()));
	QObject::connect(ui->pushButton_load, SIGNAL(clicked()), this, SLOT(load()));
	QObject::connect(ui->pushButton_printgameboard, SIGNAL(clicked()), this, SLOT(printGameBoard()));
	QObject::connect(ui->pushButton_printgameanswer, SIGNAL(clicked()), this, SLOT(printGameAnswer()));
	QObject::connect(ui->pushButton_printgamestate, SIGNAL(clicked()), this, SLOT(printGameState()));
	QObject::connect(ui->pushButton_startgame, SIGNAL(clicked()), this, SLOT(startGame()));

	QObject::connect(ui->pushButton_printgameboard_2, SIGNAL(clicked()), this, SLOT(printGameBoard()));
	QObject::connect(ui->pushButton_printgameanswer_2, SIGNAL(clicked()), this, SLOT(printGameAnswer()));
	QObject::connect(ui->pushButton_printgamestate_2, SIGNAL(clicked()), this, SLOT(printGameState()));

}

void minesweeper::modeChange() {
	if (ui->comboBox_mode->currentText() == "BoardFile") {
		ui->label_row->hide();
		ui->label_col->hide();
		ui->spinBox_row->hide();
		ui->spinBox_col->hide();
		ui->label_count->hide();
		ui->label_rate->hide();
		ui->spinBox_count->hide();
		ui->doubleSpinBox_rate->hide();

		ui->label_filename->show();
		ui->lineEdit_filename->show();
	}
	else if (ui->comboBox_mode->currentText() == "RandomCount") {
		ui->label_row->show();
		ui->label_col->show();
		ui->spinBox_row->show();
		ui->spinBox_col->show();
		ui->label_count->show();
		ui->label_rate->hide();
		ui->spinBox_count->show();
		ui->doubleSpinBox_rate->hide();
		ui->label_filename->hide();
		ui->lineEdit_filename->hide();
	}
	else if (ui->comboBox_mode->currentText() == "RandomRate") {
		ui->label_row->show();
		ui->label_col->show();
		ui->spinBox_row->show();
		ui->spinBox_col->show();
		ui->label_count->hide();
		ui->label_rate->show();
		ui->spinBox_count->hide();
		ui->doubleSpinBox_rate->show();
		ui->label_filename->hide();
		ui->lineEdit_filename->hide();
	}
	
}

minesweeper::~minesweeper()
{
	delete ui;
}

void minesweeper::start() {
	ui->label_row->hide();
	ui->label_col->hide();
	ui->spinBox_row->hide();
	ui->spinBox_col->hide();
	ui->label_count->hide();
	ui->label_rate->hide();
	ui->spinBox_count->hide();
	ui->doubleSpinBox_rate->hide();
}

//when load is click
void minesweeper::load() {
	if (ui->comboBox_mode->currentText() == "BoardFile") {
		std::string filename = ui->lineEdit_filename->text().toLocal8Bit().constData();
		GUIcommand("Load BoardFile " + filename);
	}
	else if (ui->comboBox_mode->currentText() == "RandomCount") {
		std::string command = "";
		int row = ui->spinBox_row->value();
		int col = ui->spinBox_col->value();
		int count = ui->spinBox_count->value();

		command = std::to_string(row) + " " + std::to_string(col) + " " + std::to_string(count);
		GUIcommand("Load RandomCount " + command);
	}
	else if (ui->comboBox_mode->currentText() == "RandomRate") {
		std::string command = "";
		int row = ui->spinBox_row->value();
		int col = ui->spinBox_col->value();
		double rate = ui->doubleSpinBox_rate->value();

		command = std::to_string(row) + " " + std::to_string(col) + " " + std::to_string(rate);
		GUIcommand("Load RandomRate " + command);
	}
}

void minesweeper::printGameBoard()
{
	GUIcommand("Print GameBoard");
}

void minesweeper::printGameState()
{
	GUIcommand("Print GameState");
}

void minesweeper::printGameAnswer()
{
	GUIcommand("Print GameAnswer");
}


//page_2

void minesweeper::startGame() {

	GUIcommand("StartGame");
	if (!boardLoad) {
		return;
	}
	if (gamestate == GameState::PLAYING) {
		ui->stackedWidget->setCurrentIndex(1);
	}
	changeInformation();
	layout = new QGridLayout(ui->frame_2);
	layout->setHorizontalSpacing(0.1);
	layout->setVerticalSpacing(0.1);
	int index = 0;

	for (int i = 0; i < rowSize; i++) {
		for (int j = 0; j < columnSize; j++) {
			QPushButton* button = new QPushButton(QString(""), ui->frame_2);
			button->setFixedSize(50, 50);
			button->setContextMenuPolicy(Qt::CustomContextMenu);
			//test
			button->setMouseTracking(true);
			connect(button, &QPushButton::customContextMenuRequested, this, [=]() {
				onRightButtonClicked(i, j);
				});
			//test

			connect(button, &QPushButton::clicked, this, [=]() {
				onLeftButtonClicked(i, j);
			});


			layout->addWidget(button, i, j);
			index++;
		}
	}
}


void minesweeper::changeBoard() {
	QIcon* bomb = new QIcon(".\\image\\bomb.png");
	QIcon* flag = new QIcon(".\\image\\flag.png");
	QSound* explosion = new QSound(".\\sound\\explosion.wav");
	explosion->setLoops(1);


	for (int i = 0; i < rowSize; i++) {
		for (int j = 0; j < columnSize; j++) {
			
			QPushButton* button = qobject_cast<QPushButton*>(layout->itemAtPosition(i, j)->widget());
			if (board[i][j].get_isOpen()) {
				button->setStyleSheet("background-color: gray");
				button->setIconSize(QSize(0, 0));
				button->setText("");
				if (board[i][j].get_adjcent() > 0) {
					button->setText(QString::number(board[i][j].get_adjcent()));
				}
			}
			else if (board[i][j].get_isClosed()) {
				button->setIconSize(QSize(0, 0));
				button->setText("");
			}
			else if (board[i][j].get_isFlaged()) {
				button->setIcon(*flag);
				button->setIconSize(QSize(button->width(), button->height()));
			}
			else if (board[i][j].get_isQuestionMarked()) {
				button->setIconSize(QSize(0, 0));
				button->setText("?");
			}
			
		}
	}
	
	if (gamestate == GameState::GAMEOVER) {
		for (int i = 0; i < rowSize; i++) {
			for (int j = 0; j < columnSize; j++) {
				QPushButton* button = qobject_cast<QPushButton*>(layout->itemAtPosition(i, j)->widget());
				if (board[i][j].get_isBomb()) {
					button->setStyleSheet("background-color: red");
					button->setIcon(*bomb);
					button->setIconSize(QSize(button->width(), button->height()));
					explosion->play();
				}
				else {
					button->setStyleSheet("background-color: gray");
					button->setText("");
				}

			}
		}

	}
	
}

void minesweeper::changeInformation() {

	if (gamestate == GameState::PLAYING) {
		ui->label_game_state_2->setText("Playing");
	}
	else if (gamestate == GameState::STANDBY) {
		ui->label_game_state_2->setText("Standby");
	}
	else if (gamestate == GameState::GAMEOVER) {
		ui->label_game_state_2->setText("Gameover");
	}

	ui->label_bomb_count->setText(QString::number(bombCount));
	ui->label_flag_count->setText(QString::number(flagCount));
	ui->label_open_blank_count->setText(QString::number(openBlankCount));
	ui->label_remain_blank_count->setText(QString::number(remainBlankCount));
	
}

void minesweeper::checkGameover() {
	if (gamestate == GameState::PLAYING && remainBlankCount == bombCount) {
		gamestate = GameState::GAMEOVER;
		gameResult = true;
	}
	if (gamestate == GameState::GAMEOVER && gameResult) {
		QMessageBox win;
		win.setText("You win the game.");
		QPushButton* replay = win.addButton(tr("Replay"), QMessageBox::ActionRole);
		QPushButton* quit = win.addButton(tr("Quit"), QMessageBox::ActionRole);
		connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
		win.exec();
		if (win.clickedButton() == replay) {
			//
			for (int i = 0; i < rowSize; i++) {
				for (int j = 0; j < columnSize; j++) {
					QPushButton* button = qobject_cast<QPushButton*>(layout->itemAtPosition(i, j)->widget());
					QWidget* widget = layout->itemAtPosition(i, j)->widget();
					if (widget) {
						layout->removeWidget(widget);
						delete widget;
					}
				}
			}
			delete layout;
			//
			GUIcommand("Replay");
			ui->stackedWidget->setCurrentIndex(0);
			
		}
		else if (win.clickedButton() == quit) {
			GUIcommand("Quit");
		}
	}
	if (gamestate == GameState::GAMEOVER && !gameResult) {
		QMessageBox lose;
		lose.setText("You lose the game.");
		QPushButton* replay = lose.addButton(tr("Replay"), QMessageBox::ActionRole);
		QPushButton* quit = lose.addButton(tr("Quit"), QMessageBox::ActionRole);
		connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
		lose.exec();
		if (lose.clickedButton() == replay) {
			//
			for (int i = 0; i < rowSize; i++) {
				for (int j = 0; j < columnSize; j++) {
					QPushButton* button = qobject_cast<QPushButton*>(layout->itemAtPosition(i, j)->widget());
					QWidget* widget = layout->itemAtPosition(i, j)->widget();
					if (widget) {
						layout->removeWidget(widget);
						delete widget;
					}
				}
			}
			delete layout;
			//
			GUIcommand("Replay");
			ui->stackedWidget->setCurrentIndex(0);
			
		}
		else if (lose.clickedButton() == quit) {
			GUIcommand("Quit");
		}

	}
}


void minesweeper::onLeftButtonClicked(int row, int column)
{
	GUIcommand("LeftClick " + std::to_string(row) + " " + std::to_string(column));
	changeBoard();
	changeInformation();
	checkGameover();

}

void minesweeper::onRightButtonClicked(int row, int column) 
{
	GUIcommand("RightClick " + std::to_string(row) + " " + std::to_string(column));
	changeBoard();
	changeInformation();
	checkGameover();
}