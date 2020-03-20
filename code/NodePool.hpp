#pragma once

#include<stack>

class CDCNode;

class NodePool{
    public:
        static std::stack<CDCNode*> pool;
        static CDCNode* pop();
        static void push(CDCNode*);
        static bool empty();
        static void recycle(CDCNode*);
};