#include "myDefine.hpp"
#include <climits>
#include <cstdlib>
#include <iostream>



class Board;

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
        static ULL randomValue[18][32];
        static ULL randomPlyValue[2];
        static ULL initValue;
        static void initStaticValue();

        HashNode hashNodes[HASH_NODE_NUM];
        
        static ULL updateHashValue(Board* board); //init
        static ULL updateHashValue(ULL oldValue, char pieceFrom, char pieceFromPos, char pieceToPos, char pieceDead); // jump or eat
        static ULL updateHashValue(ULL oldValue, char pieceFrom, char pieceFromPos, char pieceToPos); // move
        static ULL updateHashValue(ULL oldValue, char pieceFlip, char pieceFromPos); // flip
    private:

        
};


