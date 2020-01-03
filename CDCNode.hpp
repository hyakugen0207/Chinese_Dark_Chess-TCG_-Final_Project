#include "myDefine.hpp"
#include "Board.hpp"
#include <vec

class CDCNode{
    public:
        Board board;
        ULL hashValue;
        char move[6];
        CDCNode* parent;
        CDCNode* children[100]; //86? move 16*4 + jump 4 + flip 16
};

