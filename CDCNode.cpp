#include "CDCNode.hpp"
#include "ZobristHashTable.hpp"
#include "Board.hpp"

int CDCNode::getScore(){
    return (this->depth%2==0) ? this->board->getScore() : this->board->getScore()*-1;
};

bool CDCNode::growChild(){
    this->board->setMoveList();
    if(this->board->moveList.size())
    {
        if(this->board->moveList[0].first==0)
        {
            //EarlyGame 
            this->move = this->board->moveList[1];
            return false;
        }
        else
        {
            // MidGame GrowTree
            for(int i = 0 ; i < this->board->moveList.size() ; ++i){
                auto move = this->board->moveList[i];
                if(move.first == move.second)
                {
                    // chance node
                    this->children[i] = new CDCNode(this, move, false);
                }
                else
                {
                    this->children[i] = new CDCNode(this, move, false);
                }
            }
            return true;
        }
        
    }
    return false; //沒路
};


CDCNode::CDCNode(CDCNode* p, std::pair<char,char> m, bool isFlip){
    //std::cerr << "new child CDCNode" << std::endl;
    this->lowerBound = -10000;
    this->upperBound = 10000;
    this->move = m; 
    this->parent = p;
    if(p->nodeType==0)
    {
        this->nodeType = 1;        
    }
    else if(p->nodeType==1)
    {
        this->nodeType = 0;
    }
    else
    {
        if(p->parent->nodeType==0)
        {
            this->nodeType = 1;

        }
        else
        {
            this->nodeType = 0;
        } 
    }
    
    this->board = new Board(p->board);
    //std::cerr << "new child CDCNode2" << std::endl;
    this->isFlipNode = isFlip;
    hashState = RuleTable::currentState;
    if(this->isFlipNode)
    {
        //std::cerr << "new child CDCNode isFlip" << int(m.first) << "," << int(m.second) << std::endl;
        this->depth = p->depth+1;
        hashValue = ZobristHashTable::updateHashValue(p->hashValue,this->board->board[m.first]->piece,m.first);
        this->board->flip(m.first,m.second);
        //std::cerr << "new child CDCNode isFlip done" << std::endl;
    }
    else
    {
        if(m.first==m.second)
        {
            // is chance node
            //std::cerr << "new child CDCNode is chance" << int(m.first) << "," << int(m.second) << std::endl;
            this->nodeType = 2;
            this->depth = p->depth;
            growFlipNode(m.first);
            //std::cerr << "new child CDCNode is chance done" << std::endl;
        }
        else
        {
            //std::cerr << "new child CDCNode is move" << int(m.first) << "," << int(m.second) << std::endl;
            this->depth = p->depth+1;
            hashValue = ZobristHashTable::updateHashValue(p->hashValue,this->board->board[m.first]->piece,m.first,m.second);
            this->board->move(m.first,m.second); 
            //std::cerr << "new child CDCNode is move done" << std::endl;
        }
    }
};

CDCNode::CDCNode(Board* b, char type){
    //std::cerr << "new CDCNode" << std::endl;
    this->nodeType = type;
    this->depth = 0;
    this->move = std::make_pair(0,0); 
    this->parent = nullptr;
    this->board = new Board(b);
    this->isFlipNode = false;
    hashState = RuleTable::currentState;
    hashValue = ZobristHashTable::updateHashValue(this->board);
    this->lowerBound = -10000;
    this->upperBound = 10000;
    //std::cerr << "end new CDCNode" << std::endl;
};

CDCNode::~CDCNode(){
    //std::cerr << "clean~~" << std::endl;
    for(int i = 0 ; i < 70 ; ++i){
        if(children[i]!=nullptr)
        {
            delete children[i];
        }
        else
        {
            break;
        }  
    }
    delete this->board;
};

void CDCNode::growFlipNode(char pos){
    std::cerr << "new chance" << std::endl;
    int index = 0;

    for(int i = 0 ; i < 2 ; ++i){
        for(int j = 0 ; j < 7 ; ++j){
            if(this->board->darkPieceNum[i][j]!=0)
            {
                if(i==0)
                {
                    this->children[i] = new CDCNode(this, std::make_pair(pos,i), true);
                }
                else
                {
                    this->children[i] = new CDCNode(this, std::make_pair(pos,i+8), true);
                }
            }
        } 
    }
    std::cerr << "end new chance" << std::endl;
};

double CDCNode::getChanceScore(){
    double score = 0;
    for(int i = 0 ; i < 70 ; ++i){
        if(this->children[i]==nullptr)
        {
            score /= i;
            break;
        }
        score += this->children[i]->getMinMaxScore();
    }
    return score;
};


int CDCNode::getMinMaxScore(){
    if(this->children[0]==nullptr)
    {
        return this->getScore();
    }

    if(this->nodeType==0) // min
    {
        int minScore = 10000;
        for(int i = 0 ; i < 70 ; ++i){
            if(this->children[i]==nullptr)
            {
                break;
            }
            int childScore = this->children[i]->getMinMaxScore();
            if(minScore > childScore)
            {
                minScore = childScore;
            }
        }
        return minScore;
    }
    else // max
    {
        int maxScore = -10000;
        for(int i = 0 ; i < 70 ; ++i){
            if(this->children[i]==nullptr)
            {
                break;
            }
            int childScore = this->children[i]->getMinMaxScore();
            if(maxScore < childScore)
            {
                maxScore = childScore;
            }
        }
        return maxScore;
    }
};

