#include "EnemyWithKing.hpp"
#include "Board.hpp"

const char EnemyWithKing::moveOrder[7] = {UG, UK, UC, UP, UM, UR, UN}; 
const char EnemyWithKing::eatOrder[7] = {UP, UC, UG, UK, UM, UR, UN}; 
const char EnemyWithKing::mixOrder[14] = {1,1,1,0,1,0,1,1,1,0,0,0,0,0}; // 0 = move | 1 = eat
std::vector<std::pair<char,char>> EnemyWithKing::moveVector[14];
//兵吃人基本上不會虧 / 炮不吃可能被跑掉 / 帥吃將或士用rule解決
const int EnemyWithKing::PIECE_SCORE[18] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int EnemyWithKing::getScore(Board* board) const{
    return 0;
};