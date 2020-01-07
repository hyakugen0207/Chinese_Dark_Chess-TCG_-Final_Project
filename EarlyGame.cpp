#include "EarlyGame.hpp"
#include "Board.hpp"
#include "RuleTable.hpp"
#include <iostream>
// early game : 不需要深度思考 用rule決定幹嘛 在此狀況不長樹
    /* 
        何時為early game : 
        1. alivePieceNum = >=15 / >=15
        2. myPieceList <= 5
    */

const int EarlyGame::flipPriority[32] = 
{12,17,42,47,21,31,28,38,
 13,14,15,16,43,44,45,46,
 22,23,24,25,26,27,32,33,
 34,35,36,37,11,18,41,48};

void EarlyGame::genMoveList(Board* board) const{

    std::cerr << "Debug : in genMoveList" << std::endl;
    // vector第一個放<0,0> 確認用
    board->moveList.clear();
    board->moveList.emplace_back(std::make_pair(0,0));
    std::cerr << "Debug : board->moveList[0] is (" << board->moveList[0].first << "," << board->moveList[0].second << ")" << std::endl;
    if(board->numPiecesInList[0]==0 && board->numPiecesInList[1]==0) //first move
    {
        std::cerr << "Debug : in genMove first move" << std::endl;
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
                std::cerr << "Debug : in no piece pao or shi" << std::endl;
                Piece* myPiece = board->pieceList[board->ply][i];
                if(((myPiece->piece)&7) == 5) //炮 包
                { 
                    std::cerr << "Debug : in no piece pao " << std::endl;
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
                    std::cerr << "Debug : end no piece pao " << std::endl;
                }
                else if(p!=5 && (((myPiece->piece)&7) == 1)) //仕 士
                { 
                    std::cerr << "Debug : in no piece shi " << std::endl;
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
                    std::cerr << "Debug : end no piece shi " << std::endl;
                }
            }
            
            if(p==17) //沒士沒包 翻安全的
            {
                std::cerr << "Debug : in no piece other " << std::endl;
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
                std::cerr << "Debug : do no piece pao or shi " << std::endl;
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


            if(getEat)
            {
                board->moveList.emplace_back(std::make_pair(move.first,move.second));
                return;
            }
            std::cerr << "Debug : pao" << std::endl;
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
                            std::cerr << "Debug : in pao pos : " << pos << std::endl;
                        }
                    }
                }   
            }
             std::cerr << "Debug : end pao" << std::endl;
            if(getFlip)
            {
                board->moveList.emplace_back(std::make_pair(move.first,move.second));
                return;
            }
             std::cerr << "Debug : cannot flip" << std::endl;
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
    return 0;
};

void EarlyGame::handle(Board* board)const{

};