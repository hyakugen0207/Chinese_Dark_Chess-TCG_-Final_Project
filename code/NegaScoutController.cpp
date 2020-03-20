#include "NegaScoutController.hpp"
#include "Board.hpp"
#include "CDCNode.hpp"
#include <climits>
#include <unistd.h>
#include "NodePool.hpp"
#include "ZobristHashTable.hpp"

std::pair<char,char> myPremove = std::make_pair(0,0);
std::pair<char,char> myPrePremove = std::make_pair(0,0);

bool earlyReturn = false;
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
int NegaScoutController::nonFlipcount = 0;

std::pair<char,char> NegaScoutController::iterativeDeepening(CDCNode* root, double timeLimit){
    timer(true); // reset timer
    int depth = 0;
    timeoff = false;
    bool genFlip = true;
    double flipSearchScore = -15000;
    int moveScore = -10000; 
    std::vector<std::vector<int>> scoreHistory;
    std::vector<int> scoreList;
    int rootScore = root->getScore();
    
    std::cerr << "root" <<  rootScore << std::endl;
    std::pair<char,char> move = std::make_pair(0,0);
    while(depth<8){
        if(timer() > timeLimit) break;
        std::cerr << "deep" << depth << std::endl;

        root->board->setMoveList(true);

        if(root->board->moveList.empty()) return std::make_pair(0,0); //no move
        if(root->board->moveList[0].first==0) return root->board->moveList[1]; //early game
        if(root->board->moveList.size()==1) return root->board->moveList[0];

        int tmpScore = -10000;
        int rootScore = root->getScore();
        std::pair<char,char> tmpMove = std::make_pair(0,0);
        for(int i = 0 ; i < root->board->moveList.size() ; ++i){
            //std::cerr << "1.6.1" << std::endl;
            double searchScore = -20000;
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
                // grow child , if this move is flip , CDCNode will become chance node
            }
            if(root->children[i]->move.first==root->children[i]->move.second) // chance node do minmax
            {
                if(((tmpScore > rootScore) && nonFlipcount<5) || depth > 4 || flipSearchScore!=-15000) continue;
                
                for(int j = 0 ; j < root->children[i]->childIndex ; ++j){
                    root->children[i]->children[j]->growMinMaxTree(root->children[i]->children[j], 3);
                    //std::cerr << "child" << j << ":";
                }
                flipSearchScore = root->children[i]->getChanceScore()*-1;
                if(nonFlipcount>=2)
                {
                    flipSearchScore += double(30)*double(nonFlipcount>>2);
                }
                
                
                std::cerr << "deep : " << depth << "  index : " << i << " flipScore = " << flipSearchScore  << " state : " << RuleTable::rootState << std::endl;
            }
            else // do negaScout
            {  
                earlyReturn = false;
                searchScore = negaScout(root->children[i], depth, -10000, 10000, &timeLimit)*-1;
                if(root->board->moveList.size()!=1 && nextMove==myPrePremove && searchScore>0) searchScore = -8000;
                scoreList.emplace_back(searchScore);
                
                std::cerr << "deep : " << depth << "  index : " << i << " (" << int(nextMove.first) << "," << int(nextMove.second) << ")"<< " searchScore = " << searchScore << " state : " << RuleTable::rootState << std::endl;
            }
            if(!timeoff && tmpScore < searchScore)
            {
                tmpScore = searchScore;
                tmpMove = root->board->moveList[i];
            }
            if((timer() > timeLimit) || timeoff) 
            {
                if(move.first==0)
                {
                    myPrePremove = myPremove;
                    myPremove = root->board->moveList[0];
                    return root->board->moveList[0];
                }
                break;
            }         
        }
        if(timer() > timeLimit)
        {
            if(move.first==0)
            {
                myPrePremove = myPremove;
                myPremove = root->board->moveList[0];
                return root->board->moveList[0];
            }
            break;
        }
        //std::cerr << "1.7" << std::endl;
        if(!timeoff && (tmpMove.first!=0))
        {
            move = tmpMove;
        } 
        else
        {   
           // std::cerr << "break2" <<std::endl;
            if(move.first==0)
            {
                myPrePremove = myPremove;
                myPremove = root->board->moveList[0];
                return root->board->moveList[0];
            }
            break;
        } 
       // std::cerr << "1.8" << std::endl;
        std::cerr << "tmpScore in deep : " << depth << " is -> "<< tmpScore << std::endl;
        std::cerr << "tmpMove in deep : " << depth << " is -> "<< int(tmpMove.first) << "," << int(tmpMove.second) << std::endl;
        depth++;
        moveScore = tmpScore;
        scoreHistory.emplace_back(scoreList);
        scoreList.clear();
    }
    //std::cerr << "2" << std::endl;
    
    if(moveScore > rootScore) // 找到最早達到此分數的走步
    {
        for(int i = scoreHistory.size()-1 ; i >= 0 ; --i){
            bool find = false;
            for(int j = scoreHistory[i].size()-1 ; j >= 0 ; --j){
                if(scoreHistory[i][j] == moveScore)
                {
                    find = true;
                    move = root->board->moveList[j];
                }
            }
            if(!find) break;
        }   
    }
    else //若整層都同分 則往前找到有一層不全都分中的最高分
    {
        for(int i = scoreHistory.size()-1 ; i >= 0 ; --i){
            bool sameScore = true;
            int localmax = moveScore;
            for(int j = scoreHistory[i].size()-1 ; j >= 0 ; --j){
                if(scoreHistory[i][j] != moveScore)
                {
                    sameScore = false;
                    if(scoreHistory[i][j] > localmax)
                    {
                        localmax = scoreHistory[i][j];
                        move = root->board->moveList[j];
                    }
                }
            }
            if(!sameScore) break;
        }
    }
    

     // if move is not eat move / compare to flipScore
    if(!RuleTable::LEGAL_EAT_ARRAY[root->board->board[move.first]->piece][root->board->board[move.second]->piece] || moveScore < rootScore)
    {
        if(flipSearchScore >= moveScore)
        {
            move = root->board->moveList[root->childIndex-1];
        }
    }
    if(move.first==0)
    {
        myPrePremove = myPremove;
        myPremove = root->board->moveList[0];
        return root->board->moveList[0];
    }
    if(move.first==move.second)
    {
        nonFlipcount=0;
    }
    else nonFlipcount++;

    myPrePremove = myPremove;
    myPremove = move;
    return move;
};

