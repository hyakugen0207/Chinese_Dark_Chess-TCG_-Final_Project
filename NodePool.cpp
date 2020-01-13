#include "NodePool.hpp"
#include "CDCNode.hpp"

std::stack<CDCNode*> NodePool::pool;

CDCNode* NodePool::pop(){
    CDCNode* tmp = pool.top();
    pool.pop();
    return tmp;
};

void NodePool::push(CDCNode* node){
    pool.push(node);
};
bool NodePool::empty(){
    return pool.empty();
};

void NodePool::recycle(CDCNode* node){
    if(node->childIndex==0)
    {
        push(node);
        return;
    }

    push(node);
    for(int i = 0 ; i < node->childIndex ; ++i){
        recycle(node->children[i]);
    }
    node->childIndex = 0;
};
