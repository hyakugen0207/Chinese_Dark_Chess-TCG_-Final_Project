#ifndef EARLYGAME
#define EARLYGAME

#include "MoveListGenerator.hpp"

class EarlyGame : public MoveListGenerator
{
    public:
        static const char eatOrder[7];
        static const int flipPriority[32];
        void handle(Board* board)const;
        void genMoveList(Board* board) const;
        int getScore(Board* board) const;
        MoveListGenerator* duplicate()const;
        bool genEatList(Board* board) const;
};

#endif //EARLYGAME