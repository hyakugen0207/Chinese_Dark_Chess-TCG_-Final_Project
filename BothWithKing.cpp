#include "BothWithKing.hpp"
#include "Board.hpp"
#include "EnemyWithoutKing.hpp"
#include "WithoutKing.hpp"


const char BothWithKing::moveOrder[7] = {UG, UK, UC, UP, UM, UR, UN}; 
const char BothWithKing::eatOrder[7] = {UP, UC, UG, UK, UM, UR, UN}; 
const char BothWithKing::mixOrder[14] = {1,1,1,0,1,0,1,1,1,0,0,0,0,0}; // 0 = move | 1 = eat
std::vector<std::pair<char,char>> BothWithKing::moveVector[14];
//兵吃人基本上不會虧 / 炮不吃可能被跑掉 / 帥吃將或士用rule解決
const int BothWithKing::PIECE_SCORE[18] = {80,48,15,5,2,29,15,0,80,48,15,5,2,29,15,0,0,0};

/* 絕對子力 此State雙方子力相同

帥  80   <45*2     允許一將換兩士  or 一將換一士兩炮 不允許 一士兩像
士  45   >44  <58  允許一士換兩炮
像  15
俥  5
馬  2
炮  29
兵  15 願意以一炮換兩兵 / 一士換三兵

*/

int BothWithKing::getScore(Board* board) const{

    int score = 0;
    for(int i = 0 ; i < board->numPiecesInList[board->ply] ; ++i){
        score += PIECE_SCORE[board->pieceList[board->ply][i]->piece];
    }

    for(int i = 0 ; i < board->numPiecesInList[!board->ply] ; ++i){
        score -= PIECE_SCORE[board->pieceList[!board->ply][i]->piece];
    }
    return score;
};

void BothWithKing::handle(Board* board)const{
    bool enemyWithKing = board->alivePieces[!board->ply][0];
    bool withKing = board->alivePieces[board->ply][0];
    if(enemyWithKing && withKing)
    {
        // Do Nothing
    }
    else
    {
        if(enemyWithKing)
        {
            board->moveListGenerator = new WithoutKing();
            std::cout << "change state from EnemyWithKing to WithoutKing" << std::endl;
        }
        else
        {
            board->moveListGenerator = new EnemyWithoutKing();
            std::cout << "change state from EnemyWithKing to EnemyWithoutKing" << std::endl;
        }
        delete this;
    }
    
};