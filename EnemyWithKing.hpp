#ifndef ENEMYWITHKING
#define ENEMYWITHKING

#include "IMoveListGenerator.hpp"
#include <iostream>
#include <vector>

class EnemyWithKing : public IMoveListGenerator
{
    public:
        static const char moveOrder[7];
        static const char eatOrder[7];
        static const char mixOrder[14];
        static std::vector<std::pair<char,char>> moveVector[14];

        void genMoveList(Board* board) const;
        
};

#endif //ENEMYWITHKING