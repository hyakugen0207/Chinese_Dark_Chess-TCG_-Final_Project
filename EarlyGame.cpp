#include "EarlyGame.hpp"
#include "Board.hpp"
#include "RuleTable.hpp"
#include <iostream>
#include <vector>
// early game : 不需要深度思考 用rule決定幹嘛 在此狀況不長樹
    /* 
        何時為early game : 
        1. alivePieceNum = >=15 / >=15
        2. myPieceList <= 5
    */
#include "MidGame.hpp"
#include "BothWithKing.hpp"
#include "EnemyWithoutKing.hpp"
#include "BothWithoutKing.hpp"
#include "WithoutKing.hpp"


const char EarlyGame::eatOrder[7] = {UC, UG, UK, UM, UR, UN, UP}; 

const int EarlyGame::flipPriority[32] = 
{12,17,42,47,21,31,28,38,
 13,14,15,16,43,44,45,46,
 22,23,24,25,26,27,32,33,
 34,35,36,37,11,18,41,48};

void EarlyGame::genMoveList(Board* board) const{

    // vector第一個放<0,0> 確認用
    board->moveList.clear();
    board->moveList.emplace_back(std::make_pair(0,0));
    if(board->numPiecesInList[0]==0 && board->numPiecesInList[1]==0) //first move
    {
        board->moveList.emplace_back(std::make_pair(12,12)); //just flip
    }
    else
    {
        if(board->numPiecesInList[!board->ply]==0) // enemy has no piece
        {   
            char p = 17;
            std::pair<char,char> move;
            // 炮 or 仕 try to eat
            for(int i = 0 ; i < board->numPiecesInList[board->ply]; ++i){
                Piece* myPiece = board->pieceList[board->ply][i];
                if(((myPiece->piece)&7) == 5) //炮 包
                { 
                    //翻一個包可以吃的地方
                    int jumpNum = RuleTable::JUMP_NUM[myPiece->position];
                    for(int j = 0 ; j < jumpNum ; ++j){ 
                        bool cross = false;
                        int pos = myPiece->position+RuleTable::JUMP_DIR[myPiece->position][j];
                        while(board->board[pos]->inside)
                        {
                            if(!cross && board->board[pos]->piece!=17) 
                            {
                                cross = true;
                            }
                            else if(cross && board->board[pos]->dark) // 超過中間子了 且這顆是暗的
                            {
                                p = 5;
                                move.first = move.second = pos;
                                break;
                            }
                            else if(cross && board->board[pos]->piece != 17) //超過中間子了 且這顆不是空的
                            {
                                break;
                            }
                            pos += RuleTable::JUMP_DIR[myPiece->position][j];
                        }
                    }
                }
                else if(p!=5 && (((myPiece->piece)&7) == 1)) //仕 士
                { 
                    //翻一個仕可以吃的地方
                    int moveNum = RuleTable::MOVE_NUM[myPiece->position];
                    for(int j = 0 ; j < moveNum ; ++j){
                        int pos = myPiece->position+RuleTable::MOVE_DIR[myPiece->position][j];
                        if(board->board[pos]->dark)
                        {
                            p = 1;
                            move.first = move.second = pos;
                            break;
                        }
                    }
                }
            }
            
            if(p==17) //沒士沒包 翻安全的
            {
                //find save position to flip 
                for(int i = 0 ; i < 32 ; ++i){
                    int pos = flipPriority[i];
                    if(board->board[pos]->dark) // this pos is dark
                    {
                        bool canFlip = true;
                        // check whether there is any my piece near by this pos

                        //move
                        int moveNum = RuleTable::MOVE_NUM[pos];
                        for(int j = 0 ; j < moveNum && canFlip ; ++j){
                            int targetPos = pos+RuleTable::MOVE_DIR[pos][j];
                            if(!board->board[targetPos]->dark && (board->board[targetPos]->piece>>3)==board->ply)
                            {
                                canFlip = false;
                                break;
                            }
                        }

                        //jump
                        int jumpNum = RuleTable::JUMP_NUM[pos];
                        for(int j = 0 ; (j < jumpNum) && canFlip ; ++j){ 
                            bool cross = false;
                            int targetPos = pos+RuleTable::JUMP_DIR[pos][j];
                            while(board->board[targetPos]->inside)
                            {
                                if(!cross && board->board[targetPos]->piece!=17)
                                {
                                    cross = true;
                                }
                                else if(!board->board[targetPos]->dark && board->board[targetPos]->piece != 17)
                                {   // 不是暗的 且 不是空的 -> 有棋
                                    canFlip = false;
                                    break;
                                }
                                else if(board->board[targetPos]->piece != 17)
                                {  // 不是空的 -> 是暗的
                                    break;
                                }
                                targetPos += RuleTable::JUMP_DIR[pos][j];
                            }
                        }

                        if(canFlip) // if no -> flip
                        {
                            board->moveList.emplace_back(std::make_pair(pos,pos));
                            return;
                        }
                    }
                }

                //if no save flip
                for(int i = 0 ; i < 32 ; ++i){
                    int pos = flipPriority[i];
                    if(board->board[pos]->dark) // this pos is dark
                    {
                        board->moveList.emplace_back(std::make_pair(pos,pos));
                        return;
                    }
                }
            }
            else
            {
                board->moveList.emplace_back(std::make_pair(move.first,move.second));
                return;
            }
            
        }
        else //try to kill enemy's piece by flip or eat 
        {
            std::pair<char,char> move;
            bool getFlip = false;
            bool getEat = false;
            

            //eat 
            if(genEatList(board)) return;

            // 包 flip
            for(int i = 0 ; i < board->numPiecesInList[!board->ply] && !getFlip ; ++i){
                // 2020.01.05 先隨便找一隻翻包的位置 之後可以改
                Piece* hisPiece = board->pieceList[!board->ply][i];

                if((hisPiece->piece&7) != 5)
                {
                    //翻一個包可以吃的地方
                    int jumpNum = RuleTable::JUMP_NUM[hisPiece->position];
                    for(int j = 0 ; j < jumpNum && !getFlip ; ++j){ 
                        bool cross = false;
                        int pos = hisPiece->position+RuleTable::JUMP_DIR[hisPiece->position][j];
                        while(board->board[pos]->inside)
                        {
                            if(!cross && board->board[pos]->piece!=17)
                            {
                                cross = true;
                            }
                            else if(board->board[pos]->dark) //是暗的
                            {
                                getFlip = true;
                                move.first = move.second = pos;
                                break;
                            }
                            else if(board->board[pos]->piece != 17) //不是暗的 且 不是空的
                            {
                                break;
                            }
                            
                            pos += RuleTable::JUMP_DIR[hisPiece->position][j];
                        }
                    }
                }   
            }
            if(getFlip)
            {
                board->moveList.emplace_back(std::make_pair(move.first,move.second));
                return;
            }
            //no eat and killer flip 隨便翻一個
            for(int i = 0 ; i < 32 ; ++i){
                int pos = flipPriority[i];
                if(board->board[pos]->dark) // this pos is dark
                {
                    board->moveList.emplace_back(std::make_pair(pos,pos));
                    return;
                }
            }
        }        
    }    
};

