#include "CDCNode.hpp"
#include "ZobristHashTable.hpp"
#include "Board.hpp"
#include "NodePool.hpp"

int CDCNode::getScore(){
    return (this->depth%2==0) ? this->board->getScore() : this->board->getScore()*-1;
};

bool CDCNode::growChild(){
    // un-used
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

void CDCNode::copy(CDCNode* p, std::pair<char,char> m, bool isFlip){
    this->score = -20000;
    this->move = m; 
    this->parent = p;
    this->childIndex = 0;
    /*
    for(int i = 0 ; i < 70 ; ++i){
        this->children[i] = nullptr;
    }
    */
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
    this->board->duplicate(p->board);

    this->isFlipNode = isFlip;
    if(this->isFlipNode)
    {
        //std::cerr << "new child CDCNode isFlip" << int(m.first) << "," << int(m.second) << std::endl;
        this->nodeType = 2;
        this->depth = p->depth+1;
        this->hashValue = ZobristHashTable::updateHashValue(p->hashValue,this->board->board[m.first]->piece,m.first);
        this->board->flip(m.first,m.second);
        //std::cerr << "new child CDCNode isFlip done" << std::endl;
    }
    else
    {
        if(m.first==m.second)
        {
            // is chance node
            //std::cerr << "new child CDCNode is chance" << int(m.first) << "," << int(m.second) << std::endl;
            this->depth = p->depth;
            growFlipNode(m.first);
            //std::cerr << "new child CDCNode is chance done" << std::endl;
        }
        else
        {
            //std::cerr << "new child CDCNode is move" << int(m.first) << "," << int(m.second) << std::endl;
            this->depth = p->depth+1;
            this->hashValue = ZobristHashTable::updateHashValue(p->hashValue,this->board->board[m.first]->piece,m.first,m.second);
            this->board->move(m.first,m.second); 
            //std::cerr << "new child CDCNode is move done" << std::endl;
        }
    }

};

CDCNode::CDCNode(CDCNode* p, std::pair<char,char> m, bool isFlip){
    //std::cerr << "new child CDCNode" << std::endl;
    this->score = -20000;
    this->move = m; 
    this->parent = p;
    this->childIndex = 0;
    /*
    for(int i = 0 ; i < 70 ; ++i){
        this->children[i] = nullptr;
    }
    */
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
    if(this->isFlipNode)
    {
        //std::cerr << "new child CDCNode isFlip" << int(m.first) << "," << int(m.second) << std::endl;
        this->nodeType = 2;
        this->depth = p->depth+1;
        this->hashValue = ZobristHashTable::updateHashValue(p->hashValue,this->board->board[m.first]->piece,m.first);
        this->board->flip(m.first,m.second);
        //std::cerr << "new child CDCNode isFlip done" << std::endl;
    }
    else
    {
        if(m.first==m.second)
        {
            // is chance node
            //std::cerr << "new child CDCNode is chance" << int(m.first) << "," << int(m.second) << std::endl;
            this->depth = p->depth;
            growFlipNode(m.first);
            //std::cerr << "new child CDCNode is chance done" << std::endl;
        }
        else
        {
            //std::cerr << "new child CDCNode is move" << int(m.first) << "," << int(m.second) << std::endl;
            this->depth = p->depth+1;
            this->hashValue = ZobristHashTable::updateHashValue(p->hashValue,this->board->board[m.first]->piece,m.first,m.second);
            this->board->move(m.first,m.second); 
            //std::cerr << "new child CDCNode is move done" << std::endl;
        }
    }
};

void CDCNode::copy(Board* b, char type){
    this->nodeType = type;
    this->depth = 0;
    this->move = std::make_pair(0,0); 
    this->parent = nullptr;
    this->board->duplicate(b);
    this->isFlipNode = false;
    this->hashValue = ZobristHashTable::updateHashValue(this->board);
    this->score = -20000;
    this->childIndex = 0;
    /*
    for(int i = 0 ; i < 70 ; ++i){
        children[i] = nullptr;
    }
    */
};

CDCNode::CDCNode(Board* b, char type){
    //std::cerr << "new CDCNode" << std::endl;
    this->nodeType = type;
    this->depth = 0;
    this->move = std::make_pair(0,0); 
    this->parent = nullptr;
    this->board = new Board(b);
    this->isFlipNode = false;
    //hashState = RuleTable::currentState;
    this->hashValue = ZobristHashTable::updateHashValue(this->board);
    this->score = -20000;
    this->childIndex = 0;
    /*
    for(int i = 0 ; i < 70 ; ++i){
        children[i] = nullptr;
    }
    */
    //std::cerr << "end new CDCNode" << std::endl;
};

CDCNode::~CDCNode(){
    //std::cerr << "clean~~" << std::endl;
    for(int i = 0 ; i < childIndex ; ++i){
        delete this->children[i];
    }
    delete this->board;
};

void CDCNode::growFlipNode(char pos){
    for(int i = 0 ; i < 2 ; ++i){
        for(int j = 0 ; j < 7 ; ++j){
            if(this->board->darkPieceNum[i][j]!=0)
            {
                if(i==0)
                {
                    if(NodePool::empty())this->children[childIndex] = new CDCNode(this, std::make_pair(pos,j), true);
                    else
                    {
                        this->children[childIndex] = NodePool::pop();
                        this->children[childIndex]->copy(this, std::make_pair(pos,j), true);
                    } 
                }
                else
                {
                    if(NodePool::empty())this->children[childIndex] = new CDCNode(this, std::make_pair(pos,j+8), true);
                    else
                    {
                        this->children[childIndex] = NodePool::pop();
                        this->children[childIndex]->copy(this, std::make_pair(pos,j+8), true);
                    } 
                }
                ++childIndex;
            }
        } 
    }
};

double CDCNode::getChanceScore(){
    double score = 0;
    int index = 0;

    for(int i = 0 ; i < 2 ; ++i){
        for(int j = 0 ; j < 7 ; ++j){
            if(this->board->darkPieceNum[i][j]!=0)
            {
                if(index >= childIndex) return score;
                //std::cerr << "index :" << index << std::endl; 
                //score += double(this->children[index]->getMinMaxScore())*this->board->flipPossibility[i][j];
                score += double(this->children[index]->getScore())*this->board->flipPossibility[i][j];
                ++index;
            }
        }
    }
    
    return score;

    /*
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
    */
};

void CDCNode::growMinMaxTree(CDCNode* node, int deep){
    if(deep<0) return;
    if(node->childIndex!=0) return;
    node->board->setMoveList();
    if(!node->board->moveList.empty())
    {
        if(node->board->moveList[0].first==0)
        {
            return;
        }
        else
        {
            // MidGame GrowTree
            for(int i = 0 ; i < node->board->moveList.size() ; ++i){
                auto move = node->board->moveList[i];
                if(move.first == move.second)
                {
                    break;
                }
                else
                {   
                    if(NodePool::empty())node->children[i] = new CDCNode(node, move, false);
                    else
                    {
                        node->children[i] = NodePool::pop();
                        node->children[i]->copy(node, move, false);
                    }
                    
                    node->childIndex++;
                    growMinMaxTree(node->children[i], deep-1);
                }
            }
            return;
        }
        
    }
    return ; //沒路
};

int CDCNode::getMinMaxScore(){


    if(childIndex==0)
    {
        return this->getScore();
    }

    int score = -20000;

    for(int i = 0 ; i < childIndex ; ++i){
        int t = this->children[i]->getMinMaxScore()*-1;
        if(t > score){
            score = t;
        }
    }
    return score;
};

