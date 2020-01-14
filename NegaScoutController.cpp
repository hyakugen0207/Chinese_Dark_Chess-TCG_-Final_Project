#include "NegaScoutController.hpp"
#include "Board.hpp"
#include "CDCNode.hpp"
#include <climits>
#include <unistd.h>
#include "NodePool.hpp"
#include "ZobristHashTable.hpp"

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

std::pair<char,char> NegaScoutController::iterativeDeepening(CDCNode* root, double timeLimit){
    timer(true); // reset timer
    int depth = 0;
    timeoff = false;
    //std::cerr << "1" << std::endl;
    std::pair<char,char> move = std::make_pair(0,0);
    while(true){
        std::cerr << depth << std::endl;
        if(timer() > timeLimit) break;
        //std::cerr << "1.2" << std::endl;
        root->board->setMoveList();

        std::cerr << "1.3" << root->board->moveList.size() << std::endl;
        if(root->board->moveList.empty()) return std::make_pair(0,0); //no move
       // std::cerr << "1.4" << std::endl;
        if(root->board->moveList[0].first==0) return root->board->moveList[1]; //early game
        //std::cerr << "1.5" << std::endl;

        int tmpScore = -10000;
        int rootScore = root->getScore();
        //std::cerr << "1.6" << std::endl;
        std::pair<char,char> tmpMove = std::make_pair(0,0);
        for(int i = 0 ; i < root->board->moveList.size() ; ++i){
            //std::cerr << "1.6.1" << std::endl;
            double searchScore;
            auto nextMove = root->board->moveList[i];
            if(root->childIndex<=i)
            {
                //std::cerr << "1.6.1.1" << std::endl;
                if(NodePool::empty())root->children[i] = new CDCNode(root, nextMove, false);
                else
                {
                    root->children[i] = NodePool::pop();
                    root->children[i]->copy(root, nextMove, false);
                }
                root->childIndex++;
              //  std::cerr << "1.6.1.2" << std::endl;
                // grow child , if this move is flip , CDCNode will become chance node
            }
            //std::cerr << "1.6.2" << std::endl;
            if(root->children[i]->move.first==root->children[i]->move.second) // chance node do minmax
            {
                
                for(int j = 0 ; j < root->children[i]->childIndex ; ++j){
                    root->children[i]->children[j]->growMinMaxTree(root->children[i]->children[j], 4);
                }
                //searchScore = rootScore-20;
                searchScore = root->children[i]->getChanceScore()*-1;

                std::cerr << "deep : " << depth << "  index : " << i << " flipScore = " << searchScore << std::endl;
            }
            else // do negaScout
            {  
 //               std::cerr << "1.6.2.1" << std::endl;
                searchScore = negaScout(root->children[i], depth, -10000, 10000, &timeLimit)*-1;
 //               std::cerr << "1.6.2.2" << std::endl;
                std::cerr << "deep : " << depth << "  index : " << i << " (" << int(nextMove.first) << "," << int(nextMove.second) << ")"<< " searchScore = " << searchScore << std::endl;
            }
 //          std::cerr << "1.6.3" << std::endl;
            if(!timeoff && tmpScore < searchScore)
            {
 //               std::cerr << "1.6.3.1" << std::endl;
                tmpScore = searchScore;
                tmpMove = root->board->moveList[i];
 //               std::cerr << "1.6.3.2" << std::endl;
            }
            if((timer() > timeLimit) || timeoff) 
            {
                if(move.first==0)
                {
                    return root->board->moveList[0];
                }
                return move;
            }         
        }
        if(timer() > timeLimit)
        {
            std::cerr << "break" <<std::endl;
            if(move.first==0)
            {
                return root->board->moveList[0];
            }
            return move;
        }
        //std::cerr << "1.7" << std::endl;
        if(!timeoff && (tmpMove.first!=0))
        {
            move = tmpMove;
        } 
        else
        {   
            std::cerr << "break2" <<std::endl;
            if(move.first==0)
            {
                return root->board->moveList[0];
            }
            return move;
        } 
       // std::cerr << "1.8" << std::endl;
        //std::cerr << "tmpScore in deep : " << depth << " is -> "<< tmpScore << std::endl;
        //std::cerr << "tmpMove in deep : " << depth << " is -> "<< int(tmpMove.first) << "," << int(tmpMove.second) << std::endl;
        ++depth;
    }
    //std::cerr << "2" << std::endl;
    if(move.first==0)
    {
        return root->board->moveList[0];
    }
    return move;
};

