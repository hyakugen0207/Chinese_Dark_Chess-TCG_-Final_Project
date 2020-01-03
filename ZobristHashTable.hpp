


#define HASH_NODE_NUM 1073741824 //2^32


class ZobristHashTable{
    public :
        static unsigned long long int randomValue[15][32];
        static unsigned long long int randomPlyValue[2];
        HashNode hashNodes[HASH_NODE_NUM];

        int getHashValue(bool ply, int oldValue); // parameter move
};


class HashNode{
    public :
        unsigned long long int key;
        int depth;
        int value;
        bool ply;
        char nextMove;

        bool isSame(unsigned long long int compareKey);
};