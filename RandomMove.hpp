#ifndef RANDOMMOVE
#define RANDOMMOVE

#include "MoveListGenerator.hpp"

class RandomMove : public MoveListGenerator
{
    public:
        void genMoveList(Board* board) const;
};

#endif //RANDOMMOVE