#pragma once
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
        char exact; // 0 = lower , 1 = upper , 2 = exact
        std::pair<char,char> nextMove;
        bool isSame(ULL compareKey){
            return compareKey==this->key;
        };

        HashNode(): key(-1), depth(-1), value(-20000),exact(0),nextMove(std::make_pair(99,99)){};
    
};


class ZobristHashTable{
    public :
        static ULL randomValue[18][32];
        static ULL randomPlyValue[2];
        static ULL initValue;
        static void initStaticValue();

        static HashNode hashNodes[HASH_NODE_NUM];
        
        static ULL updateHashValue(Board* board); //init
        static ULL updateHashValue(ULL oldValue, char pieceFrom, char pieceFromPos, char pieceToPos, char pieceDead); // jump or eat
        static ULL updateHashValue(ULL oldValue, char pieceFrom, char pieceFromPos, char pieceToPos); // move
        static ULL updateHashValue(ULL oldValue, char pieceFlip, char pieceFromPos); // flip
    private:

        
};


