#pragma once
#include <chrono> 
#include <iostream>

class CDCNode;
class Board;



class NegaScoutController{
    public:
        static bool timeoff;
        static int nonFlipcount; 
        static std::pair<char,char> iterativeDeepening(CDCNode* node, double timeLimit);
        static int negaScout(CDCNode* node, int depth, int alpha, int beta, double* timeLimit);
};
