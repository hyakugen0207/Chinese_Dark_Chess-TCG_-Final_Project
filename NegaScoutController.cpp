#include "NegaScoutController.hpp"
#include "Board.hpp"
#include "CDCNode.hpp"
#include <climits>


auto timer = [] ( bool reset = false ) -> double {
    static decltype(std::chrono::steady_clock::now()) tick, tock;
    if ( reset ) {
        tick = std::chrono::steady_clock::now();
        return (0);
    }
    tock = std::chrono::steady_clock::now();
    return (std::chrono::duration_cast<std::chrono::duration<double>>(tock-tick).count());
};

bool NegaScoutController::timeoff = false;

std::pair<char,char> NegaScoutController::iterativeDeepening(CDCNode* root, double timeLimit)
{
    timer(true); // reset timer
    int depth = 1;
    timeoff = false;
    std::pair<char,char> move;

    while(true){
        if(timer() > timeLimit) break;

        root->board->setMoveList();

        if(root->board->moveList.size()==0) return std::make_pair(0,0); //no move
        if(root->board->moveList[0].first==0) return root->board->moveList[1]; //early game
        int tmpScore = -10000;
        std::pair<char,char> tmpMove;
        for(int i = 0 ; i < root->board->moveList.size() ; ++i){
            auto nextMove = root->board->moveList[i];
            int searchScore;
            if(root->children[i]==nullptr)
            {
                root->children[i] = new CDCNode(root, root->board->moveList[i], false);
                // grow child , if this move is flip , CDCNode will become chance node
            }
            
            if(root->children[i]->nodeType==2) // chance node do minmax
            {

            }
            else // do negaScout
            {  
                searchScore = negaScout(root->children[i], depth, INT_MIN, INT_MAX, &timeLimit);
            }

            if(!timeoff && tmpScore < searchScore)
            {
                tmpScore = searchScore;
                tmpMove = root->board->moveList[i];
            }

            if(timeoff) break;
        }

        
        if(!timeoff) move = tmpMove;
        else break;

        ++depth;
    }

    return move;
}

int NegaScoutController::negaScout(CDCNode* node, int depth, int alpha, int beta, double* timeLimit){
    
    node->board->setMoveList();

    if(timer() > (*timeLimit)) timeoff = true;

    if(depth==0 || node->board->moveList.size()==0 || timeoff)//rule here
    {
        return node->getScore();
    }

    int currentLowerBound = INT_MIN; // fail soft
    int currentUpperBound = beta;
    int tmpScore;
    for(int i = 0 ; (i < node->board->moveList.size()) && !timeoff ; ++i){
        auto nextMove = node->board->moveList[i];
        if(node->children[i]==nullptr)
        {
            //new node (do not new chance node)
            if(nextMove.first == nextMove.second) continue;
            node->children[i] = new CDCNode(node, nextMove, false);
        }
        tmpScore = -negaScout(node->children[i], depth-1, -currentUpperBound, -std::max(alpha,currentLowerBound), timeLimit);
        if(tmpScore > currentLowerBound)
        {
            if(currentUpperBound==beta || depth < 3 || tmpScore >= beta)
            {
                currentLowerBound = tmpScore;
            }
            else //re-search
            {
                currentLowerBound = -negaScout(node->children[i], depth-1, -beta, -tmpScore, timeLimit);
            }   
        }

        if(currentLowerBound >= beta)
        {
            return currentLowerBound; // cut-off
        }

        currentUpperBound = std::max(alpha, currentLowerBound)+1; // set up a null window
    }
    return currentUpperBound;
};