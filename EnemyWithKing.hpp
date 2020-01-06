#ifndef ENEMYWITHKING
#define ENEMYWITHKING

#include "MidGame.hpp"
#include <iostream>
#include <vector>

class EnemyWithKing : public MidGame
{
    public:
        static const char moveOrder[7];
        static const char eatOrder[7];
        static const char mixOrder[14];
        static std::vector<std::pair<char,char>> moveVector[14]; 
        static const int PIECE_SCORE[18];
        int getScore(Board* board) const;
};

#endif //ENEMYWITHKING