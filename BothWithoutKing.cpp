#include "BothWithoutKing.hpp"
#include "Board.hpp"

const char BothWithoutKing::moveOrder[7] = {UG, UC, UM, UR, UN, UP, UK}; 
const char BothWithoutKing::eatOrder[7] = {UG, UC, UM, UR, UN, UP, UK}; 
const char BothWithoutKing::mixOrder[14] = {1,1,0,0,1,1,1,0,0,0,1,1,0,0}; // 0 = move | 1 = eat
std::vector<std::pair<char,char>> BothWithoutKing::moveVector[14];
//兵沒用 / 士大好 / 炮不吃可能被跑掉
const int BothWithoutKing::PIECE_SCORE[18] = {0,70,15,5,2,19,1,0,0,70,15,5,2,19,1,0,0,0};

/* 
兵沒用 / 士大好 

敵人         玩家
帥  0        將  0
士  70       士  70
像  15       像  15
俥  5        俥  5
馬  2        馬  2
炮  19       炮  19
兵  1        兵  1
*/
 

int BothWithoutKing::getScore(Board* board) const{

    int score = 0;
    for(int i = 0 ; i < board->numPiecesInList[board->ply] ; ++i){
        score += PIECE_SCORE[board->pieceList[board->ply][i]->piece];   
    }
    
    for(int i = 0 ; i < board->numPiecesInList[!board->ply] ; ++i){
        score -= PIECE_SCORE[board->pieceList[!board->ply][i]->piece];
    }
    return score;
};
void BothWithoutKing::handle(Board* board)const{

};