#include "myDefine.hpp"
#include "Board.hpp"

class CDCNode{
    public:
        Board* board;
        ULL hashValue;
        char hashState;
        bool isFlipNode;
        std::pair<char,char> move;
        CDCNode* parent;
        CDCNode* children[70]; //86? move 16*4 + jump 4 + flip 1


        CDCNode(Board*); // init
        CDCNode(Board*, CDCNode*, std::pair<char,char> move, bool f);
        ~CDCNode();
};

