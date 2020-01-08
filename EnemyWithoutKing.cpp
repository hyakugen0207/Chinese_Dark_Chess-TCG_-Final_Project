#include "EnemyWithoutKing.hpp"
#include "Board.hpp"
#include "BothWithoutKing.hpp"

const char EnemyWithoutKing::moveOrder[7] = {UK, UG, UC, UM, UR, UN, UP}; 
const char EnemyWithoutKing::eatOrder[7] = {UG, UK, UC, UM, UR, UN, UP}; 
const char EnemyWithoutKing::mixOrder[14] = {1,0,1,0,1,0,1,1,1,1,0,0,0,0}; // 0 = move | 1 = eat
std::vector<std::pair<char,char>> EnemyWithoutKing::moveVector[14];
//兵基本上廢掉 / 士變超重要 / 帥逃跑重要 
const int EnemyWithoutKing::PIECE_SCORE[18] = {290,162,54,18,6,54,1,0,0,163,55,19,7,55,57,0,0,0};

/* 
總分883  對面兵只比士不重要  / 且都願意同等級子互換

玩家        敵人
帥  290      將  0
士  162      士  163
像  54       像  55
俥  18       俥  19
馬  6        馬  7
炮  54       炮  55
兵  1        兵  57
*/
 

int EnemyWithoutKing::getScore(Board* board) const{

    int score = 0;
    if(board->ply) // 如果我是黑子
    {
        for(int i = 0 ; i < board->numPiecesInList[board->ply] ; ++i){
            score += PIECE_SCORE[board->pieceList[board->ply][i]->piece-8];   
        }
        
        for(int i = 0 ; i < board->numPiecesInList[!board->ply] ; ++i){
            score -= PIECE_SCORE[board->pieceList[!board->ply][i]->piece+8];
        }
    }
    else //如果我是紅子
    {
        for(int i = 0 ; i < board->numPiecesInList[board->ply] ; ++i){
            score += PIECE_SCORE[board->pieceList[board->ply][i]->piece];   
        }
        
        for(int i = 0 ; i < board->numPiecesInList[!board->ply] ; ++i){
            score -= PIECE_SCORE[board->pieceList[!board->ply][i]->piece];
        }
    }
    return score;
};

void EnemyWithoutKing::handle(Board* board)const{

    bool withKing = board->alivePieces[board->ply][0];
    if(withKing)
    {
        // Do Nothing
    }
    else
    {
        board->moveListGenerator = new BothWithoutKing();
        std::cout << "change state from EnemyWithoutKing to BothWithoutKing" << std::endl;
        delete this;
    }
};