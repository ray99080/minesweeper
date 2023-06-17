/**************************************************************
 * file         Block.cpp
 * create date  April 12 2023
 * author       B11132032
 * date         April 12 2023
 * description
 * define the fumctions of class "Block"
 **************************************************************/

#include "block.h"
#include "function.h"
#include "variables.h"
#include "enum.h"

 // constructors
Block::Block()
{
    state = CLOSED;
    isBomb = false;
    adjcent = 0;
}

// return private variables
bool Block::get_isClosed()
{
    return (this->state == CLOSED);
}

// return private variables
bool Block::get_isOpen()
{
    return (this->state == OPENED);
}

// return private variables
bool Block::get_isFlaged()
{
    return (this->state == FLAGED);
}

// return private variables
bool Block::get_isQuestionMarked()
{
    return (this->state == QUESTIONMARK);
}

// return private variables
bool Block::get_isBomb() { return this->isBomb; }

// return private variables
int Block::get_adjcent() { return this->adjcent; }

// access private variables
void Block::setBomb()
{
    this->isBomb = true;
}

// access private variables
void Block::set_Closed()
{
    this->state = CLOSED;
}

// access private variables
void Block::set_Opened()
{
    this->state = OPENED;
}

// access private variables
void Block::set_Flaged()
{
    this->state = FLAGED;
}

// access private variables
void Block::set_QuestionMark()
{
    this->state = QUESTIONMARK;
}

// access private variables
void Block::set_adjcent(int number)
{
    this->adjcent = number;
}

// reset this Block to default condition
void Block::reset()
{
    this->state = CLOSED;
    this->isBomb = false;
    this->adjcent = 0;
}