int NegaScoutController::negaScout(CDCNode* node, int depth, int alpha, int beta, double* timeLimit){
    int oriAlpha = alpha;
    int currentLowerBound = -10000; // fail soft
    int currentUpperBound = beta;
    int tag = 0;
    bool isDeeper = false;
    
    ULL hashIndex = (node->hashValue)>>32;
    HashNode hashNode = ZobristHashTable::hashNodes[hashIndex];
    int hashValueInTable = hashNode.value;
    /*
    if(hashNode.isSame(node->hashValue)) //hit
    {
        if(depth <= hashNode.depth)
        {
            if(hashNode.exact==2)
            {
                //std::cerr << "return exact d,hashD" <<  depth << "," <<  hashNode.depth << "," << hashValueInTable << std::endl;
                return hashValueInTable;
            }
            else if(hashNode.exact==1)  //upper
            {
                beta = std::min(beta,hashValueInTable);
            }
            else if(hashNode.exact==0) //lower
            {
                alpha = std::max(alpha, hashValueInTable);  
            }
            if(alpha>=beta){return hashValueInTable;}//cut off
        }
        else
        {
            //if(hashNode.exact==2)currentUpperBound = hashValueInTable; 
        }
    }
    */
    
    if(timer() > (*timeLimit))
    {
       // std::cerr << "a.1.1" << std::endl;
       //std::cerr << "break3" <<std::endl;
        timeoff = true;
        earlyReturn = true;
        return -9000;
    }
    //std::cerr << "a.2" << std::endl;
    if(depth==0 || timeoff)
    {
       if(!timeoff && depth==0 && node->board->haveEatMove())
       {
           //std::cerr << "expand" << node->depth << std::endl;
           //return node->getScore();
           depth++;
           tag = 2;
           isDeeper = true;
       }
       else
       {
           return node->getScore();
       }
    }

    // if this board is in the hash table return value
       
   // std::cerr << "a.3" << std::endl;
    node->board->setMoveList(tag);
   // std::cerr << "a.4" << std::endl;
    if(node->board->moveList.size() <= 1)
    {//rule here
        //std::cerr << "a.4.1" << std::endl;
        if(node->board->moveList.size()==1 && (node->board->moveList[0].first != node->board->moveList[0].second))
        {
            //std::cerr << "a.4.1.1" << std::endl;
        }
        else
        { 
            if(isDeeper) return node->board->getScore();
            earlyReturn = true;
            if(node->board->numAlivePieces[node->board->ply]==0) return -10000;
            return -6000;
        }
        //std::cerr << "a.4.2" << std::endl;
    }
    //std::cerr << "a.5" << std::endl;
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
        if(currentLowerBound >= beta && !timeoff)
        {
            break;
        }
       // std::cerr << "a.5.5" << std::endl;
        currentUpperBound = std::max(alpha, currentLowerBound)+1; // set up a null window
       // std::cerr << "a.5.6" << std::endl;
    }
    //std::cerr << "a.6" << std::endl;
   

    if(currentLowerBound<=oriAlpha && !timeoff && !earlyReturn)
    {
        ZobristHashTable::hashNodes[hashIndex].depth = depth;
        ZobristHashTable::hashNodes[hashIndex].nextMove = node->move;
        ZobristHashTable::hashNodes[hashIndex].key = node->hashValue;
        ZobristHashTable::hashNodes[hashIndex].value = currentLowerBound;
        ZobristHashTable::hashNodes[hashIndex].exact = 1;
    }
    else if(currentLowerBound >= beta && !timeoff /*&& !earlyReturn*/)
    {
        ZobristHashTable::hashNodes[hashIndex].depth = depth;
        ZobristHashTable::hashNodes[hashIndex].nextMove = node->move;
        ZobristHashTable::hashNodes[hashIndex].key = node->hashValue;
        ZobristHashTable::hashNodes[hashIndex].value = currentLowerBound;
        ZobristHashTable::hashNodes[hashIndex].exact = 0;
        //std::cerr << "cut move : " << int(node->move.first) << "," << int(node->move.second) <<"return:" << currentLowerBound << std::endl;
        return currentLowerBound; // cut-off
    }
    else if(!timeoff && !earlyReturn)
    {
        ZobristHashTable::hashNodes[hashIndex].nextMove = node->move;
        ZobristHashTable::hashNodes[hashIndex].key = node->hashValue;
        ZobristHashTable::hashNodes[hashIndex].value = currentLowerBound;
        ZobristHashTable::hashNodes[hashIndex].exact = 2;
        ZobristHashTable::hashNodes[hashIndex].depth = depth;
    }
    
    //std::cerr << "move : " << int(node->move.first) << "," << int(node->move.second) <<"return:" << currentLowerBound << std::endl;
    return currentLowerBound;
};