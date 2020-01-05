#ifndef ENEMYWITHKING
#define ENEMYWITHKING

#include "IMoveListGenerator.hpp"

class EnemyWithKing : public IMoveListGenerator
{
    public:
        void genMoveList(Board* board) const;
};

#endif //ENEMYWITHKING