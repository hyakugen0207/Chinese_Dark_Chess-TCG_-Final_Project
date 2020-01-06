#ifndef RANDOMMOVE
#define RANDOMMOVE

#include "IMoveListGenerator.hpp"

class RandomMove : public IMoveListGenerator
{
    public:
        void genMoveList(Board* board) const;
};

#endif //RANDOMMOVE