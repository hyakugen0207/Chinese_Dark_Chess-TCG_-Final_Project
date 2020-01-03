#include "myDefine.hpp"
#include "Board.hpp"
#include <climits>
#include <cstdlib>
#include <iostream>





class HashNode{
    public :
        ULL key;
        int depth;
        int value;
        bool ply;
        char nextMove;
        bool isSame(ULL compareKey);
};


class ZobristHashTable{
    public :
        static ULL randomValue[15][32];
        static ULL randomPlyValue[2];
        static ULL initValue;
        static void initStaticValue();

        HashNode hashNodes[HASH_NODE_NUM];
        
        ULL updateHashValue(Board* board); //init
        ULL updateHashValue(ULL oldValue, char pieceFrom, char pieceFromPos, char pieceToPos, char pieceDead); // jump or eat
        ULL updateHashValue(ULL oldValue, char pieceFrom, char pieceFromPos, char pieceToPos); // move
        ULL updateHashValue(ULL oldValue, char pieceFlip, char pieceFromPos); // flip
    private:

        
};


