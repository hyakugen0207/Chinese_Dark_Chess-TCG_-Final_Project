#include "MoveListGenerator.hpp"
#include <stack>
#include "Board.hpp"
#include "Piece.hpp"
#include "RuleTable.hpp"

#include "EarlyGame.hpp"
#include "MidGame.hpp"
#include "BothWithKing.hpp"
#include "EnemyWithoutKing.hpp"
#include "BothWithoutKing.hpp"
#include "WithoutKing.hpp"

void MoveListGenerator::genPossibleMoveAndEat(Board* board, std::vector<std::pair<char, char>>* moveVector, Piece* piece){
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

void MoveListGenerator::genPossibleMove(Board* board, std::vector<std::pair<char, char>>* moveVector, Piece* piece){
    //假設piece為0~31之間的"真實子"
    
    int p = piece->piece;
    int pos = piece->position;

    for(int i = 0 ; i < RuleTable::MOVE_NUM[pos] ; ++i){
        Piece* targetPiece = board->board[pos + RuleTable::MOVE_DIR[pos][i]];
        if(targetPiece->piece == EMPTY)
        {
            moveVector->emplace_back(std::make_pair(pos, pos + RuleTable::MOVE_DIR[pos][i]));
        }
    }
};

void MoveListGenerator::genPossibleEat(Board* board, std::vector<std::pair<char, char>>* moveVector, Piece* piece){
    
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
                moveVector->emplace_back(std::make_pair(pos, pos + RuleTable::MOVE_DIR[pos][i])); // push eat first
            }
        }
    }
};

void MoveListGenerator::genPossibleFlipPosition(Board* board, std::vector<std::pair<char, char>>* moveVector){
    
    if(board->darkPieceNumAll==0)return;
    //我有炮
    char possibleEatPos = 0;
    char possibleEatValue = 0;
    if(board->darkPieceNum[board->ply][UC] > 0)
    {
        for(int i = 0 ; i < board->numPiecesInList[!board->ply] ; ++i){
            if(((board->pieceList[!board->ply][i]->piece)&7)==UC) continue;
            int score = RuleTable::getPieceScore(board->pieceList[!board->ply][i]->piece);
            if(possibleEatValue > score)continue;
            int pos = board->pieceList[!board->ply][i]->position;
            for(int j = 0 ; j < RuleTable::JUMP_NUM[pos] ; ++j){
                int targetPos = pos + RuleTable::JUMP_DIR[pos][j];
                bool cross = false;
                while(RuleTable::isInside(targetPos)){
                    if(!cross && board->board[targetPos]->piece != EMPTY){
                        cross = true;
                        targetPos += RuleTable::JUMP_DIR[pos][j];
                    }
                    else if(board->board[targetPos]->piece == EMPTY)
                    {
                        targetPos += RuleTable::JUMP_DIR[pos][j];
                    }
                    else if(board->board[targetPos]->dark)
                    {
                        bool canFlip = true;
                        for(int k = 0 ; k < RuleTable::MOVE_NUM[targetPos] ; ++k){
                            int checkPos = targetPos + RuleTable::MOVE_DIR[targetPos][k];
                            int checkPiece = board->board[checkPos]->piece;
                            if(RuleTable::isInside(checkPos) && !board->board[checkPos]->dark &&(checkPiece != EMPTY) && ((checkPiece>>3)!=board->ply))
                            {
                                //是敵人的子 且 可以吃炮
                                if((checkPiece&7)!=UP && (checkPiece&7)!=UC)
                                {
                                    canFlip = false;
                                    break;
                                }
                            }
                        }
                        if(!canFlip) break;
                        if(possibleEatValue < score)
                        {
                            possibleEatPos = targetPos;
                            possibleEatValue = score;
                        }
                        break;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
    
    if(possibleEatPos != 0)
    {
        moveVector->emplace_back(std::make_pair(possibleEatPos,possibleEatPos));
        return;
    }

    //對面沒將 我有士像 翻炮卒馬車周圍的子 先吃 炮 > 卒 > 馬 > 車 的
    //目前可能翻給炮吃
    if((board->darkPieceNum[board->ply][UG] != 0) || (board->darkPieceNum[board->ply][UM] != 0))
    {
        char possibleEatPos = 0;
        char possibleEatValue = 0;
        for(int i = 0 ; i < board->numPiecesInList[!board->ply] ; ++i){
            int score = RuleTable::getPieceScore(board->pieceList[!board->ply][i]->piece);
            if(possibleEatValue > score) continue;
            int piece = board->pieceList[!board->ply][i]->piece;
            int pos = board->pieceList[!board->ply][i]->position;
            
            if((piece&7) >= 3)
            {
                if(((piece&7) == 6) && (board->darkPieceNum[board->ply][UK] != 0) )continue;
                for(int j = 0 ; j < RuleTable::MOVE_NUM[pos] ; ++j){
                    int targetPos = pos + RuleTable::MOVE_DIR[pos][j];
                    if(RuleTable::isInside(targetPos) && board->board[targetPos]->dark)
                    {
                        bool canFlip = true;
                        for(int k = 0 ; k < RuleTable::MOVE_NUM[targetPos] ; ++k){
                            int checkPos = targetPos + RuleTable::MOVE_DIR[targetPos][k];
                            int checkPiece = board->board[checkPos]->piece;
                            if(RuleTable::isInside(checkPos) && !(board->board[checkPos]->dark) && (checkPiece!=EMPTY) && ((checkPiece>>3)!=board->ply))
                            {
                                //是敵人
                                if((checkPiece&7)<3)
                                {
                                    canFlip = false;
                                    break;
                                }
                            }
                        }

                        if(canFlip)
                        {
                            possibleEatPos = targetPos;
                            possibleEatValue = score;
                            break;
                        }
                    }
                }
            }
        }

        if(possibleEatPos!=0)
        {
            moveVector->emplace_back(std::make_pair(possibleEatPos,possibleEatPos));
            return;
        }
    }

    //安全位置  暫時隨便翻
    for(int i = 11 ; i < 49 ; ++i){
        if(board->board[i]->dark){
            moveVector->emplace_back(std::make_pair(i,i));
            return;
        }
    }

};

MoveListGenerator::~MoveListGenerator(){

};