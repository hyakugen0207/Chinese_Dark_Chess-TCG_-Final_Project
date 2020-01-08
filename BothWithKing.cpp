#include "BothWithKing.hpp"
#include "Board.hpp"
#include "EnemyWithoutKing.hpp"
#include "WithoutKing.hpp"


const char BothWithKing::moveOrder[7] = {UG, UK, UC, UP, UM, UR, UN}; 
const char BothWithKing::eatOrder[7] = {UP, UC, UG, UK, UM, UR, UN}; 
const char BothWithKing::mixOrder[14] = {1,1,1,0,1,0,1,1,1,0,0,0,0,0}; // 0 = move | 1 = eat
std::vector<std::pair<char,char>> BothWithKing::moveVector[14];
//兵吃人基本上不會虧 / 炮不吃可能被跑掉 / 帥吃將或士用rule解決
const int BothWithKing::PIECE_SCORE[18] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int BothWithKing::getScore(Board* board) const{
    return 0;
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