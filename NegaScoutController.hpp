#pragma once
#include <chrono> 
#include <iostream>

class CDCNode;
class Board;



class NegaScoutController{
    
    static bool timeoff;

    static std::pair<char,char> iterativeDeepening(CDCNode* node, double timeLimit);
    static int negaScout(CDCNode* node, int depth, int alpha, int beta, double* timeLimit);
};