int NegaScoutController::negaScout(CDCNode* node, int depth, int alpha, int beta, double* timeLimit){
    //std::cerr << "a.1" << std::endl;
    int currentLowerBound = -10000; // fail soft
    int currentUpperBound = beta;
    /*
    ULL hashIndex = (node->hashValue)&(HASH_NODE_NUM-1);
    //std::cerr << "hashIndex" << hashIndex << std::endl;
    HashNode hashNode = ZobristHashTable::hashNodes[hashIndex];
    //std::cerr << "hashNode" << std::endl;
    int hashValueInTable = hashNode.value;
    if(hashNode.isSame(node->hashValue)) //hit
    {
        //std::cerr << "hit" << std::endl;
        if(depth <= hashNode.depth)
        {
            if(hashNode.exact==2)return hashValueInTable;
            else if(hashNode.exact==1)  //upper
            {
                beta = std::min(beta,hashValueInTable);
            }
            else if(hashNode.exact==0) //lower
            {
                alpha = std::max(alpha, hashValueInTable);  
            }

            if(alpha>=beta) return hashValueInTable; //cut off
        }
        else
        {
            if(hashNode.exact==2)currentLowerBound = hashValueInTable;
        }
    }
    else
    {
        //std::cerr << "no hit" << std::endl;
    }
    */
    if(timer() > (*timeLimit))
    {
       // std::cerr << "a.1.1" << std::endl;
       std::cerr << "break3" <<std::endl;
        timeoff = true;
        return -9000;
    }
    //std::cerr << "a.2" << std::endl;
    if(depth==0 || timeoff )
    {
       // std::cerr << "a.2.1" << std::endl;
        return node->getScore();
    }

    // if this board is in the hash table return value
       
   // std::cerr << "a.3" << std::endl;
    node->board->setMoveList();
   // std::cerr << "a.4" << std::endl;
    if(node->board->moveList.size() <= 1)
    {//rule here
        //std::cerr << "a.4.1" << std::endl;
        if(node->board->moveList.size()==1 && (node->board->moveList[0].first != node->board->moveList[0].second))
        {
           // std::cerr << "a.4.1.1" << std::endl;
        }
        else
        {
            //std::cerr << "a.4.1.2" << std::endl;
            return node->getScore();
        }
        //std::cerr << "a.4.2" << std::endl;
    }
   // std::cerr << "a.5" << std::endl;
    int tmpScore;
    for(int i = 0 ; (i < node->board->moveList.size()); ++i){
        //std::cerr << "a.5.1" << std::endl;
        auto nextMove = node->board->moveList[i];
        if(node->childIndex<=i)
        {   
            //std::cerr << "a.5.1.1" << std::endl;
            //new node (do not new chance node)
            if(nextMove.first == nextMove.second) continue;
            if(NodePool::empty())node->children[i] = new CDCNode(node, nextMove, false);
            else
            {
                node->children[i] = NodePool::pop();
                node->children[i]->copy(node, nextMove, false);
            }
            node->childIndex++;
            //std::cerr << "a.5.1.2" << std::endl;
        }
        //std::cerr << "a.5.2" << std::endl;
        tmpScore = negaScout(node->children[i], depth-1, currentUpperBound*-1, std::max(alpha,currentLowerBound)*-1, timeLimit)*-1;
        //std::cerr << "a.5.3" << std::endl;
        //std::cerr << "In negaScout index : " << i << " (" << int(nextMove.first) << "," << int(nextMove.second) << ")"<< " tmpScore = " << tmpScore << std::endl;
        if(tmpScore > currentLowerBound)
        {
          //  std::cerr << "a.5.3.1" << std::endl;
            if(currentUpperBound==beta || depth < 3 || tmpScore >= beta)
            {
            //    std::cerr << "a.5.3.1.1" << std::endl;
                currentLowerBound = tmpScore;
            }
            else //re-search
            {
              //  std::cerr << "a.5.3.1.2.1" << std::endl;
                currentLowerBound = negaScout(node->children[i], depth-1, beta*-1, tmpScore*-1, timeLimit)*-1;
                //std::cerr << "a.5.3.1.2.2" << std::endl;
            }   
            //std::cerr << "a.5.3.2" << std::endl;
        }
        //std::cerr << "a.5.4" << std::endl;
        if(currentLowerBound >= beta)
        {/*
            hashNode.depth = depth;
            hashNode.nextMove = node->move;
            hashNode.key = node->hashValue;
            hashNode.value = currentLowerBound;
            hashNode.exact = 0;*/
            return currentLowerBound; // cut-off
        }
       // std::cerr << "a.5.5" << std::endl;
        currentUpperBound = std::max(alpha, currentLowerBound)+1; // set up a null window
       // std::cerr << "a.5.6" << std::endl;
    }
    //std::cerr << "a.6" << std::endl;
    /*
    if(currentLowerBound>alpha)
    {
        hashNode.nextMove = node->move;
        hashNode.key = node->hashValue;
        hashNode.value = currentLowerBound;
        hashNode.exact = 2;
        hashNode.depth = depth;
    }
    else
    {
        hashNode.depth = depth;
        hashNode.nextMove = node->move;
        hashNode.key = node->hashValue;
        hashNode.value = currentLowerBound;
        hashNode.exact = 1;
    }
    */
    return currentLowerBound;
};