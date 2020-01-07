#include "EnemyWithoutKing.hpp"
#include "Board.hpp"

const char EnemyWithoutKing::moveOrder[7] = {UK, UG, UC, UM, UR, UN, UP}; 
const char EnemyWithoutKing::eatOrder[7] = {UG, UK, UC, UM, UR, UN, UP}; 
const char EnemyWithoutKing::mixOrder[14] = {1,0,1,0,1,0,1,1,1,1,0,0,0,0}; // 0 = move | 1 = eat
std::vector<std::pair<char,char>> EnemyWithoutKing::moveVector[14];
//兵基本上廢掉 / 士變超重要 / 帥逃跑重要 
const int EnemyWithoutKing::PIECE_SCORE[18] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int EnemyWithoutKing::getScore(Board* board) const{
    return 0;
};

void EnemyWithoutKing::handle(Board* board)const{

};