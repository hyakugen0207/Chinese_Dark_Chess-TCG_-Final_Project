#include "BothWithoutKing.hpp"
#include "Board.hpp"

const char BothWithoutKing::moveOrder[7] = {UG, UC, UM, UR, UN, UP, UK}; 
const char BothWithoutKing::eatOrder[7] = {UG, UC, UM, UR, UN, UP, UK}; 
const char BothWithoutKing::mixOrder[14] = {1,1,0,0,1,1,1,0,0,0,1,1,0,0}; // 0 = move | 1 = eat
std::vector<std::pair<char,char>> BothWithoutKing::moveVector[14];
//兵沒用 / 士大好 / 炮不吃可能被跑掉

/* 
兵沒用 / 士大好 

敵人         玩家
帥  0        將  0
士  70       士  70
像  15       像  15
俥  5        俥  5
馬  2        馬  2s
炮  19       炮  19
兵  1        兵  1
*/
 
void BothWithoutKing::handle(Board* board)const{

};

int BothWithoutKing::getState()const{
    return 4;
};

MoveListGenerator* BothWithoutKing::duplicate()const{
    return new BothWithoutKing();
};