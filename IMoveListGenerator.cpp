#include "IMoveListGenerator.hpp"
#include <stack>
#include "Board.hpp"
#include "Piece.hpp"
#include "RuleTable.hpp"

void IMoveListGenerator::genPossibleMoveAndEat(Board* board, std::vector<std::pair<char, char>>* moveVector, Piece* piece){
    //假設piece為0~31之間的"真實子"
    
    int p = piece->piece;
    int pos = piece->position;
    std::stack<std::pair<char,char>> moveStack;
    // Legal Eat and Move
    if((p&7) == UC) //炮 包
    {
        // Pao Eat
        for(int i = 0 ; i < RuleTable::JUMP_NUM[pos] ; ++i){
            bool cross = false;
            int targetPos = pos + RuleTable::JUMP_DIR[pos][i];
            while(RuleTable::isInside(targetPos)){
                if(!cross && board->board[targetPos]->piece != EMPTY)
                {
                    cross = true;
                    targetPos += RuleTable::JUMP_DIR[pos][i];
                }
                else if(board->board[targetPos]->piece == EMPTY)
                {
                    targetPos += RuleTable::JUMP_DIR[pos][i];
                }
                else if(board->board[targetPos]->dark || (board->board[targetPos]->piece>>3)==(p>>3))
                {
                    break;
                }
                else
                {
                    moveVector->emplace_back(std::make_pair(pos,targetPos));
                    break;
                }
            }
        }
    }

    for(int i = 0 ; i < RuleTable::MOVE_NUM[pos] ; ++i){
        Piece* targetPiece = board->board[pos + RuleTable::MOVE_DIR[pos][i]];
        if(RuleTable::LEGAL_EAT_ARRAY[p][targetPiece->piece])
        {
            moveVector->emplace_back(std::make_pair(pos, targetPiece->position)); // push eat first
        }
        else if(targetPiece->piece == EMPTY)
        {
            moveStack.push(std::make_pair(pos, targetPiece->position));
        }
    }
    
    // push move later
    while(!moveStack.empty()){
        moveVector->emplace_back(moveStack.top());
        moveStack.pop();
    }
};

void IMoveListGenerator::genPossibleMove(Board* board, std::vector<std::pair<char, char>>* moveVector, Piece* piece){
    //假設piece為0~31之間的"真實子"
    
    int p = piece->piece;
    int pos = piece->position;

    for(int i = 0 ; i < RuleTable::MOVE_NUM[pos] ; ++i){
        Piece* targetPiece = board->board[pos + RuleTable::MOVE_DIR[pos][i]];
        if(targetPiece->piece == EMPTY)
        {
            moveVector->emplace_back(std::make_pair(pos, targetPiece->position));
        }
    }
};

void IMoveListGenerator::genPossibleEat(Board* board, std::vector<std::pair<char, char>>* moveVector, Piece* piece){
    
    int p = piece->piece;
    int pos = piece->position;
    // Legal Eat
    if((p&7) == UC) //炮 包
    {
        // Pao Eat
        for(int i = 0 ; i < RuleTable::JUMP_NUM[pos] ; ++i){
            bool cross = false;
            int targetPos = pos + RuleTable::JUMP_DIR[pos][i];
            while(RuleTable::isInside(targetPos)){
                if(!cross && board->board[targetPos]->piece != EMPTY)
                {
                    cross = true;
                    targetPos += RuleTable::JUMP_DIR[pos][i];
                }
                else if(board->board[targetPos]->piece == EMPTY)
                {
                    targetPos += RuleTable::JUMP_DIR[pos][i];
                }
                else if(board->board[targetPos]->dark || (board->board[targetPos]->piece>>3)==(p>>3))
                {
                    break;
                }
                else
                {
                    moveVector->emplace_back(std::make_pair(pos,targetPos));
                    break;
                }
            }
        }
    }
    else
    {
        for(int i = 0 ; i < RuleTable::MOVE_NUM[pos] ; ++i){
            Piece* targetPiece = board->board[pos + RuleTable::MOVE_DIR[pos][i]];
            if(RuleTable::LEGAL_EAT_ARRAY[p][targetPiece->piece])
            {
                moveVector->emplace_back(std::make_pair(pos, targetPiece->position)); // push eat first
            }
        }
    }
};

void IMoveListGenerator::genPossibleFlip(Board* board, std::vector<std::pair<char, char>>* moveVector){
    for(int i = 11 ; i < 49 ; ++i){
        if(board->board[i]->dark){
            moveVector->emplace_back(std::make_pair(i,i));
        }
    }
};