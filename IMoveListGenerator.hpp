
#ifndef IMOVELISTGENERATOR
#define IMOVELISTGENERATOR
#include <vector>
#include "Board.hpp"
#include "EarlyGame.hpp"
#include "EnemyWithKing.hpp"
#include "EnemyWithoutKing.hpp"

class IMoveListGenerator
{
    public:
        virtual void genMoveList(Board* board) const = 0;
};

#endif //IMOVELISTGENERATOR