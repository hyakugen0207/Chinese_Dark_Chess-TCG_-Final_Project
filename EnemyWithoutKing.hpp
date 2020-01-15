#ifndef ENEMYWITHOUTKING
#define ENEMYWITHOUTKING

#include "MidGame.hpp"

class EnemyWithoutKing : public MidGame
{
    public:
        static const char moveOrder[7];
        static const char eatOrder[7];
        static const char mixOrder[14];
        static std::vector<std::pair<char,char>> moveVector[14]; 
        void handle(Board* board)const;
        int getState()const;
        MoveListGenerator* duplicate()const;
};


#endif //ENEMYWITHOUTKING