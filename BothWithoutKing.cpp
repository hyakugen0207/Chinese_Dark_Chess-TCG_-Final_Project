#include "BothWithoutKing.hpp"
#include "Board.hpp"

const char BothWithoutKing::moveOrder[7] = {UG, UC, UM, UR, UN, UP, UK}; 
const char BothWithoutKing::eatOrder[7] = {UG, UC, UM, UR, UN, UP, UK}; 
const char BothWithoutKing::mixOrder[14] = {1,1,0,0,1,1,1,0,0,0,1,1,0,0}; // 0 = move | 1 = eat
std::vector<std::pair<char,char>> BothWithoutKing::moveVector[14];
//兵沒用 / 士大好 / 炮不吃可能被跑掉
const int BothWithoutKing::PIECE_SCORE[18] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int BothWithoutKing::getScore(Board* boad) const{
    return 0;
};