/**************************************************************
 * file         Block.h
 * create date  April 11 2023
 * author       B11132032
 * date         April 12 2023
 * description
 * define the class "Block" for each element of the map
 **************************************************************/

#pragma once

#include"enum.h"

 // define class block
class Block
{
private:
    blockstate state;
    bool isBomb;
    // store how many bomb(s) near by
    int adjcent;

public:
    // constructors
    Block();

    // return private variables
    bool get_isClosed();
    bool get_isOpen();
    bool get_isFlaged();
    bool get_isQuestionMarked();
    bool get_isBomb();
    int get_adjcent();

    // access private variables
    void setBomb();
    void set_Closed();
    void set_Opened();
    void set_Flaged();
    void set_QuestionMark();
    void set_adjcent(int number);

    // reset this Block to default condition
    void reset();
};