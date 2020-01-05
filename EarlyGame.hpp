#ifndef EARLYGAME
#define EARLYGAME

#include "IMoveListGenerator.hpp"

class EarlyGame : public IMoveListGenerator
{
    public:
        void genMoveList(Board* board) const;
};

#endif //EARLYGAME