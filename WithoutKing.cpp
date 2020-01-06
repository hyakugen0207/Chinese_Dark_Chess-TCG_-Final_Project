#include "WithoutKing.hpp"
#include "Board.hpp"

const char WithoutKing::moveOrder[7] = {UP, UC, UG, UM, UR, UN, UK}; 
const char WithoutKing::eatOrder[7] = {UP, UC, UG, UM, UR, UN, UK}; 
const char WithoutKing::mixOrder[14] = {1,1,0,0,1,0,1,1,1,0,0,0,1,0}; // 0 = move | 1 = eat
std::vector<std::pair<char,char>> WithoutKing::moveVector[14];
//兵非常重要 / 炮非常重要 / 士重要
const int WithoutKing::PIECE_SCORE[18] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int WithoutKing::getScore(Board* board) const{
    return 0;
};