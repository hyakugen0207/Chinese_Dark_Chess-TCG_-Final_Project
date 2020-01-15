#include "ZobristHashTable.hpp"
#include "Board.hpp"

ULL ZobristHashTable::randomPlyValue[2];
ULL ZobristHashTable::stateValue[6];
ULL ZobristHashTable::randomValue[18][32];
HashNode ZobristHashTable::hashNodes[HASH_NODE_NUM];

void ZobristHashTable::initStaticValue(){
    for(int i = 0 ; i < 18 ; ++i){
        for(int j = 0 ; j < 32 ; ++j){
            randomValue[i][j] = num(my_random_engine);
        }
    }
    randomPlyValue[0] = num(my_random_engine);
    randomPlyValue[1] = num(my_random_engine);
    for(int i = 0 ; i < 6 ; ++i){
        stateValue[i] = num(my_random_engine);
    }
};

ULL ZobristHashTable::updateHashValue(Board* board){
    ULL value = 0;
    //ply
    value ^= randomPlyValue[int(board->ply)];
    //state
    value ^= stateValue[RuleTable::rootState];
    //board
    for(int i = 11 ; i < 49 ; ++i){
        if(board->board[i]->piece != EMPTY){
            value ^= randomValue[int(board->board[i]->piece)][int(board->board[i]->position)];
        }
    }
    return value;
}; //init

ULL ZobristHashTable::updateHashValue(ULL oldValue,  char pieceFrom, char pieceFromPos, char pieceToPos, char pieceDead){
    return oldValue^randomPlyValue[0]^randomPlyValue[1]
            ^randomValue[int(pieceFrom)][int(pieceFromPos)]
            ^randomValue[int(pieceFrom)][int(pieceToPos)]
            ^randomValue[int(pieceDead)][int(pieceToPos)];
}; // jump or eat


ULL ZobristHashTable::updateHashValue(ULL oldValue, char pieceFrom, char pieceFromPos, char pieceToPos){
    return oldValue^randomPlyValue[0]^randomPlyValue[1]
        ^randomValue[int(pieceFrom)][int(pieceFromPos)]
        ^randomValue[int(pieceFrom)][int(pieceToPos)];
}; // move 

ULL ZobristHashTable::updateHashValue(ULL oldValue, char pieceFlip, char pieceFromPos){
    return oldValue^randomPlyValue[0]^randomPlyValue[1]
        ^randomValue[DARK][int(pieceFromPos)]
        ^randomValue[int(pieceFlip)][int(pieceFromPos)];
}; // flip