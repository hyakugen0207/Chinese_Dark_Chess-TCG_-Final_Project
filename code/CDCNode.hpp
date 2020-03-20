#pragma once
#include "myDefine.hpp"
class Board;

class CDCNode{
    public:
        Board* board;
        ULL hashValue;
        bool isFlipNode;
        int score;
        //char nodeType;
        int depth;
        int childIndex;
        std::pair<char,char> move;
        CDCNode* parent;
        CDCNode* children[70]; //86? move 16*4 + jump 4 + flip 1
        static int nodecount;
        int getScore();
        int getRootScore();
        int getNoMoveScore();
        bool growChild();
        double getChanceScore(); // for chanceNode
        int getMinMaxScore();  // for chanceNode
        void growMinMaxTree(CDCNode*, int);
        void copy(Board*, char type);
        void copy(CDCNode*, std::pair<char,char> move, bool f);
        void debug();
        CDCNode(Board*, char type); // init
        CDCNode(CDCNode*, std::pair<char,char> move, bool f);
        ~CDCNode();
    private:
        void growFlipNode(char flipPos); // for chanceNode
};

