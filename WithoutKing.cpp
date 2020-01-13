#include "WithoutKing.hpp"
#include "Board.hpp"
#include "BothWithoutKing.hpp"

const char WithoutKing::moveOrder[7] = {UP, UC, UG, UM, UR, UN, UK}; 
const char WithoutKing::eatOrder[7] = {UP, UC, UG, UM, UR, UN, UK}; 
const char WithoutKing::mixOrder[14] = {1,1,0,0,1,0,1,1,1,0,0,0,1,0}; // 0 = move | 1 = eat
std::vector<std::pair<char,char>> WithoutKing::moveVector[14];
//兵非常重要 / 炮非常重要 / 士重要

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


void WithoutKing::handle(Board* board)const{
    
    bool enemyWithKing = board->alivePieces[!board->ply][0];
    if(enemyWithKing)
    {
        // Do Nothing  
    }
    else
    {
        board->moveListGenerator = new BothWithoutKing();
        //std::cout << "change state from WithoutKing to BothWithoutKing" << std::endl;
        delete this; 
    }
    
};

MoveListGenerator* WithoutKing::duplicate()const{
    return new WithoutKing();
};