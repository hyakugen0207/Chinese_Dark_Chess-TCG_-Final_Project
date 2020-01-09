#include "CDCNode.hpp"
#include "ZobristHashTable.hpp"

CDCNode::CDCNode(Board* b, CDCNode* p, std::pair<char,char> m, bool isFlip){
    this->move = m; 
    this->parent = p;
    this->board = new Board(b);
    this->isFlipNode = isFlip;
    hashState = RuleTable::currentState;
    if(this->isFlipNode)
    {
        hashValue = ZobristHashTable::updateHashValue(p->hashValue,this->board->board[m.first]->piece,m.first);
        this->board->flip(m.first,m.second);
    }
    else
    {
        hashValue = ZobristHashTable::updateHashValue(p->hashValue,this->board->board[m.first]->piece,m.first,m.second);
        this->board->move(m.first,m.second);   
    }
};

CDCNode::CDCNode(Board* b){
    this->move = std::make_pair(0,0); 
    this->parent = nullptr;
    this->board = new Board(b);
    this->isFlipNode = false;
    hashState = RuleTable::currentState;
    hashValue = ZobristHashTable::updateHashValue(this->board);
};

CDCNode::~CDCNode(){

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
}