int EarlyGame::getScore(Board* board) const{
    //不需要
    return 0;
};

void EarlyGame::handle(Board* board)const{
    //std::cout << board->numAlivePieces[board->ply] << "," << board->numPiecesInList[board->ply] << std::endl;
    if(board->numAlivePieces[board->ply] < 14 || board->numPiecesInList[board->ply] > 7)
    {
        bool enemyWithKing = board->alivePieces[!board->ply][0];
        bool withKing = board->alivePieces[board->ply][0];
        if(enemyWithKing)
        {
            if(withKing)
            {
                board->moveListGenerator = new BothWithKing();
                std::cerr << "change state from EarlyGame to BothWithKing" << std::endl;
            }
            else
            {
                board->moveListGenerator = new WithoutKing();
                std::cerr << "change state from EarlyGame to WithoutKing" << std::endl;
            }
        }
        else
        {
            if(withKing)
            {
                board->moveListGenerator = new EnemyWithoutKing();
                std::cerr << "change state from EarlyGame to EnemyWithoutKing" << std::endl;
            }
            else
            {
                board->moveListGenerator = new BothWithoutKing();
                std::cerr << "change state from EarlyGame to BothWithoutKing" << std::endl;
            }
        }
        delete this;
    }
};

MoveListGenerator* EarlyGame::duplicate()const{
    return new EarlyGame();
};

bool EarlyGame::genEatList(Board* board) const{

    std::vector<std::pair<char,char>> moveVector[7];
    for(int i = 0 ; i < 7 ; ++i) moveVector[i].clear();

    for(int i = 0 ; i < board->numPiecesInList[board->ply] ; ++i){
        Piece* currentPiece = board->pieceList[board->ply][i];
        //std::cerr << "currentPiece" << int(currentPiece->piece) << "," << int(((currentPiece->piece)&7)+7) << std::endl;
        genPossibleEat(board, &(moveVector[((currentPiece->piece)&7)]), currentPiece);
    }
    
    for(int i = 0 ; i < 7 ; ++i){
        if(!moveVector[eatOrder[i]].empty()) // pick the best
        {
            int value = 99;
            std::pair<char,char> move = std::make_pair(0,0);
            for(int j = 0 ; j < moveVector[eatOrder[i]].size() ; ++j){
                if(board->board[moveVector[eatOrder[i]][j].second]->piece < value)
                {
                    int pos = moveVector[eatOrder[i]][j].second;
                    bool canGo = true;
                    // check whether there is any my piece near by this pos

                    //eat
                    int moveNum = RuleTable::MOVE_NUM[pos];
                    for(int j = 0 ; j < moveNum && canGo ; ++j){
                        int targetPos = pos+RuleTable::MOVE_DIR[pos][j];
                        if(!board->board[targetPos]->dark && RuleTable::LEGAL_EAT_ARRAY[board->board[targetPos]->piece][board->board[moveVector[eatOrder[i]][j].first]->piece])
                        {
                            canGo = false;
                            break;
                        }
                    }

                    //jump
                    int jumpNum = RuleTable::JUMP_NUM[pos];
                    for(int j = 0 ; (j < jumpNum) && canGo ; ++j){ 
                        bool cross = false;
                        int targetPos = pos+RuleTable::JUMP_DIR[pos][j];
                        while(board->board[targetPos]->inside)
                        {
                            if(!cross && board->board[targetPos]->piece!=17)
                            {
                                cross = true;
                            }
                            else if(!board->board[targetPos]->dark && board->board[targetPos]->piece != EMPTY && ((board->board[targetPos]->piece&7)==UC) && (board->board[targetPos]->piece>>3)!=board->ply)
                            {   // 不是暗的 且 不是空的 -> 有棋
                                canGo = false;
                                break;
                            }
                            else if(board->board[targetPos]->piece != 17)
                            {  // 不是空的 -> 是暗的
                                break;
                            }
                            targetPos += RuleTable::JUMP_DIR[pos][j];
                        }
                    }

                    if(canGo) // CanEat
                    {
                        value = board->board[moveVector[eatOrder[i]][j].first]->piece;
                        move = moveVector[eatOrder[i]][j];
                    }
                    
                }
            }
            if(move.first!=0)
            {
                board->moveList.emplace_back(move);
                return true;
            }
        }
    }
    return false;
}