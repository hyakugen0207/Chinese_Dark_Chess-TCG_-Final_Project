#pragma once
#include "myDefine.hpp"
class Board;

class CDCNode{
    public:
        Board* board;
        ULL hashValue;
        char hashState;
        bool isFlipNode;
        int lowerBound;
        int upperBound;
        char nodeType;
        int depth;
        std::pair<char,char> move;
        CDCNode* parent;
        CDCNode* children[70]; //86? move 16*4 + jump 4 + flip 1

        int getScore();
        bool growChild();
        double getChanceScore(); // for chanceNode
        int getMinMaxScore();  // for chanceNode
        //int getNegaScoutScore();

        CDCNode(Board*, char type); // init
        CDCNode(CDCNode*, std::pair<char,char> move, bool f);
        ~CDCNode();
    private:
        void growFlipNode(char flipPos); // for chanceNode
};

