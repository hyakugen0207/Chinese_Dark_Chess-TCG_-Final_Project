#include "WithoutKing.hpp"
#include "Board.hpp"
#include "BothWithoutKing.hpp"

const char WithoutKing::moveOrder[7] = {UP, UC, UG, UM, UR, UN, UK}; 
const char WithoutKing::eatOrder[7] = {UP, UC, UG, UM, UR, UN, UK}; 
const char WithoutKing::mixOrder[14] = {1,1,0,0,1,0,1,1,1,0,0,0,1,0}; // 0 = move | 1 = eat
std::vector<std::pair<char,char>> WithoutKing::moveVector[14];
//兵非常重要 / 炮非常重要 / 士重要
const int WithoutKing::PIECE_SCORE[18] = {0,150,32,10,4,180,120,0,500,130,32,10,4,54,1,0,0,0};

/* 
總分  兵只比士和炮不重要 

敵人         玩家
帥  500      將  0
士  130      士  150
像  32       像  32
俥  10       俥  10
馬  4        馬  4
炮  54       炮  180
兵  1        兵  120
*/
 

int WithoutKing::getScore(Board* board) const{

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

void WithoutKing::handle(Board* board)const{
    bool enemyWithKing = board->alivePieces[!board->ply][0];
    if(enemyWithKing)
    {
        // Do Nothing  
    }
    else
    {
        board->moveListGenerator = new BothWithoutKing();
        std::cout << "change state from WithoutKing to BothWithoutKing" << std::endl;
        delete this; 
    }
};