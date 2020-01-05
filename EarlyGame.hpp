#ifndef EARLYGAME
#define EARLYGAME

#include "IMoveListGenerator.hpp"

class EarlyGame : public IMoveListGenerator
{
    public:
        static const int flipPriority[32];
        void genMoveList(Board* board) const;
};

#endif //EARLYGAME