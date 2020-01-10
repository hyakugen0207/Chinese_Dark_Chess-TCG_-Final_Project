#ifndef MIDGAME
#define MIDGAME

#include "MoveListGenerator.hpp"
#include <iostream>
#include <vector>

class MidGame : public MoveListGenerator
{
    public:
        virtual void handle(Board* board)const = 0;
        virtual MoveListGenerator* duplicate()const = 0;
        static const char moveOrder[7];
        static const char eatOrder[7];
        static const char mixOrder[14];
        static std::vector<std::pair<char,char>> moveVector[14];

        void genMoveList(Board* board) const;

        virtual ~MidGame();
        
};



#endif //MIDGAME