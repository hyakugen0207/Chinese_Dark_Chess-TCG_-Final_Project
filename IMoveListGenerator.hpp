
#ifndef IMOVELISTGENERATOR
#define IMOVELISTGENERATOR
#include <vector>

class Board;
class RuleTable;

class IMoveListGenerator
{
    public:
        virtual void genMoveList(Board* board) const = 0;
};

#include "EarlyGame.hpp"
#include "EnemyWithKing.hpp"
#include "EnemyWithoutKing.hpp"

#endif //IMOVELISTGENERATOR