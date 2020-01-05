#ifndef ENEMYWITHOUTKING
#define ENEMYWITHOUTKING

#include "IMoveListGenerator.hpp"

class EnemyWithoutKing : public IMoveListGenerator
{
    public:
        void genMoveList(Board* board) const;
};


#endif //ENEMYWITHOUTKING